/*
 * YuanYang.cpp
 *
 *  Created on: Jun 17, 2015
 *      Author: chendi
 */

#include <cassert>
#include <cstdint>
#include <functional>

#include "libbase/log.h"
#include "libbase/helper.h"
#include LIBBASE_H(gpio)
#include LIBBASE_H(pin)
#include LIBBASE_H(pit)
#include LIBBASE_H(clock_utils)

#include <libsc/system.h>
#include <libsc/config.h>
#include "YuanYang.h"

using namespace LIBBASE_NS;

#if LIBSC_USE_YUANYANG

namespace
{

#if LIBSC_USE_YUANYANG == 1
inline Pin::Name GetDoPin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_YUANYANG_DO;
}

inline Pin::Name GetStatePin(const uint8_t id)
{
	if (id != 0)
	{
		assert(false);
	}
	return LIBSC_YUANYANG_STATE;
}

#else
inline Pin::Name GetDoPin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_ULTRASONIC0_DO;

	case 1:
		return LIBSC_ULTRASONIC1_DO;
	}
}

inline Pin::Name GetStatePin(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		// no break

	case 0:
		return LIBSC_ULTRASONIC0_STATE;

	case 1:
		return LIBSC_ULTRASONIC1_STATE;
	}
}

#endif //LIBSC_USE_YUANYANG

}

Pit::Config YuanYang::GetPitConfig()
{
	Pit::Config config;
	config.channel = 0;
	config.count = 0xFFFFFFFF;
	config.is_enable = false;
	return config;
}

/*inline Gpi::Config GetDoConfig(const YuanYang::Config &config, Gpi::OnGpiEventListener isr)
{
Gpi::Config config;
	config.pin = GetDoPin(id);
	config.interrupt = Pin::Config::Interrupt::kBoth;
	config.isr = isr;
	return config;
}*/

Gpi::Config YuanYang::GetDoConfig(Gpi::OnGpiEventListener isr){
	Gpi::Config config;
	config.pin = GetDoPin(0);
	config.interrupt = Pin::Config::Interrupt::kBoth;
	config.isr = isr;
	return config;
}

Gpi::Config YuanYang::GetStateConfig(){
	Gpi::Config config;
	config.pin = GetStatePin(0);
	config.interrupt = Pin::Config::Interrupt::kDisable;
	return config;
}

void YuanYang::OnUSEdge(Gpi *gpi)
{
    if (gpi->Get()){
    	pit.SetCount(0xFFFFFFFF);
    	pit.SetEnable(true);

    }
    else {
    	duration = 0xFFFFFFFF - pit.GetCountLeft();
    	pit.SetEnable(false);
    }
}

YuanYang::YuanYang(void)
:
	pit(GetPitConfig()),
	DO(nullptr), //DO(GetDoConfig(config, (std::bind(&YuanYang::OnUSEdge, this,std::placeholders::_1)))),
	State(GetStateConfig())
{
	DO=Gpi(GetDoConfig(std::bind(&YuanYang::OnUSEdge, this,std::placeholders::_1)));
	System::Init();
}

YuanYang::~YuanYang()
{}

uint32_t YuanYang::distance(){
	uint32_t result;
	result = (float)duration/ClockUtils::GetBusTickPerUs()*(340000.0f/1000000);
	return result;
}

bool YuanYang::is_valid(){
	return State.Get();
}

#endif //LIBSC_USE_YUANYANG
