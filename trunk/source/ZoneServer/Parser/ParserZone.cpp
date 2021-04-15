#include "stdafx.h"

#include "ParserZone.h"
#include "../../common/log.h"
#include "LogItemsParser.h"
#include "LogAppearancesParser.h"

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
	while (std::optional<PacketLog> log = PopNextLog()) {
		log->log_id = database.CreateLogEntry(log->filename, log->logVersion);
		for (const std::string& opt : options) {
			if (opt == "-items") {
				ProcessItems(*log);
			}
			else if (opt == "-appearances") {
				ProcessAppearances(*log);
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

	for (auto& itr : packets) {
		auto& p = itr.second;
	}
}