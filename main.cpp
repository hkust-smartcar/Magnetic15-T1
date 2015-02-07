/*
 * main.cpp
 *
 * Author: Cindy Yu, Jade Wu, Peter Lau
 * Copyright (c) 2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <libbase/k60/mcg.h>

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

int main()
{
	return 0;
}
