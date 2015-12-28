<<<<<<< HEAD
/*
 * main.cpp
 *
 *  Created on: Dec 29, 2014
 *      Author: Peter
 */

#include <libbase/k60/mcg.h>

#include "MySmartCar.h"
#include "PIDhandler.h"
#include "MyConfig.h"
#include "MyVarManager.h"
#include "MyVar.h"
#include "MySmartCar.h"

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
using namespace std;

MyConfig globalConfig;
MyVar globalVars;
MySmartCar myCar(globalConfig, globalVars);

void myListener(const vector<Byte> &bytes)
{
	switch (bytes[0])
	{

	case 'm':
		globalConfig.MyMotorSpeedControlRef += 10;
		break;
	case 'M':
		globalConfig.MyMotorSpeedControlRef = max(globalConfig.MyMotorSpeedControlRef - 10, 0.0f);
		break;

	case 'p':
		globalConfig.MyMotorSpeedControlKp += 0.0001f;
		break;
	case 'P':
		globalConfig.MyMotorSpeedControlKp = max(globalConfig.MyMotorSpeedControlKp - 0.0001f, 0.0f);
		break;

	case 'i':
		globalConfig.MyMotorSpeedControlKi += 0.00001f;
		break;
	case 'I':
		globalConfig.MyMotorSpeedControlKi = max(globalConfig.MyMotorSpeedControlKi - 0.0001f, 0.0f);
		break;

	case 'd':
		globalConfig.MyMotorSpeedControlKd += 0.001f;
		break;
	case 'D':
		globalConfig.MyMotorSpeedControlKd = max(globalConfig.MyMotorSpeedControlKd - 0.001f, 0.0f);
		break;

	case 'q':
		globalConfig.MyMagSenFilterQ += 0.0005f;
		break;
	case 'Q':
		globalConfig.MyMagSenFilterQ = max(globalConfig.MyMagSenFilterQ - 0.0005f, 0.0f);
		break;
	case 'r':
		globalConfig.MyMagSenFilterR += 0.01f;
		break;
	case 'R':
		globalConfig.MyMagSenFilterR = max(globalConfig.MyMagSenFilterR - 0.01f, 0.0f);
		break;

	case 'h':
		globalConfig.MyServoTurningKp += 100.0f;
		break;
	case 'H':
		globalConfig.MyServoTurningKp = max(globalConfig.MyServoTurningKp - 100.0f, 0.0f);
		break;
	case 'j':
		globalConfig.MyServoTurningKp += 10.0f;
		break;
	case 'J':
		globalConfig.MyServoTurningKp = max(globalConfig.MyServoTurningKp - 10.0f, 0.0f);
		break;

	case 'k':
		globalConfig.MyMagSenFilterQ += 0.0001f;
		break;
	case 'K':
		globalConfig.MyMagSenFilterQ = max(globalConfig.MyMagSenFilterQ - 0.0001f, 0.0f);
		break;
	case 'l':
		globalConfig.MyMagSenFilterR += 0.05f;
		break;
	case 'L':
		globalConfig.MyMagSenFilterR = max(globalConfig.MyMagSenFilterR - 0.05f, 0.0f);
		break;

	case ';':
		myCar.motorSetEnabled(true);
		break;
	case ':':
		myCar.motorSetEnabled(false);
		break;
	case '\'':
		myCar.servoSetEnabled(true);
		break;
	case '\"':
		myCar.servoSetEnabled(false);
		break;
	case ',':
		globalConfig.MySmartCarPowerMode = MyConfig::SmartCarPowerMode::kNormalMode;
		break;
	case '<':
		globalConfig.MySmartCarPowerMode = MyConfig::SmartCarPowerMode::kLowFrictionMode;
		break;

	case '[':
		globalConfig.MyMagSenSDRatio = min(globalConfig.MyMagSenSDRatio + 0.05f, 1.0f);
		break;
	case ']':
		globalConfig.MyMagSenSDRatio = max(globalConfig.MyMagSenSDRatio - 0.05f, 0.0f);
		break;

	case '{':
		globalConfig.MyMagSenHDRatio = min(globalConfig.MyMagSenHDRatio + 0.05f, 1.0f);
		break;
	case '}':
		globalConfig.MyMagSenHDRatio = max(globalConfig.MyMagSenHDRatio - 0.05f, 0.0f);
		break;
	}
}

int main()
{
	myCar.myVarMng.addWatchedVar(globalVars.MagSenSDLeft, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.MagSenSDRight, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.MagSenFDLeft, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.MagSenFDRight, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.MagSenHDLeft, "asd");
	myCar.myVarMng.addWatchedVar(globalVars.MagSenHDRight, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.magSenReferenceReading, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.lastAngleListSum, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.MagSenFDRight, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.lastTurningAngle, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.speed, "asd");
//	myCar.myVarMng.addWatchedVar(globalVars.lastCount, "asd");
//	myCar.myVarMng.addWatchedVar(&globalConfig.MyMotorSpeedControlRef, "asd");
	myCar.myVarMng.Init(&myListener);

	myCar.startMainLoop();

}
=======
/*
 * main.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cassert>
#include <cstring>
#include <libbase/k60/mcg.h>
#include <libsc/button.h>
#include <libbase/k60/gpio.h>
#include <libsc/system.h>
#include "MyResource.h"
#include "MyLcd.h"
#include "MySmartCar.h"
#include <libbase/k60/flash.h>

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

void myListener(const std::vector<Byte>& bytes)
{
	switch (bytes[0])
	{
	case 'e':
		MyResource::smartCar().m_motor.setEnabled(!MyResource::smartCar().m_motor.isEnabled());
		break;

	case 'E':
		MyResource::smartCar().m_servo.setEnabled(!MyResource::smartCar().m_servo.isEnabled());
		break;

	case 'b':
		MyResource::smartCar().m_buzzer.setEnabled(!MyResource::smartCar().m_buzzer.isEnabled());
		break;

	case 'r':
		MyResource::smartCar().m_lcdConsole.onDraw(99999);
		break;

	case 'w':
		MyResource::smartCar().m_motor.setSpeed(400);
		MyResource::smartCar().m_motor.setEnabled(true);
		break;

	case 's':
		MyResource::smartCar().m_motor.setSpeed(-100);
		MyResource::smartCar().m_motor.setEnabled(false);
		break;

	case 'a':
		MyResource::smartCar().m_servo.setEnabled(true);
		MyResource::smartCar().m_servo.setAngle(450);
		break;

	case 'd':
		MyResource::smartCar().m_servo.setEnabled(true);
		MyResource::smartCar().m_servo.setAngle(-450);
		break;

//	case 'a':
//		MyResource::ConfigTable::MotorConfig::Kp += 0.005f;
//		break;
//
//	case 'A':
//		MyResource::ConfigTable::ServoConfig::NormalKp += 10.0f;
//		break;
//
//	case 'z':
//		MyResource::ConfigTable::MotorConfig::Kp = (MyResource::ConfigTable::MotorConfig::Kp - 0.005f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Kp - 0.005f;
//		break;
//
//	case 'Z':
//		MyResource::ConfigTable::ServoConfig::NormalKp = (MyResource::ConfigTable::ServoConfig::NormalKp - 10.0f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::NormalKp - 10.0f;
//		break;
//
//	case 's':
//		MyResource::ConfigTable::MotorConfig::Ki += 0.001f;
//		break;
//
//	case 'S':
//		MyResource::ConfigTable::ServoConfig::NormalKi += 0.1f;
//		break;
//
//	case 'x':
//		MyResource::ConfigTable::MotorConfig::Ki = (MyResource::ConfigTable::MotorConfig::Ki - 0.001f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Ki - 0.001f;
//		break;
//
//	case 'X':
//		MyResource::ConfigTable::ServoConfig::NormalKi = (MyResource::ConfigTable::ServoConfig::NormalKi - 0.1f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::NormalKi - 0.1f;
//		break;
//
//	case 'd':
//		MyResource::ConfigTable::MotorConfig::Kd += 0.0001f;
//		break;
//
//	case 'D':
//		MyResource::ConfigTable::ServoConfig::NormalKd += 1.0f;
//		break;
//
//	case 'c':
//		MyResource::ConfigTable::MotorConfig::Kd = (MyResource::ConfigTable::MotorConfig::Kd - 0.0001f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Kd - 0.0001f;
//		break;
//
//	case 'C':
//		MyResource::ConfigTable::ServoConfig::NormalKd = (MyResource::ConfigTable::ServoConfig::NormalKd - 1.0f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::NormalKd - 1.0f;
//		break;
//
	case 'f':
		MyResource::ConfigTable::MotorConfig::Reference += 100.0f;
		break;

	case 'v':
		MyResource::ConfigTable::MotorConfig::Reference = (MyResource::ConfigTable::MotorConfig::Reference - 100.0f < 0.0f)? 0.0f : MyResource::ConfigTable::MotorConfig::Reference - 100.0f;
		break;
//
//	case 'F':
//		MyResource::ConfigTable::ServoConfig::TurningKpA += 10.0f;
//		break;
//
//	case 'V':
//		MyResource::ConfigTable::ServoConfig::TurningKpA = (MyResource::ConfigTable::ServoConfig::TurningKpA - 10.0f < 0.0f)? 0.0f : MyResource::ConfigTable::ServoConfig::TurningKpA - 10.0f;
//		break;
	}

//	if (!MyResource::smartCar().m_mode)
//		MyResource::smartCar().m_lcdConsole.onDraw(99999);
	myCar.m_lastTimeUpdateKey = System::Time();
}

void OnPress(const uint8_t id)
{
	switch (id)
	{
	default:
		assert(false);
		break;

	case 0:
//		myCar.m_batteryMeter.checkBattery(0);
		DelayMsByTicks(500);
		MyResource::smartCar().m_motor.setEnabled(!MyResource::smartCar().m_motor.isEnabled());
		break;

	case 1:
//		myCar.m_servo.setDegree(MID_SERVO_ANGLE);
//		myCar.m_servo.setEnabled(!myCar.m_servo.isEnabled());
//		myCar.m_motor.setEnabled(!myCar.m_motor.isEnabled());
		MyResource::smartCar().m_flash.eraseAll();
		break;
	}
}

int main(void)
{
	System::Init();

	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getOutputValue(), "SD_Output");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getOutputValue(), "FD_Output");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getOutputValue(), "HD_Output");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getFilteredValueAvg(), "SD_Avg");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getFilteredValueAvg(), "FD_Avg");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getFilteredValueAvg(), "HD_Avg");
	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_finalAngle, "Angle");

//	myCar.m_varMng.addWatchedVar((uint8_t *)&MyResource::smartCar().m_servo.m_90DegreeTurningNeed, "90DegreeMode");
//	myCar.m_varMng.addWatchedVar((uint8_t *)&MyResource::smartCar().m_servo.m_isPidNonLinear, "PidNonLinear");

//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_lastError, "IntegratedOutput");
//	myCar.m_varMng.addWatchedVar(myCar.m_servo.m_servoPid[MyServo::Normal].getLastError(), "LastError");
//	myCar.m_varMng.addWatchedVar(myCar.m_servo.m_servoPid[MyServo::PidType::Turning].getLastError(), "LastError");

//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getFilteredValue()[0], "SD_FilteredValueL");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[0].getFilteredValue()[1], "SD_FilteredValueR");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getFilteredValue()[0], "FD_FilteredValueL");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[1].getFilteredValue()[1], "FD_FilteredValueR");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getFilteredValue()[0], "HD_FilteredValueL");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_MagSen[2].getFilteredValue()[1], "HD_FilteredValueR");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_finalAngle, "Angle");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_deiffChangeL, "<sup>d</sup>/dt(Fd_L)");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_deiffChangeR, "<sup>d</sup>/dt(Fd_R)");
//	myCar.m_varMng.addWatchedVar(&myCar.m_servo.m_FdDiffChangingRateChangingRate, "FdDiffRate");

//	myCar.m_varMng.addWatchedVar(myCar.m_motor.m_encoder.getEncoderCountPointer(), "EncoderCount");
//	myCar.m_varMng.addWatchedVar(myCar.m_motor.getPower(), "Speed");
//	myCar.m_varMng.addWatchedVar(&MyResource::smartCar().m_motor.m_curReference, "EncoderTaget");
//	myCar.m_varMng.addWatchedVar((uint8_t *)&MyResource::smartCar().m_servo.m_lastTurningDirection, "LastDirection");
//	myCar.m_varMng.addWatchedVar(myCar.m_motor.m_speedPid.getLastError(), "Error");
//	myCar.m_varMng.addWatchedVar(&myCar.m_motor.m_speedPid.m_output, "Output");

	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Reference, "Motor_Ref");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::NormalKp, "Servo_nKp");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::NormalKd, "Servo_nKd");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::TurningKpA, "Servo_tKpA");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::TurningKpB, "Servo_tKpB");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::TurningKd, "Servo_tKd");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightSD, "Weight_SD");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightFD, "Weight_FD");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::WeightHD, "Weight_HD");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Kp, "Kp");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MotorConfig::Kd, "Kd");
	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::SdNoSignalThreshold, "Sd_TurningTH");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::ServoConfig::Turning90DegreeThresholdFd, "Fd_90");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MagSenConfig::Kq, "Kq");
//	myCar.m_varMng.addSharedVar(&MyResource::ConfigTable::MagSenConfig::Kr, "Kr");

	Button::Config buttonConfig;
	buttonConfig.listener_trigger = Button::Config::Trigger::kDown;
	buttonConfig.listener = &OnPress;

	buttonConfig.id = 0;
	Button bnt0(buttonConfig);

	buttonConfig.id = 1;
	Button bnt1(buttonConfig);

	myCar.m_varMng.SetOnReceiveListener(&myListener);
//	myCar.m_varMng.Init(&myListener);

	MyResource::smartCar().m_lcdConsole.onDraw(99999);

	myCar.m_batteryMeter.checkBattery(0);

	System::DelayMs(500);

//	myCar.m_servo.setAngle(0);

	myCar.m_loop.start();

}
>>>>>>> refs/heads/PeterLauRemix
