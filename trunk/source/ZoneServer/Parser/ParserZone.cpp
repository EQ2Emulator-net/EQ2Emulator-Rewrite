#include "stdafx.h"

#include "ParserZone.h"
#include "../../common/log.h"
#include "../../common/Parser/PacketLog.h"

ParserZone::ParserZone(int argc, char** argv) : Parser(argc, argv) {
	bool logging = true;
	std::thread logging_thread(&LoggingSystem::LogWritingThread, &logging);

	ProcessLogs();

	logging = false;
	logging_thread.join();
}

void ParserZone::ProcessLogs() {

}