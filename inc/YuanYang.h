/*
 * YuanYang.h
 *
 *  Created on: Jun 17, 2015
 *      Author: chendi
 */

#pragma once

#include <libbase/helper.h>
#include <cstdint>
#include LIBBASE_H(gpio)
#include LIBBASE_H(pit)

using namespace libbase::k60;

/*
 * YuanYang is an ultrasonic module bought from taobao.
 * http://item.taobao.com/item.htm?spm=a230r.1.14.1.saWTib&id=43852802532&ns=1&abbucket=7#detail
 */
class YuanYang{
public:
	struct Config
	{
		uint8_t id;
	};

	explicit YuanYang();
	~YuanYang();

	bool is_valid(void);
	uint32_t distance(void);             //distance is in mm

private:
	Pit pit;
	Gpi DO;
	Gpi State;
	void OnUSEdge(Gpi* gpi);
	Gpi::Config GetDoConfig(Gpi::OnGpiEventListener isr);
	Gpi::Config GetStateConfig(void);
	Pit::Config GetPitConfig(void);
	volatile uint32_t duration;
};
