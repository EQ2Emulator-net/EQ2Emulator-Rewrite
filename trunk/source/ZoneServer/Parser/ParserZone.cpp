#include "stdafx.h"

#include "ParserZone.h"
#include "../../common/log.h"
#include "../../common/Parser/PacketLog.h"
#include "../Packets/ItemPackets.h"
#include <set>

ParserZone::ParserZone(int argc, char** argv) : Parser(argc, argv) {
	bool logging = true;
	std::thread logging_thread(&LoggingSystem::LogWritingThread, &logging);

	ProcessLogs();

	logging = false;
	logging_thread.join();
}

void ParserZone::ProcessLogs() {
	while (std::optional<PacketLog> log = PopNextLog()) {
		for (const std::string& opt : options) {
			if (opt == "-items") {
				ProcessItems(*log);
			}
		}
	}
}

void ParserZone::ProcessItems(PacketLog& log) {
	LogItemsParser processor(log, database);
}

LogItemsParser::LogItemsParser(PacketLog& log, ParserDatabase& db) {
	auto packets = log.FindPackets<OP_EqExamineInfoCmd_Packet, ExamineInfoCmd_Item_Packet>();

	LogDebug(LOG_PARSER, 0, "Item count for log %s : %llu", log.filename.c_str(), packets.size());
	for (auto& itr : packets) {
	}
}