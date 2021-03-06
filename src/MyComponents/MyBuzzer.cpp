/*
 * MyBuzzer.cpp
 *
 * Author: Peter
 * Copyright (c) 2014-2015 HKUST SmartCar Team
 * Refer to LICENSE for details
 */

#include <cstring>
#include <libbase/k60/soft_pwm.h>
#include <libbase/k60/pin.h>
#include <libbase/k60/pit.h>
#include "MyBuzzer.h"
#include "MyLoop.h"
#include "MyResource.h"

using namespace libbase::k60;
using namespace libsc;

inline Pin::Name getPwmPin(const uint8_t id)
{
	assert(id == 0);

	return LIBSC_BUZZER0;
}

//SoftPwm::Config MyBuzzer::getSoftFtmConfig(const uint8_t id)
//{
//	SoftPwm::Config config;
//	config.pin = getPwmPin(id);
//	config.period = getPeriod(m_noteIndex);
//	config.pos_width = getPosWidth(m_loudness);
//	config.precision = Pwm::Config::Precision::kUs;
//	config.pit_channel = 0;
//	return config;
//}

Gpo::Config MyBuzzer::getGpoConfig(void)
{
	Gpo::Config config;
	config.is_high = false;
	config.pin = getPwmPin(0);
	return config;
}

MyBuzzer::MyBuzzer(void)
:
//	m_noteIndex(40),
//	m_loudness(100),
	m_gpo(getGpoConfig()),
	m_allowChange(true),
	m_enabled(true)
//	m_pwm(getSoftFtmConfig(0))
{
	System::Init();
}

//void MyBuzzer::noteDown(const uint8_t note, const uint16_t delayMs)
//{
//	setNote(note);
//	if (delayMs > 0)
//	{
//		System::DelayMs(delayMs);
//		setLoudness(0);
//	}
//}
//
//void MyBuzzer::noteDown(const char noteName[5], const uint16_t delayMs)
//{
//	setNote(noteName);
//	if (delayMs > 0)
//	{
//		System::DelayMs(delayMs);
//		setLoudness(0);
//	}
//}

void MyBuzzer::setEnabled(const bool enabled)
{
//	set(false, enabled);
	m_enabled = enabled;
}

bool MyBuzzer::isEnabled(void) const
{
	return m_enabled;
}

void MyBuzzer::set(const bool beep, const bool allowChange)
{
	if (m_enabled)
	{
		m_gpo.Set(beep);
		m_allowChange = allowChange;
	}
}

void MyBuzzer::toggle(void)
{
	if (m_allowChange && m_enabled)
		m_gpo.Turn();
}

void MyBuzzer::beep(const uint8_t times, const uint16_t delayMs)
{
	if (m_allowChange && m_enabled)
		for (int i = 0; i < times; i++)
		{
			if (i)
				DelayMsByTicks(delayMs / 2);
			set(true);
			DelayMsByTicks(delayMs / 2);
			set(false);
		}
}

//void MyBuzzer::setLoudness(const uint8_t percentage)
//{
//	if (percentage)
//		m_loudness = percentage;
//	m_pwm.SetPosWidth(getPosWidth(percentage));
//}
//
//uint32_t MyBuzzer::getPeriod(const float freq)
//{
//	return (uint32_t)(1000000.0f / freq);
//}
//
//uint32_t MyBuzzer::getPosWidth(const uint8_t percentage)
//{
//	return (uint32_t)(getPeriod(notes[m_noteIndex].freq) * inRange(0, percentage, 100) * 0.003f);
//}
//
//void MyBuzzer::setNote(const uint8_t index, const uint8_t percentage)
//{
//	m_noteIndex = inRange(0, index, 87);
//	m_loudness = (percentage > 100)? m_loudness : percentage;
//	m_pwm.SetPeriod(getPeriod(notes[m_noteIndex].freq), getPosWidth((percentage > 100)? m_loudness : percentage));
//}
//void MyBuzzer::setNote(const char noteName[5], const uint8_t percentage)
//{
//	m_noteIndex = getNoteIndexByName(noteName);
//	m_loudness = (percentage > 100)? m_loudness : percentage;
//	m_pwm.SetPeriod(getPeriod(notes[m_noteIndex].freq), getPosWidth((percentage > 100)? m_loudness : percentage));
//}
//
//uint8_t MyBuzzer::getNoteIndexByName(const char noteName[5])
//{
//	for (uint8_t i = 0; i < 88; i++)
//		if (strcmp(noteName, notes[i].name) == 0)
//			return i;
//
//	assert(false);
//}
//
//const MyBuzzer::note MyBuzzer::notes[88] =
//{
//	{ "A0", 27.5 },
//	{ "A#0", 29.1352 },
//	{ "B0", 30.8677 },
//	{ "C1", 32.7032 },
//	{ "C#1", 34.6478 },
//	{ "D1", 36.7081 },
//	{ "D#1", 38.8909 },
//	{ "E1", 41.2034 },
//	{ "F1", 43.6535 },
//	{ "F#1", 46.2493 },
//	{ "G1", 48.9994 },
//	{ "G#1", 51.9131 },
//	{ "A1", 55.0000 },
//	{ "A#1", 58.2705 },
//	{ "B1", 61.7354 },
//	{ "C2", 65.4064 },
//	{ "C#2", 69.2957 },
//	{ "D2", 73.4162 },
//	{ "D#2", 77.7817 },
//	{ "E2", 82.4069 },
//	{ "F2", 87.3071 },
//	{ "F#2", 92.4986 },
//	{ "G2", 97.9989 },
//	{ "G#2", 103.826 },
//	{ "A2", 110.000 },
//	{ "A#2", 116.541 },
//	{ "B2", 123.471 },
//	{ "C3", 130.813 },
//	{ "C#3", 138.591},
//	{ "D3", 146.832 },
//	{ "D#3", 155.563 },
//	{ "E3", 164.814 },
//	{ "F3", 174.614 },
//	{ "F#3", 184.997 },
//	{ "G3", 195.998 },
//	{ "G#3", 207.652 },
//	{ "A3", 220.000 },
//	{ "A#3", 233.082 },
//	{ "B3", 246.942 },
//	{ "C4", 261.626 },
//	{ "C#4", 277.183 },
//	{ "D4", 293.665 },
//	{ "D#4", 311.127 },
//	{ "E4", 329.628 },
//	{ "F4", 349.228 },
//	{ "F#4", 369.994 },
//	{ "G4", 391.995 },
//	{ "G#4", 415.305 },
//	{ "A4", 440.000 },
//	{ "A#4", 466.164 },
//	{ "B4", 493.883 },
//	{ "C5", 523.251 },
//	{ "C#5", 554.365 },
//	{ "D5", 587.330 },
//	{ "D#5", 622.254 },
//	{ "E5", 659.255 },
//	{ "F5", 698.456 },
//	{ "F#5", 739.989 },
//	{ "G5", 783.991 },
//	{ "G#5", 830.609 },
//	{ "A5", 880.000 },
//	{ "A#5", 932.328 },
//	{ "B5", 987.767 },
//	{ "C6", 1046.50 },
//	{ "C#6", 1108.73 },
//	{ "D6", 1174.66 },
//	{ "D#6", 1244.51 },
//	{ "E6", 1318.51 },
//	{ "F6", 1396.91 },
//	{ "F#6", 1479.98 },
//	{ "G6", 1567.98 },
//	{ "G#6", 1661.22 },
//	{ "A6", 1760.00 },
//	{ "A#6", 1864.66 },
//	{ "B6", 1975.53 },
//	{ "C7", 2093.00 },
//	{ "C#7", 2217.46 },
//	{ "D7", 2349.32 },
//	{ "D#7", 2489.02 },
//	{ "E7", 2637.02 },
//	{ "F7", 2793.83 },
//	{ "F#7", 2959.96 },
//	{ "G7", 3135.96 },
//	{ "G#7", 3322.44 },
//	{ "A7", 3520.00 },
//	{ "A#7", 3729.31 },
//	{ "B7", 3951.07 },
//	{ "C8", 4186.01 }
//};
