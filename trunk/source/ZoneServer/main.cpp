#include "stdafx.h"

#include "../common/ConfigReader.h"
#include "../common/util.h"
#include "WorldTalk/WorldTalk.h"


//REMOVE
#include "../WorldServer/Database/WorldDatabase.h"
#include "../common/Classes.h"

WorldDatabase database;
Classes classes;

int main() {
	bool looping = true;

	WorldTalk talk;

	ConfigReader cr(nullptr, nullptr, &talk);
	cr.ReadConfig("zone-config.xml");

	talk.Open();

	while (looping) {
		talk.Process();

		SleepMS(5);
	}

	return 0;
}