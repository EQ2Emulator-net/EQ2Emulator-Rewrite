#include "stdafx.h"

#include "Sign.h"

Sign::Sign():
m_distance(0.f),
m_zoneID(0),
m_zoneX(0.f),
m_zoneY(0.f),
m_zoneZ(0.f),
m_zoneHeading(0.f),
m_signType(ESignType::ESIGN_GENERIC) 
{

}

Sign::~Sign() {

}