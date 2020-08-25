#include "stdafx.h"
#include "PacketLog.h"
#include "../common/log.h"
#include "ParserDatabase.h"
#include "../common/ConfigReader.h"

ParserDatabase database;

int main() {
	srand(static_cast<unsigned int>(time(nullptr)));

	LoggingSystem::LogStart();
	LoggingSystem::LogSetPrefix("EQ2Emu-Parser");
	bool logging = true;
	std::thread logging_thread(&LoggingSystem::LogWritingThread, &logging);

	{
		ConfigReader cr(nullptr, &database, nullptr);
		cr.ReadConfig("parser-config.xml");
	}

	database.Start();

	PacketLog test("test.log");
	test.TransformPackets();

	logging = false;
	logging_thread.join();
	LoggingSystem::LogStop();

	return 0;
}