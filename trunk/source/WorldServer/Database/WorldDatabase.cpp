#include "WorldDatabase.h"
#include "../../common/log.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"

#ifdef _WIN32
	#include <WS2tcpip.h>
#else
	#include <arpa/inet.h>
#endif

static void DatabaseQueryError(Database *db) {
	LogError(LOG_DATABASE, 0, "Error running MySQL query: %s", db->GetError());
}

WorldDatabase::WorldDatabase() {

}

WorldDatabase::~WorldDatabase() {
	Disconnect();
}

bool WorldDatabase::Start() {
	DatabaseCallbacks callbacks;
	callbacks.query_error = DatabaseQueryError;
	SetCallbacks(&callbacks);

	// temp until config reader
	/*std::string host = "127.0.0.1";
	uint16_t port = 3306;
	std::string user = "rewrite";
	std::string pass = "1234";
	std::string db = "eq2_rewrite";
	SetHost(host.c_str());
	SetPort(port);
	SetUser(user.c_str());
	SetPassword(pass.c_str());
	SetDatabase(db.c_str());*/

	if (Connect()) {
		LogInfo(LOG_DATABASE, 0, "Connected to MySQL server at %s:%u", GetHost(), GetPort());
		return true;
	}

	LogError(LOG_DATABASE, 0, "Error connecting to MySQL: %s", GetError());
	return false;
}

void WorldDatabase::Stop() {
	Disconnect();
}

bool WorldDatabase::LoadOpcodes() {
	DatabaseResult result;
	bool success;
	uint32_t count = 0;

	success = Select(&result, "SELECT `name`, `version_range1`, `version_range2`, `opcode` FROM `opcodes`");
	if (!success)
		return false;

	while (result.Next()) {
		OpcodeManager::GetGlobal()->RegisterVersionOpcode(result.GetString(0), result.GetUInt16(1), result.GetUInt16(2), result.GetUInt16(3));
		count++;
	}

	LogDebug(LOG_DATABASE, 0, "Loaded %u opcodes", count);
	return true;
}

bool WorldDatabase::GetAccount(Client* client, std::string user, std::string pass) {
	DatabaseResult result;
	bool success;

	char* esc_user = Escape(user.c_str());
	char* esc_pass = Escape(pass.c_str());

	success = Select(&result, "SELECT * FROM `account` WHERE `name` = '%s' AND passwd = md5('%s')", esc_user, esc_pass);
	if (success) {
		success = result.Next();
		if (success) {
			uint32_t id = result.GetUInt32(0);
			client->SetAccount(id);
			UpdateAccountIPAddress(id, client->GetIP());
			UpdateAccountClientVersion(id, client->GetVersion());
		}
		// if user and pass check failed
		else {
			// if auto account creation is enabled
			if (true) {
				// see if there is already an account with this username
				success = Select(&result, "SELECT * FROM `account` WHERE `name` = '%s'", esc_user);
				if (success)
					success = result.Next();

				// account found which means bad password so return false
				if (success) {
					success = false;
				}
				// no account found so create one
				else {
					in_addr ip_addr;
					ip_addr.s_addr = client->GetIP();
					success = Query("INSERT INTO account(`name`, `passwd`, `ip_address`, `last_client_version`) VALUES ('%s', md5('%s'), '%s', %u)", esc_user, esc_pass, inet_ntoa(ip_addr), client->GetVersion());
					if (success) 
						client->SetAccount(LastInsertID());
				}
					
			}
			// No auto account so return false
			else {
				success = false;
			}
		}
	}

	free(esc_user);
	free(esc_pass);
	return success;
}

bool WorldDatabase::UpdateAccountIPAddress(uint32_t account, uint32_t address) {
	in_addr ip_addr;
	ip_addr.s_addr = address;
	return Query("UPDATE `account` SET `ip_address` = '%s' WHERE `id` = %u", inet_ntoa(ip_addr), account);
}

bool WorldDatabase::UpdateAccountClientVersion(uint32_t account, uint16_t version) {
	return Query("UPDATE `account` SET `last_client_version` = %u WHERE `id` = %u", version, account);
}