/*
 * Time.h
 *
 *  Created on: 2017年5月27日
 *      Author: tom
 */

#ifndef CORE_TIME_H_
#define CORE_TIME_H_

namespace Fx {

class Time {
public:
	Time();
	virtual ~Time();
	static void awkDelay(long us);
};

} /* namespace Fx */

#endif /* CORE_TIME_H_ */
