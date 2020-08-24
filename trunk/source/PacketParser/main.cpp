#include "stdafx.h"
#include "PacketLog.h"

int main() {
	//std::cin.get();
	PacketLog test("test.log");
	test.TransformPackets();
	return 0;
}