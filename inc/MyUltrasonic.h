/*
 * MyUltrasonic.h
 *
 *  Created on: Jun 19, 2015
 *      Author: Jeffrey Tsang
 */

#pragma once

#include "YuanYang.h"

#define MIN_DISTANCE 50 //in mm

class MyUltrasonic : private YuanYang{

public:

	MyUltrasonic(void);

	void setDistanceFeedback(void);
	bool isBrake(void);

private:

	static MyUltrasonic *m_instance;

	bool m_brake;
};
