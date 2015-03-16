/*
 * PIDhandler.h
 *
 *  Created on: Jan 15, 2015
 *      Author: Peter
 */

#include <libsc/k60/system.h>

#pragma once

#define MAX(a, b) ((a > b)? a : b)

#define EPSILON_RATIO			0.05

using namespace libsc::k60;

class PIDhandler
{
public:

	explicit PIDhandler(float *ref, float *kp, float *ki, float *kd, const float min, const float max);
	float updatePID(float val);

	float getKp(void);
	float getKi(void);
	float getKd(void);

	void reset(void);

private:

	float min;
	float max;

	float *reference;
	float *Kp;
	float *Ki;
	float *Kd;

	float eSum;
	float lastError;
	float epsilon;
	Timer::TimerInt lastTimeUpdate;

	float output;
};
