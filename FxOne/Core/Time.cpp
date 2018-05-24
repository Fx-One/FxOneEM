/*
 * Time.cpp
 *
 * Created on: 2017年5月27日
 * Author: tom
 */

#include <Core/Time.h>

#if defined (STM32F10X_HD) || (defined STM32F10X_XL) || (defined STM32F10X_HD_VL)
 #include "stm32f10x.h"
#endif

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx)|| defined(STM32F417xx)
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"
#endif

namespace Fx {

Time::Time() {

}

Time::~Time() {

}

void Time::awkDelay(long us)
{
	us = us * (SystemCoreClock / 1000000);
	while(us > 0) {
		--us;
	}
}

} /* namespace Fx */
