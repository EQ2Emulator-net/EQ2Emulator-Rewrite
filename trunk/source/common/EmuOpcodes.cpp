#include "stdafx.h"

#include "EmuOpcodes.h"
#include "EmuPacket.h"
#include "log.h"
#include "Packets/EmuPackets/Emu_RegisterZoneServer_Packet.h"

class EmuPacketAllocatorBase {
public:
	virtual ~EmuPacketAllocatorBase() = default;

	virtual EmuPacket* Create() = 0;

protected:
	EmuPacketAllocatorBase() = default;
};

namespace EmuOpcode {
	std::unordered_map<uint8_t, std::unique_ptr<EmuPacketAllocatorBase> > emu_opcodes;

	EmuPacket* GetPacketForOpcode(uint8_t op) {
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

#define RegisterEmuOpcode(op, t) EmuOpcodeRegistrar<t> zUNIQUENAMEz ## t ## (op)

	//Register emu opcodes here
	RegisterEmuOpcode(EMUOP_REGISTER_ZONESERVER, Emu_RegisterZoneServer_Packet);
}
