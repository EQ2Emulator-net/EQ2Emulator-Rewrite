#include "stdafx.h"
#include "PacketLog.h"
#include "../common/log.h"
#include "ParserDatabase.h"
#include "../common/ConfigReader.h"
#include "../ZoneServer/Packets/OP_EqExamineInfoCmd.h"

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

	PacketLog test("BoL_Questline_DeepChelsithSolo.log");
	test.TransformPackets();

	OP_EqExamineInfoCmd_Packet p(test.logVersion);
	p.FindOpcode();

	std::cout << p.GetSubOpcode();

	logging = false;
	logging_thread.join();
	LoggingSystem::LogStop();

	return 0;
}