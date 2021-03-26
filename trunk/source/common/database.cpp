#include "stdafx.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <sstream>
#include "database.h"
#include "log.h"
#include "EmuAssert.h"

using namespace std;

Database::Database() {
	mysql_library_init(0, nullptr, nullptr);
	port = 3306;
}

Database::~Database() {
	m_connection_pool.Lock();
	while (!connection_pool.empty()) {
		delete connection_pool.top();
		connection_pool.pop();
	}
	m_connection_pool.Unlock();
	mysql_library_end();
}

void Database::SetHost(const char* p_host) {
	host = p_host;
}

void Database::SetPort(unsigned int port) {
	port = port;
}

void Database::SetUser(const char* p_user) {
	user = p_user;
}

void Database::SetPassword(const char* p_password) {
	password = p_password;
}

void Database::SetDatabase(const char* p_db) {
	db = p_db;
}

const char* Database::GetHost() {
	return host.c_str();
}

unsigned int Database::GetPort() {
	return port;
}

const char* Database::GetDatabase() {
	return db.c_str();
}

bool Database::ValidateConfig() {
	if (host.empty()) {
		LogError(LOG_DATABASE, 0, "Database config is invalid: 'host' not set");
		return false;
	}
	if (user.empty()) {
		LogError(LOG_DATABASE, 0, "Database config is invalid: 'user' not set");
		return false;
	}
	if (password.empty()) {
		LogError(LOG_DATABASE, 0, "Database config is invalid: 'password' not set");
		return false;
	}
	if (db.empty()) {
		LogError(LOG_DATABASE, 0, "Database config is invalid: 'db' not set");
		return false;
	}

	return true;
}

#define CONNECTION_POOL_SIZE 10

bool Database::Connect() {
	SpinLocker lock(m_connection_pool);
	for (int i = 0; i < CONNECTION_POOL_SIZE; i++) {
		DatabaseConnection* con = OpenNewConnection();
		if (!con) {
			break;
		}

		connection_pool.push(con);
	}

	return true;
}

DatabaseConnection* Database::OpenNewConnection() {
	DatabaseConnection* con = new DatabaseConnection(host.c_str(), port, user.c_str(), password.c_str(), db.c_str());
	if (con->bError) {
		delete con;
		con = nullptr;
	}

	return con;
}

QueryResult Database::QueryWithFetchedResult(uint32_t result_flags, const char* fmt, ...) {
	bool success = true;
	int count;
	va_list ap;
	my_ulonglong affected_rows = 0;
	my_ulonglong last_insert_id = 0;

	std::unique_ptr<char[]> oversizedBuf;
	char buf[4096];

	va_start(ap, fmt);
	count = vsnprintf(buf, sizeof(buf), fmt, ap);

	const char* query = buf;
	if (count > static_cast<int>(sizeof(buf) - 1)) {
		oversizedBuf.reset(new char[count + 1]);
		vsprintf(oversizedBuf.get(), fmt, ap);
		query = oversizedBuf.get();
	}
	va_end(ap);

	LogDebug(LOG_DATABASE, 5, "Query:\n%s", query);

	DatabaseConnection* connection = GetPooledConnection();
	MYSQL* mysql = connection->mysql_con;
	if (mysql_real_query(mysql, query, (unsigned long)count) != 0) {
		LogError(LOG_DATABASE, 0, "Error running MySQL query (%d): %s\n%s", mysql_errno(mysql), mysql_error(mysql), query);
		success = false;
	}
	else if (MYSQL_RES* res = mysql_use_result(mysql)) {
		mysql_free_result(res);
	}

	if (success && result_flags) {
		if (result_flags & QUERY_RESULT_FLAG_AFFECTED_ROWS) {
			affected_rows = mysql_affected_rows(mysql);
		}
		if (result_flags & QUERY_RESULT_FLAG_LAST_INSERT_ID) {
			last_insert_id = mysql_insert_id(mysql);
		}
	}

	while (mysql_next_result(mysql) == 0) {
		if (MYSQL_RES* res = mysql_use_result(mysql)) {
			mysql_free_result(res);
		}
	}

	AddConnectionToPool(connection);

	return QueryResult(success, affected_rows, last_insert_id);
}

bool Database::Query(const char* fmt, ...) {
	bool success = true;
	int count;
	va_list ap;

	std::unique_ptr<char[]> oversizedBuf;
	char buf[4096];

	va_start(ap, fmt);
	count = vsnprintf(buf, sizeof(buf), fmt, ap);

	const char* query = buf;
	if (count > static_cast<int>(sizeof(buf) - 1)) {
		oversizedBuf.reset(new char[count + 1]);
		vsprintf(oversizedBuf.get(), fmt, ap);
		query = oversizedBuf.get();
	}
	va_end(ap);

	if (query == NULL) {
		LogError(LOG_DATABASE, 0, "Out of memory trying to allocate database query in %s:%u", __FUNCTION__, __LINE__);
		return false;
	}

	LogDebug(LOG_DATABASE, 5, "Query:\n%s", query);

	DatabaseConnection* connection = GetPooledConnection();
	MYSQL* mysql = connection->mysql_con;
	if (mysql_real_query(mysql, query, (unsigned long)count) != 0) {
		LogError(LOG_DATABASE, 0, "Error running MySQL query (%d): %s\n%s", mysql_errno(mysql), mysql_error(mysql), query);
		success = false;
	}
	else if (MYSQL_RES* res = mysql_use_result(mysql)) {
		mysql_free_result(res);
	}

	while (mysql_next_result(mysql) == 0) {
		if (MYSQL_RES* res = mysql_use_result(mysql)) {
			mysql_free_result(res);
		}
	}

	AddConnectionToPool(connection);

	return success;
}

bool Database::Select(DatabaseResult* result, const char* fmt, ...) {
	bool success = true;
	int count;
	va_list ap;
	MYSQL_RES* res;

	std::unique_ptr<char[]> oversizedBuf;
	char buf[4096];

	va_start(ap, fmt);
	count = vsnprintf(buf, sizeof(buf), fmt, ap);

	const char* query = buf;
	if (count > static_cast<int>(sizeof(buf) - 1)) {
		oversizedBuf.reset(new char[count + 1]);
		vsprintf(oversizedBuf.get(), fmt, ap);
		query = oversizedBuf.get();
	}
	va_end(ap);

	if (query == NULL) {
		LogError(LOG_DATABASE, 0, "Out of memory trying to allocate database query in %s:%u", __FUNCTION__, __LINE__);
		return false;
	}

	LogDebug(LOG_DATABASE, 5, "Select:\n%s", query);

	DatabaseConnection* connection = GetPooledConnection();
	MYSQL* mysql = connection->mysql_con;
	if (mysql_real_query(mysql, query, (unsigned long)count) != 0) {
		LogError(LOG_DATABASE, 0, "Error running MySQL query (%d): %s\n%s", mysql_errno(mysql), mysql_error(mysql), query);
		success = false;
	}

	if (success && (res = mysql_store_result(mysql))) {
		for (bool first = true; res != NULL || mysql_next_result(mysql) == 0; first = false, res = mysql_store_result(mysql)) {
			if (res != NULL) {
				result->AddResult(res, first);
			}
		}
	}
	AddConnectionToPool(connection);

	return success;
}

string Database::Escape(const char* str, size_t len) {
	char stackBuf[4096];
	unique_ptr<char[]> oversizedBuf;
	size_t reqSize = len * 2 + 1;
	
	char* buf = stackBuf;
	if (reqSize > sizeof(stackBuf)) {
		oversizedBuf.reset(new char[reqSize]);
		buf = oversizedBuf.get();
	}

	DatabaseConnection* con = GetPooledConnection();
	unsigned long size = mysql_real_escape_string(con->mysql_con, buf, str, static_cast<unsigned long>(len));
	AddConnectionToPool(con);

	if (size == ~0ul) {
		LogError(LOG_DATABASE, 0, "Error escaping string %s!", str);
		return "";
	}

	string esc(buf, size);

	size_t pos = esc.find('%');
	if (pos != string::npos) {
		do {
			esc.insert(pos, 1, '%');
		} while (pos + 2 < esc.size() && (pos = esc.find('%', pos + 2)) != string::npos);
	}

	return esc;
}

string Database::Escape(const char* str) {
	if (str == NULL) {
		return string();
	}
	return Escape(str, strlen(str));
}

string Database::Escape(const std::string& str) {
	return Escape(str.c_str(), str.length());
}

//Since we have multiple connections now and this is a keepalive, get as many from the pool as we can
//Any connections not in the pool are in use so probably don't need a ping anyway
void Database::PingDatabase() {
	vector<DatabaseConnection*> connections;
	while (DatabaseConnection* con = GetPooledConnection(false)) {
		connections.push_back(con);
	}

	for (auto& itr : connections) {
		mysql_ping(itr->mysql_con);
		AddConnectionToPool(itr);
	}
}

//This will hold onto a single connection for this thread until a transaction is completed
thread_local DatabaseTransaction* currentTransaction = nullptr;

DatabaseConnection* Database::GetPooledConnection(bool bBlock) {
	//Each thread needs to be initialized to use MySQL safely
	class MySQLThreadInitializer {
	public:
		MySQLThreadInitializer() {
			mysql_thread_init();
		}

		~MySQLThreadInitializer() {
			mysql_thread_end();
		}
	};
	//This will be constructed the first time a thread hits this func, and destroyed when the thread ends
	thread_local MySQLThreadInitializer thread_initializer;

	DatabaseConnection* ret = nullptr;

	if (auto trans = currentTransaction) {
		ret = trans->conn;
	}
	else {
		do {
			m_connection_pool.Lock();
			if (!connection_pool.empty()) {
				ret = connection_pool.top();
				connection_pool.pop();
			}
			m_connection_pool.Unlock();
		} while (ret == nullptr && bBlock);
	}

	return ret;
}

void Database::AddConnectionToPool(DatabaseConnection* con) {
	if (!currentTransaction) {
		m_connection_pool.Lock();
		connection_pool.push(con);
		m_connection_pool.Unlock();
	}
}

DatabaseConnection::DatabaseConnection(const char* host, unsigned int port, const char* user, const char* password, const char* db) {
	//mutexing this call per SQL documentation since mysql_init will try to call mysql_library_init which is not thread safe
	static SpinLock lock;
	lock.Lock();
	//allow mysql to allocate this in case our alignment is different
	mysql_con = mysql_init(nullptr);

	lock.Unlock();

	my_bool reconnect = 1;
	mysql_options(mysql_con, MYSQL_OPT_RECONNECT, &reconnect);

	unsigned int conn_timeout = 10;
	mysql_options(mysql_con, MYSQL_OPT_CONNECT_TIMEOUT, &conn_timeout); // 10 seconds

	if (mysql_real_connect(mysql_con, host, user, password, db, port, nullptr, CLIENT_MULTI_STATEMENTS) == nullptr) {
		LogError(LOG_DATABASE, 0, "Could not connect to MySQL server (%d): %s", mysql_errno(mysql_con), mysql_error(mysql_con));
		bError = true;
	}
	else {
		bError = false;
	}
}

DatabaseConnection::~DatabaseConnection() {
	mysql_close(mysql_con);
}

QueryResult::QueryResult() :
	query_success(false),
	affected_rows(0),
	last_insert_id(0)
{}

QueryResult::QueryResult(bool res, my_ulonglong rows, my_ulonglong last_insert) :
	query_success(res),
	affected_rows(rows),
	last_insert_id(last_insert)
{}

QueryResult::~QueryResult() {
}

bool Database::QueriesFromFile(const char* file) {
	bool success = true;
	char* buf;
	int ret;
	MYSQL_RES* res;
	FILE* f;

	f = fopen(file, "rb");
	if (f == NULL) {
		LogError(LOG_DATABASE, 0, "Unable to open '%s' for reading: %s", file, strerror(errno));
		return false;
	}

	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);

	buf = (char*)malloc(size);
	if (buf == NULL) {
		fclose(f);
		LogError(LOG_DATABASE, 0, "Out of memory trying to allocate %u bytes in %s:%u\n", size, __FUNCTION__, __LINE__);
		return false;
	}

	if (fread(buf, sizeof(*buf), size, f) != size) {
		LogError(LOG_DATABASE, 0, "Failed to read from '%s': %s", file, strerror(errno));
		fclose(f);
		free(buf);
		return false;
	}

	fclose(f);

	DatabaseConnection* con = GetPooledConnection();

	ret = mysql_real_query(con->mysql_con, buf, static_cast<unsigned long>(size));
	free(buf);

	if (ret != 0) {
		LogError(LOG_DATABASE, 0, "Error running MySQL queries from file '%s' (%d): %s", file, mysql_errno(con->mysql_con), mysql_error(con->mysql_con));
		success = false;
	}
	else {
		//all results must be processed
		do {
			res = mysql_store_result(con->mysql_con);
			if (res != NULL)
				mysql_free_result(res);
			ret = mysql_next_result(con->mysql_con);

			if (ret > 0) {
				LogError(LOG_DATABASE, 0, "Error running MySQL queries from file '%s' (%d): %s", file, mysql_errno(con->mysql_con), mysql_error(con->mysql_con));
				success = false;
			}
		} while (ret == 0);
	}

	AddConnectionToPool(con);

	return success;
}

DatabaseTransaction::DatabaseTransaction(Database& in_db)
	:db(in_db) {
	//Not allowing someone to try and open 2 transactions on the same thread at once, fix if it hits this
	EmuAssert(currentTransaction == nullptr);
	conn = db.GetPooledConnection();
	currentTransaction = this;

	mysql_autocommit(conn->mysql_con, 0);
}

DatabaseTransaction::~DatabaseTransaction() {
	EmuAssert(currentTransaction == this);
	Rollback();
	mysql_autocommit(conn->mysql_con, 1);
	currentTransaction = nullptr;
	db.AddConnectionToPool(conn);
}

void DatabaseTransaction::Commit() {
	if (mysql_commit(conn->mysql_con)) {
		LogError(LOG_DATABASE, 0, "Error in mysql_commit");
	}
}

void DatabaseTransaction::Rollback() {
	if (mysql_rollback(conn->mysql_con)) {
		LogError(LOG_DATABASE, 0, "Error in mysql_rollback");
	}
}