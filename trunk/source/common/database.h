#pragma once

#include <vector>
#include "database-result.h"

class Database;

typedef void(*DatabaseQueryErrorCallback)(Database *database);

typedef struct {
    DatabaseQueryErrorCallback query_error;
} DatabaseCallbacks;

class Database {
public:
	Database();
	virtual ~Database();

    void SetCallbacks(DatabaseCallbacks *callbacks);
	void SetHost(const char *host);
	void SetPort(unsigned int port);
	void SetUser(const char *user);
	void SetPassword(const char *password);
	void SetDatabase(const char *db);

	const char * GetHost();
	unsigned int GetPort();

    unsigned int GetErrno();
	const char * GetError();

    void SetIgnoredErrno(unsigned int db_errno);
    void RemoveIgnoredErrno(unsigned int db_errno);
    bool IsIgnoredErrno(unsigned int db_errno);

	bool Connect();
    void Disconnect();

	bool Query(const char *fmt, ...);
	bool Select(DatabaseResult *result, const char *fmt, ...);
    bool QueriesFromFile(const char *file);

	unsigned long LastInsertID();
	unsigned long AffectedRows();

	char * Escape(const char *str, size_t len);
	char * Escape(const char *str);

    void BeginTransaction();
    void CommitTransaction();
    void RollbackTransaction();

    void PingDatabase();

private:
	MYSQL mysql;
    bool connected;
    DatabaseCallbacks callbacks;
	char host[64];
	unsigned int port;
	char user[16];
	char password[64];
	char db[64];
    char error[256];
    std::vector<unsigned int> ignored_errnos;
};
