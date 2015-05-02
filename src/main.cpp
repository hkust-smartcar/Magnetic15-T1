/*
 * main.cpp
 *
 *  Created on: Apr 22, 2015
 *      Author: Peter
 */

#include <cstring>
#include "MySmartCar.h"
#include <libsc/led.h>
#include <libsc/system.h>
#include <libsc/us_100.h>
#include <libsc/lcd_console.h>
#include <libsc/st7735r.h>
#include <libbase/k60/mcg.h>
#include <libbase/k60/uart.h>
#include "MyLoop.h"

using namespace std;
using namespace libsc;
using namespace libbase::k60;
namespace libbase
{
namespace k60
{
Mcg::Config Mcg::GetMcgConfig()
{	Mcg::Config config;
config.external_oscillator_khz = 50000;
config.core_clock_khz = 150000;
return config;
}
}
}

int main(){
	System::Init();

	Us100::Config us_config;
	us_config.id = 0;
	Us100 us(us_config);

	Led::Config led_config;
	led_config.id = 0;
	led_config.is_active_low = 0;
	Led led(led_config);

	MySmartCar myCar;

	uint16_t distance = 0;

	myCar.m_varMng.addWatchedVar(&distance);
//	myCar.m_varMng.addSharedVar(&distance, "a");
	myCar.m_varMng.Init();

	us.Start();
	Timer::TimerInt st;
	myCar.m_lcdConsole.Clear();
	while(true){
		us.Start();
		myCar.m_lcdConsole.ResetPosition();
		distance = us.GetDistance();
		myCar.m_lcdConsole << distance << MyLcd::endl;
		if(us.GetDistance() == 0){
			led.Switch();
		}
		myCar.m_varMng.sendWatchData();
		MyLoop::DelayMsByTicks(50);
	}
}
