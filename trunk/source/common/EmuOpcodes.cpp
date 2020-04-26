#include "stdafx.h"

#include "EmuOpcodes.h"
#include "EmuPacket.h"
#include "log.h"
#include "Packets/EmuPackets/Emu_RegisterZoneServer_Packet.h"
#include "Packets/EmuPackets/Emu_RegisterZoneServerReply_Packet.h"
#include "Packets/EmuPackets/Emu_RequestZone_Packet.h"
#include "Packets/EmuPackets/Emu_RequestZoneReply_Packet.h"
#include "Packets/EmuPackets/Emu_TransferClient_Packet.h"
#include "Packets/EmuPackets/Emu_TransferClientConfirm_Packet.h"
#include "Packets/EmuPackets/Emu_ClientLoginConfirm_Packet.h"
#include "Packets/EmuPackets/Emu_CancelClientTransfer_Packet.h"
#include "Packets/EmuPackets/Emu_ClientSessionEnded_Packet.h"
#include "Packets/EmuPackets/Emu_RequestZoneTransfer_Packet.h"
#include "Packets/EmuPackets/Emu_ZoneTransferReply_Packet.h"

class EmuPacketAllocatorBase {
public:
	virtual ~EmuPacketAllocatorBase() = default;

	virtual EmuPacket* Create() = 0;

protected:
	EmuPacketAllocatorBase() = default;
};

namespace EmuOpcode {
	std::unordered_map<EmuOpcode_t, std::unique_ptr<EmuPacketAllocatorBase> > emu_opcodes;

	EmuPacket* GetPacketForOpcode(EmuOpcode_t op) {
		auto a = emu_opcodes[op].get();

		if (!a) {
			LogError(LOG_PACKET, 0, "Received an EMU packet with an op %u that is not registered! Fix!", op);
			return nullptr;
		}

		return a->Create();
	}

	template <typename T>
	class EmuPacketAllocator : public EmuPacketAllocatorBase {
		static_assert(std::is_base_of<EmuPacket, T>::value, "Tried to create a packet constructor for a non packet type!");
	public:
		EmuPacketAllocator() = default;
		~EmuPacketAllocator() = default;

		EmuPacket* Create() override {
			return new T;
		}
	};

	void RegisterOpcode(EmuOpcode_t op, EmuPacketAllocatorBase* a) {
		auto& ptr = emu_opcodes[op];

		assert(("Emu opcode registered twice! Fix!", !ptr));

		ptr.reset(a);
	}

	template<typename T>
	class EmuOpcodeRegistrar {
		static_assert(std::is_base_of<EmuPacket, T>::value, "Tried to register an Opcode for a non packet type!");
	public:
		EmuOpcodeRegistrar(EmuOpcode_t op) {
			RegisterOpcode(op, new EmuPacketAllocator<T>);
		}
	};

#define RegisterEmuOpcode(op, t) EmuOpcodeRegistrar<t> zUNIQUENAMEz ## t (op)

	//Register emu opcodes here
	RegisterEmuOpcode(EmuOpcode_t::EMUOP_REGISTER_ZONESERVER, Emu_RegisterZoneServer_Packet);
	RegisterEmuOpcode(EmuOpcode_t::EMUOP_REGISTER_ZONESERVERREPLY, Emu_RegisterZoneServerReply_Packet);
	RegisterEmuOpcode(EmuOpcode_t::EMUOP_REQUEST_ZONE, Emu_RequestZone_Packet);
	RegisterEmuOpcode(EmuOpcode_t::EMUOP_REQUEST_ZONE_REPLY, Emu_RequestZoneReply_Packet);
	RegisterEmuOpcode(EmuOpcode_t::EMUOP_TRANSFER_CLIENT, Emu_TransferClient_Packet);
	RegisterEmuOpcode(EmuOpcode_t::EMUOP_TRANSFER_CLIENT_CONFIRM, Emu_TransferClientConfirm_Packet);
	RegisterEmuOpcode(EmuOpcode_t::EMUOP_CLIENT_LOGIN_CONFIRM, Emu_ClientLoginConfirm_Packet);
	RegisterEmuOpcode(EmuOpcode_t::EMUOP_CANCEL_CLIENT_TRANSFER, Emu_CancelClientTransfer_Packet);
	RegisterEmuOpcode(EmuOpcode_t::EMUOP_CLIENT_SESSION_ENDED, Emu_ClientSessionEnded_Packet);
	RegisterEmuOpcode(EmuOpcode_t::EMUOP_REQUEST_ZONE_TRANSFER, Emu_RequestZoneTransfer_Packet);
	RegisterEmuOpcode(EmuOpcode_t::EMUOP_ZONE_TRANSFER_REPLY, Emu_ZoneTransferReply_Packet);
}
