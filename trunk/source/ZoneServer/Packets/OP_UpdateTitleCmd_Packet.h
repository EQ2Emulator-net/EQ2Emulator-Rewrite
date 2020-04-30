#pragma once

#include "../../common/Packets/EQ2Packet.h"
#include "../../common/Packets/EQ2Packets/OpcodeManager.h"
#include "../../common/Packets/PacketElements/PacketElements.h"

class OP_UpdateTitleCmd_Packet : public EQ2Packet {
public:
    OP_UpdateTitleCmd_Packet(uint32_t version) : EQ2Packet(version) {
        unknown1 = 0;
        isPlayer = false;
        RegisterElements();
    }

    void RegisterElements() {
        RegisterUInt32(spawn_id);
        Register16String(name);
        RegisterUInt8(unknown1);
        RegisterBool(isPlayer);
        Register16String(last_name);
        Register16String(suffix_title);
        Register16String(prefix_title);
        Register16String(pvp_title);
        if (GetVersion() < 1153)
            Register16String(unknown4);
        Register16String(guild_title);
    }

    uint32_t spawn_id;
    std::string name;
    uint8_t unknown1;
    bool isPlayer;
    std::string last_name;
    std::string suffix_title;
    std::string prefix_title;
    std::string pvp_title;    
    std::string unknown4;
    std::string guild_title;
};
/*
<Struct Name="WS_UpdateTitle" ClientVersion="1" OpcodeName="OP_UpdateTitleCmd">
  <Data ElementName="player_id" Type="int32" Size="1" />
  <Data ElementName="player_name" Type="EQ2_16Bit_String" />
  <Data ElementName="unknown1" Type="int8" Size="2" />
  <Data ElementName="unknown2" Type="int16" Size="1" />
  <Data ElementName="suffix_title" Type="EQ2_16Bit_String" />
  <Data ElementName="unknown3" Type="int16" Size="1" />
  <Data ElementName="prefix_title" Type="EQ2_16Bit_String" />
  <Data ElementName="unknown4" Type="int16" Size="1" />
  <Data ElementName="sub_title" Type="EQ2_16Bit_String" />
</Struct>
<Struct Name="WS_UpdateTitle" ClientVersion="1153" OpcodeName="OP_UpdateTitleCmd">
  <Data ElementName="player_id" Type="int32" Size="1" />
  <Data ElementName="player_name" Type="EQ2_16Bit_String" />
  <Data ElementName="unknown1" Type="int8" Size="2" />
  <Data ElementName="last_name" Type="EQ2_16Bit_String" Size="1" />
  <Data ElementName="suffix_title" Type="EQ2_16Bit_String" />
  <Data ElementName="unknown3" Type="int16" Size="1" />
  <Data ElementName="prefix_title" Type="EQ2_16Bit_String" />
  <Data ElementName="sub_title" Type="EQ2_16Bit_String" />
</Struct>
*/