#pragma once

#include "database-result.h"
#include "Mutex.h"
#include <stack>
#include <new>

class DatabaseConnection;
class DatabaseTransaction;

class QueryResult {
public:
	QueryResult();
	QueryResult(bool res, my_ulonglong rows, my_ulonglong last_insert_id);
	~QueryResult();

	bool query_success;
	my_ulonglong affected_rows;
	my_ulonglong last_insert_id;

	operator bool() const {
		return query_success;
	}
};

class Database {
public:
	Database();
	~Database();

	void SetHost(const char* host);
	void SetPort(unsigned int port);
	void SetUser(const char* user);
	void SetPassword(const char* password);
	void SetDatabase(const char* db);

	const char* GetHost();
	unsigned int GetPort();
	const char* GetDatabase();

	bool ValidateConfig();

	bool Connect();

	DatabaseConnection* OpenNewConnection();

	void PingDatabase();

	bool Query(const char* fmt, ...);

#define QUERY_RESULT_FLAG_AFFECTED_ROWS 1
#define	QUERY_RESULT_FLAG_LAST_INSERT_ID 2
	QueryResult QueryWithFetchedResult(uint32_t result_flags, const char* fmt, ...);

	bool Select(DatabaseResult* result, const char* fmt, ...);

	std::string Escape(const char* str, size_t len);
	std::string Escape(const char* str);
	std::string Escape(const char* str, bool with_percent);
	std::string Escape(const std::string& str);

	// For PatchServer
	bool QueriesFromFile(const char* file);

private:
	std::stack<DatabaseConnection*> connection_pool;
	SpinLock m_connection_pool;
	std::string host;
	unsigned int port;
	std::string user;
	std::string password;
	std::string db;

	friend class DatabaseTransaction;
	DatabaseConnection* GetPooledConnection(bool bBlock = true);
	void AddConnectionToPool(DatabaseConnection* con);
};

class DatabaseConnection {
public:
	DatabaseConnection(const char* host, unsigned int port, const char* user, const char* password, const char* db);
	~DatabaseConnection();
	MYSQL* mysql_con;
	bool bError;
};

//This allows a SINGLE thread to run multiple queries in a transaction
//When the DatabaseTransaction is destroyed it will auto rollback 
//You must commit before destruction if a commit is desired.
//This class is stack allocation only, it should not be passed around to different threads so please don't
//As stated, this will work for one thread only even if you did.
class DatabaseTransaction {
	friend class Database;

public:
	DatabaseTransaction(Database& db);
	~DatabaseTransaction();

	void Rollback();
	void Commit();

	static void* operator new(size_t) = delete;
	static void* operator new[](size_t) = delete;
	static void  operator delete(void*) = delete;
	static void  operator delete[](void*) = delete;
	DatabaseTransaction& operator=(const DatabaseTransaction&) = delete;
	DatabaseTransaction(const DatabaseTransaction&) = delete;

private:
	DatabaseConnection* conn;
	Database& db;
};