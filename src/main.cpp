/*
 * main.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstring>
#include <libbase/k60/mcg.h>
#include <libbase/k60/gpio.h>
#include <libbase/k60/clock_utils.h>
#include <libsc/system.h>
#include "MySmartCar.h"
#include <libbase/k60/mcg.h>
#include <libsc/led.h>
#include <libsc/us_100.h>
#include <libsc/system.h>
#include <libsc/st7735r.h>
#include <libsc/lcd_console.h>

#include "ultrasonic.h"


namespace libbase
{
	namespace k60
	{

		Mcg::Config Mcg::GetMcgConfig()
		{
			Mcg::Config config;
			config.external_oscillator_khz = 50000;
			config.core_clock_khz = 150000;
			return config;
		}

	}
}

namespace
{

Pit::Config GetPitConfig()
{
	Pit::Config config;
	config.channel = 0;
	config.count = 0xFFFFFFFF;
	config.is_enable = false;
	return config;
}

}

using namespace libsc;
using namespace libbase::k60;

MySmartCar myCar;
//Us100 us({0});
Led led({0});
Pit pit(GetPitConfig());

volatile uint32_t current_time;
volatile uint32_t PulseWidth;
volatile uint32_t duration;

//
//Gpi::Config GetTestConfig(Gpi::OnGpiEventListener isr)
//
//	Gpi::Config product;
//	product.pin = Pin::Name::kPtc13;
//	product.interrupt = Pin::Config::Interrupt::kRising;
//	product.isr = isr;
//	return product;
//}
//
//
//void Test(Gpi *gpi)
//{
//    led.Switch();
//    System::DelayMs(300);
//
//
//}

void OnUSEdge(Gpi *gpi)
{
    if (gpi->Get()){
    	//current_time=System::Time();
    	pit.SetCount(0xFFFFFFFF);
    	pit.SetEnable(true);

    }
    else {
    	duration = 0xFFFFFFFF - pit.GetCountLeft();
    	pit.SetEnable(false);
    	//PulseWidth=System::Time()-current_time;
    }
}

int main(void)
{
	uint32_t distance;
	System::Init();

//	St7735r::Config lcdConfig;
//	lcdConfig.fps = 60;
//	lcdConfig.is_bgr = true;
//	lcdConfig.is_revert = false;

//	LcdConsole::Config consoleConfig;
//	consoleConfig.bg_color = 0;
//	consoleConfig.lcd = new St7735r(lcdConfig);
//	consoleConfig.text_color = -1;


//	LcdConsole console(consoleConfig);


	//us.Start();
   //Gpi(GetTestConfig(std::bind(&Test,placeholders::_1)));
	Gpi us(GetUSConfig());
	while (true)
	{
		//myCar.m_lcdConsole.setRow(0) << us.GetDistanceyuan() << "   ";
//		console.WriteBuffer(buf, n);
		//distance=PulseWidth*34;
		ClockUtils::GetBusClockMhz();
		distance=duration/50*34000/1000000;
		myCar.m_lcdConsole.setRow(0)<<distance<<"  "<<'\n';
		System::DelayMs(500);


	}

	//us.Stop();

//	myCar.m_loop.start();
	return 0;

}
