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
#include "MySmartCar.h"
#include <libbase/k60/mcg.h>
#include <libsc/led.h>
#include <libsc/us_100.h>
#include <libsc/system.h>
#include <libsc/st7735r.h>
#include <libsc/lcd_console.h>

#include "YuanYang.h"


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


using namespace libsc;
using namespace libbase::k60;

MySmartCar myCar;
//Us100 us({0});
Led led({0});
YuanYang us;
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


int main(void)
{
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
	while (true)
	{
		//myCar.m_lcdConsole.setRow(0) << us.GetDistanceyuan() << "   ";
//		console.WriteBuffer(buf, n);


		//uint32_t freq=ClockUtils::GetBusTickPerUs();
		System::DelayMs(300);
		if (us.is_valid()){
			myCar.m_lcdConsole.setRow(0)<<us.distance()<<"    ";
			//myCar.m_lcdConsole.setRow(0)<<freq<<"  ";
		}
		else{
			myCar.m_lcdConsole.setRow(0)<<"---"<<"    ";
		}


	}

	//us.Stop();

//	myCar.m_loop.start();
	return 0;

}
