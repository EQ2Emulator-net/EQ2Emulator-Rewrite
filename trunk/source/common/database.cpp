#include "stdafx.h"

#include "database.h"

//Might want to move this "string.h" stuff somewhere else/rename... little confusing
#include "string.h"
//Again... not great to be using stdlib names
#include "stdio.h"

#include <string>

// fix for incompatible mysqlclient.lib
// http://stackoverflow.com/questions/30450042/unresolved-external-symbol-imp-iob-func-referenced-in-function-openssldie
#if defined(_WIN32)
FILE _iob[] = { *stdin, *stdout, *stderr };
extern "C" FILE * __cdecl __iob_func(void) { return _iob; }
#endif

Database::Database() {
	mysql_init(&mysql);
    connected = false;
    memset(&callbacks, 0, sizeof(callbacks));
    host[0] = '\0';
	port = 3306;
    user[0] = '\0';
    password[0] = '\0';
    db[0] = '\0';
    error[0] = '\0';
}

Database::~Database() {
    Disconnect();
}

void Database::SetCallbacks(DatabaseCallbacks *callbacks) {
    memcpy(&this->callbacks, callbacks, sizeof(this->callbacks));
}

void Database::SetHost(const char *host) {
    if (host == NULL)
        this->host[0] = '\0';
    else
	    strlcpy(this->host, host, sizeof(this->host));
}

void Database::SetPort(unsigned int port) {
	this->port = port;
}

void Database::SetUser(const char *user) {
	strlcpy(this->user, user, sizeof(this->user));
}

void Database::SetPassword(const char *password) {
	strlcpy(this->password, password, sizeof(this->password));
}

void Database::SetDatabase(const char *db) {
	strlcpy(this->db, db, sizeof(this->db));
}

const char * Database::GetHost() {
	return host;
}

unsigned int Database::GetPort() {
	return port;
}

unsigned int Database::GetErrno() {
    return mysql_errno(&mysql);
}

const char * Database::GetError() {
    return error;
}

void Database::SetIgnoredErrno(unsigned int db_errno) {
    std::vector<unsigned int>::iterator itr;

    for (itr = ignored_errnos.begin(); itr != ignored_errnos.end(); itr++) {
        if ((*itr) == db_errno)
            return;
    }

    ignored_errnos.push_back(db_errno);
}

void Database::RemoveIgnoredErrno(unsigned int db_errno) {
	std::vector<unsigned int>::iterator itr;

    for (itr = ignored_errnos.begin(); itr != ignored_errnos.end(); itr++) {
        if ((*itr) == db_errno) {
            ignored_errnos.erase(itr);
            break;
        }
    }
}

bool Database::IsIgnoredErrno(unsigned int db_errno) {
	std::vector<unsigned int>::iterator itr;

    for (itr = ignored_errnos.begin(); itr != ignored_errnos.end(); itr++) {
        if ((*itr) == db_errno)
            return true;
    }

    return false;
}

bool Database::Connect() {
    my_bool reconnect = true;
    unsigned int timeout = 5;

    if (!connected) {
        mysql_options(&mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
        mysql_options(&mysql, MYSQL_OPT_RECONNECT, &reconnect);

	    connected = mysql_real_connect(&mysql, host, user, password, db, port, NULL, 0) != NULL;
        if (!connected)
		    snprintf(error, sizeof(error), "Could not connect to MySQL server (%d): %s", mysql_errno(&mysql), mysql_error(&mysql));
	}

	return connected;
}

void Database::Disconnect() {
    if (connected) {
        mysql_close(&mysql);
        connected = false;
    }
}

bool Database::Query(const char *fmt, ...) {
	bool success = true;
	char *query;
	int count;
	va_list ap;

	va_start(ap, fmt);
	count = vasprintf(&query, fmt, ap);
	va_end(ap);

	if (count == -1) {
		strlcpy(error, "Out of memory", sizeof(error));
		return false;
	}

	if (mysql_real_query(&mysql, query, (unsigned long)count) != 0) {
        if (!IsIgnoredErrno(mysql_errno(&mysql))) {
		    snprintf(error, sizeof(error), "Error running MySQL query (%d): %s\n%s", mysql_errno(&mysql), mysql_error(&mysql), query);
		    success = false;
            if (callbacks.query_error)
                callbacks.query_error(this);
        }
	}

	free(query);
	return success;
}

bool Database::Select(DatabaseResult *result, const char *fmt, ...) {
	bool success = true;
	char *query;
	int count;
	va_list ap;
	MYSQL_RES *res;

	va_start(ap, fmt);
	count = vasprintf(&query, fmt, ap);
	va_end(ap);

	if (count == -1) {
		strlcpy(error, "Out of memory", sizeof(error));
		return false;
	}

	if (mysql_real_query(&mysql, query, (unsigned long)count) != 0) {
        if (!IsIgnoredErrno(mysql_errno(&mysql))) {
		    snprintf(error, sizeof(error), "Error running MySQL query (%d): %s\n%s", mysql_errno(&mysql), mysql_error(&mysql), query);
		    success = false;
            if (callbacks.query_error)
                callbacks.query_error(this);
        }
	}

	if (success && !IsIgnoredErrno(mysql_errno(&mysql))) {
		res = mysql_store_result(&mysql);

        if (res != NULL)
			result->SetResult(res);
		else {
			snprintf(error, sizeof(error), "Error storing MySql query result (%d): %s\n%s", mysql_errno(&mysql), mysql_error(&mysql), query);
			success = false;
            if (callbacks.query_error)
                callbacks.query_error(this);
		}
	}

	free(query);
	return success;
}

bool Database::QueriesFromFile(const char *file) {
    bool success = true;
    long size;
    char *buf;
    int ret;
    MYSQL_RES *res;
    FILE *f;

    f = fopen(file, "rb");
    if (f == NULL) {
        snprintf(error, sizeof(error), "Unable to open '%s' for reading: %s", file, strerror(errno));
        return false;
    }

    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);

    buf = (char *)malloc(size + 1);
    if (buf == NULL) {
        fclose(f);
        strlcpy(error, "Out of memory", sizeof(error));
        return false;
    }

    if (fread(buf, sizeof(*buf), size, f) != (size_t)size) {
        snprintf(error, sizeof(error), "Failed to read from '%s': %s", file, strerror(errno));
        fclose(f);
        return false;
    }

    buf[size] = '\0';
    fclose(f);

    mysql_set_server_option(&mysql, MYSQL_OPTION_MULTI_STATEMENTS_ON);
    ret = mysql_real_query(&mysql, buf, size);
    free(buf);

    if (ret != 0) {
        snprintf(error, sizeof(error), "Error running MySQL queries from file '%s' (%d): %s", file, mysql_errno(&mysql), mysql_error(&mysql));
		success = false;
        if (callbacks.query_error)
            callbacks.query_error(this);
    }
    else {
        //all results must be processed
        do {
            res = mysql_store_result(&mysql);
            if (res != NULL)
                mysql_free_result(res);
            ret = mysql_next_result(&mysql);

            if (ret > 0) {
                snprintf(error, sizeof(error), "Error running MySQL queries from file '%s' (%d): %s", file, mysql_errno(&mysql), mysql_error(&mysql));
                success = false;
                if (callbacks.query_error)
                    callbacks.query_error(this);
            }
                
        }
        while (ret == 0);
    }
    mysql_set_server_option(&mysql, MYSQL_OPTION_MULTI_STATEMENTS_OFF);

    return success;
}

unsigned long Database::LastInsertID() {
	return (unsigned long)mysql_insert_id(&mysql);
}

unsigned long Database::AffectedRows() {
	return (unsigned long)mysql_affected_rows(&mysql);
}

std::string Database::Escape(const char *str, size_t len) {
	std::vector<char> buf(len * 2 + 1);

	mysql_real_escape_string(&mysql, buf.data(), str, (unsigned long)len);

	return std::move(std::string(buf.data()));
}

std::string Database::Escape(const char *str) {
	return std::move(Escape(str, strlen(str)));
}

std::string Database::Escape(const std::string& str) {
	return std::move(Escape(str.c_str(), str.length()));
}

void Database::BeginTransaction() {
    mysql_autocommit(&mysql, 0);
//    mysql_real_query(&mysql, "START TRANSACTION", 17);
}

void Database::CommitTransaction() {
    mysql_commit(&mysql);
    mysql_autocommit(&mysql, 1);
//    mysql_real_query(&mysql, "COMMIT", 6);
}

void Database::RollbackTransaction() {
    mysql_rollback(&mysql);
    mysql_autocommit(&mysql, 1);
//    mysql_real_query(&mysql, "ROLLBACK", 8);
}

void Database::PingDatabase() {
	mysql_ping(&mysql);
}
