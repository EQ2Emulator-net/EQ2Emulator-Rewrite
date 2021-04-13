#include "stdafx.h"

#include "PacketSubstruct.h"
#include "PacketArray.h"

void PacketSubstruct::PreWrite() {
	CheckElementsInitialized();
	for (auto& e : elements) {
		if (auto substr = dynamic_cast<PacketSubstructParentBase*>(e)) {
			substr->PreWrite();
		}
		if (auto arr = dynamic_cast<PacketSubstructArray*>(e)) {
			arr->PreWrite();
		}
	}
}

void PacketSubstruct::PostWrite() {
	for (auto& e : elements) {
		if (auto substr = dynamic_cast<PacketSubstructParentBase*>(e)) {
			substr->PostWrite();
		}
		if (auto arr = dynamic_cast<PacketSubstructArray*>(e)) {
			arr->PostWrite();
		}
	}
}

void PacketSubstruct::PostRead() {
	for (auto& e : elements) {
		if (auto substr = dynamic_cast<PacketSubstructParentBase*>(e)) {
			substr->PostRead();
		}
		if (auto arr = dynamic_cast<PacketSubstructArray*>(e)) {
			arr->PostRead();
		}
	}
}

void PacketSubstruct::PreRead() {
	for (auto& e : elements) {
		if (auto substr = dynamic_cast<PacketSubstructParentBase*>(e)) {
			substr->PreRead();
		}
		if (auto arr = dynamic_cast<PacketSubstructArray*>(e)) {
			arr->PreRead();
		}
	}
}