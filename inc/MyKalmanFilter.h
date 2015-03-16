/*
 * MyKalmanFilter.h
 *
 * Author: Tommy Wong
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#pragma once

class MyKalmanFilter
{
public:
	MyKalmanFilter(float *q, float *r, const float x, const float p);

	float Filter(const float data);

private:
	void PredictState();
	void PredictCovariance();
	void UpdateState(const float z);
	void UpdateCovariance();
	void Gain();

	float *m_q, *m_r, m_k, m_x, m_lx, m_p, m_lp;
};
