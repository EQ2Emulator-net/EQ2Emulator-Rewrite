#include "stdafx.h"

#include "EQ2Packet.h"
#include "../util.h"
#include "../Packets/EQ2Packets/OpcodeManager.h"
#include "../../common/Packets/PacketElements/PacketSubstruct.h"

EQ2Packet::EQ2Packet(uint32_t version) {
	app_opcode_size = 2;
	Version = version;
	bOpcodeError = false;
	PacketEncrypted = false;
	EQ2Compressed = false;
	PacketPrepared = false;
	bCombined = false;
	opcode = 0;
}

uint8_t EQ2Packet::PreparePacket(uint16_t MaxLen) {
	// Set the opcode for this packet based on version
	PacketPrepared = true;
	
	uint16_t login_opcode = opcode;
	uint8_t offset = 0;
	//one of the int16s is for the seq, other is for the EQ2 opcode and compressed flag (OP_Packet is the header, not the opcode)
	uint32_t new_size = Size + sizeof(uint16_t) + sizeof(uint8_t);
	bool oversized = false;
	if (login_opcode != 2) {
		new_size += sizeof(uint8_t); //for opcode
		if (login_opcode >= 255) {
			new_size += sizeof(uint16_t);
			oversized = true;
		}
		else
			login_opcode = ntohs(login_opcode);
	}
	unsigned char* new_buffer = new unsigned char[new_size];
	memset(new_buffer, 0, new_size);
	unsigned char* ptr = new_buffer + sizeof(uint16_t); // sequence is first
	if (login_opcode != 2) {
		if (oversized) {
			ptr += sizeof(uint8_t); //compressed flag
			uint8_t addon = 0xff;
			memcpy(ptr, &addon, sizeof(uint8_t));
			ptr += sizeof(uint8_t);
		}
		memcpy(ptr, &login_opcode, sizeof(uint16_t));
		ptr += sizeof(uint16_t);
	}
	else {
		memcpy(ptr, &login_opcode, sizeof(uint8_t));
		ptr += sizeof(uint8_t);
	}
	memcpy(ptr, buffer, Size);
	if (buffer)
		delete[] buffer;
	buffer = new_buffer;
	offset = new_size - Size - 1;
	Size = new_size;
	return offset;
}

uint32_t EQ2Packet::serialize(unsigned char *dest) const {
	if (app_opcode_size == 1)
		*(unsigned char *)dest = (uint8_t)opcode;
	else
		*(uint16_t *)dest = opcode;

	memcpy(dest + app_opcode_size, buffer, Size);

	return Size + app_opcode_size;
}

void EQ2Packet::FindOpcode() {
	bOpcodeError = !OpcodeManager::GetGlobal()->SetOpcodeForPacket(this);
}

void EQ2Packet::HandlePacket(std::shared_ptr<Client> client) {
	LogWarn(LOG_PACKET, 0, "Unhandled packet, opcode = %u", GetOpcode());
	DumpBytes(buffer, Size);
}

//This function makes the assumption that the rhs is not combined!
bool EQ2Packet::TryCombine(EQ2Packet* rhs, uint32_t MaxLength) {
	//Account for the 0x00 0x19 bytes if we need to add those, skip the sequence bytes we have as well
	uint16_t combinedAdd = bCombined ? 0 : (2 + (Size - 2 >= 255 ? 3 : 1));
	uint16_t addSize = rhs->Size - 2;
	uint32_t newSize = Size + combinedAdd + addSize + (addSize >= 255 ? 3 : 1);

	//We have 2 bytes at the beginning for sequence which we used to set earlier, remove it eventually
	if (newSize - 2 > MaxLength) {
		return false;
	}

	//We can combine these packets
	unsigned char* combBuf = new unsigned char[newSize];
	uint32_t offset = 0;
	if (!bCombined) {
		bCombined = true;
		combBuf[2] = 0x00;
		combBuf[3] = 0x19;

		offset = 4;
		uint16_t myDataSize = static_cast<uint16_t>(Size - 2);
		if (myDataSize >= 255) {
			combBuf[offset++] = 0xFF;
			*reinterpret_cast<uint16_t*>(combBuf + offset) = htons(myDataSize);
			offset += 2;
		}
		else {
			combBuf[offset++] = static_cast<uint8_t>(myDataSize);
		}
		
		//Copy this packet's data minus protocol
		memcpy(combBuf + offset, buffer + 2, myDataSize);
		offset += myDataSize;
	}
	else {
		//We already have the 0x19, sizes for already added packets set up
		memcpy(combBuf, buffer, Size);
		offset = Size;
	}

	//Add the new packet's size
	if (addSize >= 255) {
		combBuf[offset++] = 0xFF;
		*reinterpret_cast<uint16_t*>(combBuf + offset) = htons(static_cast<uint16_t>(addSize));
		offset += 2;
	}
	else {
		combBuf[offset++] = static_cast<uint8_t>(addSize);
	}

	//Copy the new packet's data
	memcpy(combBuf + offset, rhs->buffer + 2, addSize);

	if (NetDebugEnabled()) {
		LogError(LOG_PACKET, 0, "Combined app packet");
		DumpBytes(combBuf, newSize);
	}

	//Cleanup our old buffer
	delete[] buffer;
	buffer = combBuf;
	Size = newSize;
	return true;
}

EQ2Packet* EQ2Packet::CopyRaw() {
	EQ2Packet* ret = new EQ2Packet(0);
	ret->Size = Size;
	ret->buffer = new unsigned char[Size];
	memcpy(ret->buffer, buffer, Size);
	ret->bCombined = bCombined;
	ret->bOpcodeError = bOpcodeError;
	ret->EQ2Compressed = EQ2Compressed;
	ret->PacketEncrypted = PacketEncrypted;
	ret->PacketPrepared = PacketPrepared;
	ret->Version = Version;
	ret->opcode = opcode;
	return ret;
}

void EQ2Packet::PreWrite() {
	for (auto& e : elements) {
		if (auto substr = dynamic_cast<PacketSubstruct*>(e)) {
			substr->PreWrite();
		}
	}
}

void EQ2Packet::PostWrite() {
	for (auto& e : elements) {
		if (auto substr = dynamic_cast<PacketSubstruct*>(e)) {
			substr->PostWrite();
		}
	}
}

void EQ2Packet::PostRead() {
	for (auto& e : elements) {
		if (auto substr = dynamic_cast<PacketSubstruct*>(e)) {
			substr->PostRead();
		}
	}
}