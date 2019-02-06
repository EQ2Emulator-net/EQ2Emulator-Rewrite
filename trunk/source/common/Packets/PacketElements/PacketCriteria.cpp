#include "stdafx.h"

#include "PacketCriteria.h"
#include "../PacketElement.h"

bool PacketVariableEquality::CheckVariableEquality() {
	//Check if we are set, if so check with the other element if they meet our equality check
	if (check_element) {
		return check_element->MeetsCriteria() && check_element->CheckVariableEquality(this);
	}

	return true;
}