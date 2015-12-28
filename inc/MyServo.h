<<<<<<< HEAD
/*
 * MyServo.h
 *
 *  Created on: Mar 6, 2015
 *      Author: Peter
 */

#pragma once

#include <array>

#include <libsc/config.h>
#include <libsc/trs_d05.h>
#include <libbase/k60/adc.h>
#include <libsc/system.h>
#include <libsc/led.h>

#include "MyKalmanFilter.h"
#include "PIDhandler.h"
#include "MyConfig.h"
#include "MyVar.h"
#include "MyLoop.h"

using namespace libsc;
using namespace libbase::k60;
using namespace std;

//#ifdef LIBSC_USE_SERVO

#define MID_SERVO_DEGREE 			9000
#define MIN_SERVO_TURNING_ANGLE 	0
#define MAX_SERVO_TURNING_ANGLE 	8000

// cross road ->
//			  |
//
//SD L->1or>1 R<normal
//FD L>normal R>SDLor>1

// cross road ^
//			  |__
//
//oppsite to above

class MyServo
{

public:

	enum struct LastTurningDirection
	{
		LEFT = -1,
		FORWARD,
		RIGHT
	};

//#ifdef LIBSC_USE_MAGSEN
	class MyMagSenPair
	{

	public:

		enum WHICH
		{
			LEFT = 0,
			RIGHT
		};

		explicit MyMagSenPair(const uint8_t id, MyConfig &config);

		void processReading(void);

		float getMagSenDiff(void);
		float getMagSenSum(void);
		float getMagSenPerentage(void);

		float *getFilteredLeftReadingPointer(void);
		float *getFilteredRightReadingPointer(void);

		float operator[](size_t index);

		float *getDistanceFromWire(void);

	private:

		float					m_MagSenFilteredValue[2];
		float					m_MagSenOffsetPrediction;

		float					*DistanceWhenMaxDiff;
		float					*MaxSenReadingDiff;

		MyKalmanFilter			m_MagSenFilters[2];
		Adc						m_MagSens[2];

		MyKalmanFilter *getFilters(MyConfig &config);

	};

	class MyLeds
	{

	public:

		explicit MyLeds(MyConfig &config);

		Led &operator[](size_t index);

		void turnAllOff(void);
		void turnOffOtherAndSetEnabled(uint8_t id);

	private:

		Led						m_leds[4];

	};

//#endif

	explicit MyServo(MyConfig &config, MyVar &vars, MyLoop &loop);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	void reset(void);

	void turn(const int16_t degree_x10);

	bool isNoSignal(void);
	bool isCrossRoad(void);

	void getMagSenRange(const Timer::TimerInt interval);

	void dropDownLastAngle(const int16_t degree_x10);

	MyConfig::SmartCarPosition getPosState(void);

	static MyServo *getServoInstance(void);

	static void turningControlRoutine(void);

private:

	enum Functions
	{
		turnAccordingToMagSenHD = 0,
		turnAccordingToMagSenFD,
		turnWithMaxAngle
	};

	typedef struct AngleControlQueue
	{
//		int16_t targetAngle;
		MyConfig::SmartCarTurning stopUntilType;
		MyConfig::SmartCarPosStatus runWhilePos;
		bool canBeChanged;
		Functions functionIndex;
	};

	Timer::TimerInt		m_lastProcessTurningControlTime;
	int16_t				m_lastTurningAngle;

	float				m_maxSDValue;
	float				m_maxFDValue;
	float				m_maxLRValue;

//#ifdef LIBSC_USE_MAGSEN
	MyMagSenPair		m_MagSenSD;
	float 				diffResultSD;
	float 				sumResultSD;
	MyMagSenPair		m_MagSenFD;
	float 				diffResultFD;
	float 				sumResultFD;
	MyMagSenPair		m_MagSenHD;
	float 				diffResultHD;
	float 				sumResultHD;

	float				m_SDRatio;
	float				m_HDRatio;

	float				m_referenceReading;

	bool				m_isStarted;
	int16_t				m_setAngleList[20];
	uint8_t				m_lastAngleListIndex;
	int32_t				m_lastAngleListSum;

	float				m_HDNoSignalThreshold;
	float				m_HDHighValueThreshold;

	float				m_FDHighValueThreshold;
	float				m_FDLowValueThreshold;

	AngleControlQueue	m_specialControl;
	bool				m_hasItem;

	uint8_t				usedMagSenPairs;
//#endif

	MyLeds				m_leds;
	MyConfig::SmartCarTurning 	m_turningState;
	MyConfig::SmartCarPosition	m_position;
	MyConfig::SmartCarPosStatus	m_positionStatus;

	PIDhandler 			m_turningController;
	TrsD05				m_servo;


	int16_t getNextAngle(void);

	LastTurningDirection updateLastTurningAngle(void);

//	void turningDispatch(int16_t degree_x10);

	void updateLastState(void);

	void applyResult(void);

	void processControlQueue(void);

	void updateControlInstruction(const/*int16_t targetAngle*/Functions functionIndex, const MyConfig::SmartCarTurning untilType, const MyConfig::SmartCarPosStatus whilePos, const bool allowChange);
	void removeControlInstruction(void);

};

//#endif
=======
/*
 * MyServo.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

#include <array>
#include <libbase/k60/adc.h>
#include <libsc/timer.h>
#include <libsc/trs_d05.h>
#include <MyPid.h>
#include "MyMagSen.h"
#include "MyResource.h"

#define MID_SERVO_ANGLE 900
#define MID_SERVO_DEGREE 900

#define MAX_SERVO_ANGLE 500

#define MAX_SERVO_DEGREE MID_SERVO_DEGREE + MAX_SERVO_ANGLE
#define MIN_SERVO_DEGREE MID_SERVO_DEGREE - MAX_SERVO_ANGLE

#define isInRange(n, v, x) ((v >= n) && (v <= x))
#define isSignedF(v) (v < 0.0f)
#define isAround(v, t, o) (ABS(v - t) <= o)

using namespace libsc;
using namespace libbase::k60;
using namespace std;

class MyServo : private TrsD05
{

public:

	enum Direction
	{
		LEFT = 0,
		RIGHT
	};

	enum PidType
	{
		Normal = 0,
		Turning
	};

	enum LastServoLockDirection
	{
		None = 0,
		Left,
		Right
	};

	struct Turning90DegreeChecker
	{
		Timer::TimerInt			lastTime;
		bool					isWaitingForSignalAgain;
		bool					is90DegreeTurningStarted;
		LastServoLockDirection	direction;
	};

	MyServo(void);

	void reset(void);

	void updateMagSen(void);
	float getFinalAngle(void);

	bool turningHandler(float &error);
	bool check90Degree(float &error);
	bool checkSmallS(float &error);

	void setAngle(const int16_t degree);
	void setDegree(const uint16_t degree);
	static void updateAngle(const uint32_t &timeDelay);

	void setEnabled(const bool enabled);
	bool isEnabled(void);

	bool					m_allow90DegreeTurning;
	bool					m_isPidNonLinear;
	bool					m_90DegreeTurningNeed;
	float					m_lastError;
	int16_t					m_finalAngle;
	uint16_t				m_lastDegree;
	LastServoLockDirection	m_lastTurningDirection;
	LastServoLockDirection	m_last90TurningDirection;
	array<MyMagSen, 3>		m_MagSen;

	Timer::TimerInt			m_dt;
	Timer::TimerInt			m_lastCheckTime;
	float					m_lastFdDiffL;
	float					m_lastFdDiffR;
	float					m_deiffChangeL;
	float					m_deiffChangeR;

	array<MyPid, 2>			m_servoPid;

	Turning90DegreeChecker	m_90DegreeChecker;

private:

//	array<float *, 3>		m_normal_weight;
//	array<float *, 3>		m_turning_weight;

	bool					m_enabled;
	bool					m_pidChangedFlag;

	bool					m_isTurning90Degree;

	static MyServo			*m_instance;

};
>>>>>>> refs/heads/PeterLauRemix
