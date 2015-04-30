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


	St7735r::Config config9;
	config9.fps = 60;
	config9.is_bgr = true;
	St7735r lcd(config9);

	LcdConsole::Config config8;
	config8.lcd = &lcd;
	LcdConsole console(config8);

	char *buffer = new char[500] {0};
	us.Start();
	int distance = 0;
	lcd.Clear();
	while(true){
		us.Start();
		distance = us.GetDistance();
		sprintf(buffer," %d",distance);
		console.WriteString((char*)buffer);
		if(us.GetDistance() == 0){
			led.Switch();
			System::DelayS(1);
		}
	}
}
