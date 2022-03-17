#include "stdafx.h"

#include "ParserZone.h"
#include "../../common/log.h"
#include "LogItemsParser.h"
#include "LogAppearancesParser.h"
#include "LogSpawnsParser.h"
#include "LogDialogsParser.h"

#include "../Packets/OP_EqUpdateMerchantCmd_Packet.h"

#include <set>
#include <thread>

ParserZone::ParserZone(int argc, char** argv) : Parser(argc, argv) {
	bool logging = true;
	std::thread logging_thread(&LoggingSystem::LogWritingThread, &logging);

	ProcessLogs();

	logging = false;
	logging_thread.join();
}

void ParserZone::ProcessLogs() {
	for (auto& opt : options) {
		if (opt == "-versions") {
			PrintLogVersions();
			return;
		}
	}

	while (std::optional<PacketLog> log = PopNextLog()) {
		log->log_id = database.CreateLogEntry(log->filename, log->logVersion);
		for (const std::string& opt : options) {
			if (opt == "-items") {
				ProcessItems(*log);
			}
			else if (opt == "-appearances") {
				ProcessAppearances(*log);
			}
			else if (opt == "-prices") {
				ProcessMerchantPrices(*log);
			}
			else if (opt == "-spawns") {
				ProcessSpawns(*log);
			}
			else if (opt == "-dialogs") {
				ProcessDialogs(*log);
			}
		}
	}

	LogInfo(LOG_PARSER, 0, "Parse completed without errors, press enter to continue");
	std::cin.get();
}

void ParserZone::ProcessItems(PacketLog& log) {
	LogItemsParser processor(log, database);
}

void ParserZone::ProcessAppearances(PacketLog& log) {
	LogAppearancesParser processor(log, database);
}

void ParserZone::ProcessMerchantPrices(PacketLog& log) {
	auto packets = log.FindPackets<OP_EqUpdateMerchantCmd_Packet>();
	LogParser lp(log, database);

	for (auto& itr : packets) {
		auto& p = itr.second;

		//Parse the buyback and sell pages
		if ((p->type & 513) == 0) continue;

		for (auto& itr : p->items) {
			DatabaseRow row;
			row.m_tableName = "raw_sell_prices";
			row.RegisterField("soe_item_id_unsigned", itr.itemID);
			row.RegisterField("price", itr.price);
			row.RegisterField("status", itr.status2);
			lp.QueueRowInsert(row);
		}
	}

	lp.DoInsertsForTable("raw_sell_prices", 250, true);
}

void ParserZone::ProcessSpawns(PacketLog& log) {
	LogSpawnsParser processor(log, database);
}

void ParserZone::ProcessDialogs(PacketLog& log) {
	LogDialogsParser processor(log, database);
}