#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

class OP_GameWorldTimeMsg_Packet : public EQ2Packet {
public:
	OP_GameWorldTimeMsg_Packet(uint32_t version)
		: EQ2Packet(version) {
		RegisterElements();

		year = 0;
		month = 0;
		day = 0;
		hour = 0;
		minute = 0;
		unknown = 0;
		unix_time = 0;
		unknown2 = 0;
	}

	uint16_t year;
	uint8_t month;
	uint8_t day;
	uint8_t hour;
	uint8_t minute;
	uint8_t unknown;
	uint32_t unix_time;
	float unknown2;

private:
	void RegisterElements() {
		RegisterUInt16(year);
		RegisterUInt8(month);
		RegisterUInt8(day);
		RegisterUInt8(hour);
		RegisterUInt8(minute);
		RegisterUInt8(unknown);
		RegisterUInt32(unix_time);
		if (GetVersion() >= 1193)
			RegisterFloat(unknown2);
	}

};

/*
<Struct Name="WS_GameWorldTime" ClientVersion="1" OpcodeName="OP_GameWorldTimeMsg">
<Data ElementName="year" Type="int16" Size="1" />
<Data ElementName="month" Type="int8" Size="1" />
<Data ElementName="day" Type="int8" Size="1" />
<Data ElementName="hour" Type="int8" Size="1" />
<Data ElementName="minute" Type="int8" Size="1" />
<Data ElementName="unknown" Type="int8" Size="1" />
<Data ElementName="unix_time" Type="int32" Size="1" />
</Struct>

<Struct Name="WS_GameWorldTime" ClientVersion="1193" OpcodeName="OP_GameWorldTimeMsg">
<Data ElementName="year" Type="int16" Size="1" />
<Data ElementName="month" Type="int8" Size="1" />
<Data ElementName="day" Type="int8" Size="1" />
<Data ElementName="hour" Type="int8" Size="1" />
<Data ElementName="minute" Type="int8" Size="1" />
<Data ElementName="unknown" Type="int8" Size="1" />
<Data ElementName="unix_time" Type="int32" Size="1" />
<Data ElementName="unknown2" Type="float" Size="1" />
</Struct>
*/