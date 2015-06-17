/*
 * ultrasonic.cpp
 *
 *  Created on: Jun 12, 2015
 *      Author: chendi
 */
#include <libbase/k60/pin.h>
#include "ultrasonic.h"
#include <libbase/k60/gpio.h>
#include <libsc/system.h>


using namespace libbase::k60;
using namespace libsc;

void OnUSEdge(Gpi* gpi);


Gpi::Config GetUSConfig(void)
{
	Gpi::Config product;
	product.pin = Pin::Name::kPtc13;
	product.interrupt = Pin::Config::Interrupt::kBoth;
	product.isr = &OnUSEdge;
	return product;
}
