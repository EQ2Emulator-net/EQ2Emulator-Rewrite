#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

//LIST OF STATES, these map to the same bitflags in the position and player movement structs

//PositionState1
// 1<<0 flymode
// 1<<1 nocollision flymode
// 1<<2 unknown
// 1<<3 turn movement only
// 1<<4 no turning
// 1<<5 glide
// 1<<6 sit
// 1<<8 crouch
// 1<<10 falling
// 1<<18 float when jumping, no movement
// 1<<19 high jump, no movement
// 1<<23 walk underwater
// 1<<24 moon jump underwater
// 1<<26 fear
// 1<<28 moon jump
// 1<<29 safe fall (float to ground)
// 1<<30 cannot move

//PositionState2
// 1<<0 dead?
// 1<<3 hover (fae)
// 1<<5 mount flymode
// 1<<6 no-fly zone

class OP_ChangeServerControlFlagMsg_Packet : public EQ2Packet {
public:
	OP_ChangeServerControlFlagMsg_Packet(uint32_t version) : EQ2Packet(version),
	positionState(0), positionState2(0), value(false) {
		RegisterElements();
	}

	void RegisterElements() {
		RegisterUInt32(positionState);
		RegisterUInt32(positionState2);
		RegisterBool(value);
	}

	uint32_t positionState;
	uint32_t positionState2;
	bool value;	
};