/*
 * MyUltrasonic.cpp
 *
 *  Created on: Jun 19, 2015
 *      Author: Jeffrey Tsang
 */

#include <libbase/k60/pin.h>
#include <libbase/k60/gpio.h>
#include <libsc/system.h>
#include "MyUltrasonic.h"
#include "YuanYang.h"

using namespace libbase::k60;

MyUltrasonic *MyUltrasonic::m_instance = nullptr;

YuanYang::Config getUSConfig(uint8_t id){
	YuanYang::Config config;
	config.id = id;
	return config;
}

MyUltrasonic::MyUltrasonic(void)
:
		YuanYang(getUSConfig(0)),
		m_brake(false)
{}

void MyUltrasonic::setDistanceFeedback(void)
{
	if (distance() <= MIN_DISTANCE){
		m_brake = true;
	}
	else {m_brake = false;}
}

bool MyUltrasonic::isBrake(void){
	return m_brake;
}
