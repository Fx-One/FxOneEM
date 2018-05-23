/*
 * Application.h
 *
 *  Created on: 2016年8月27日
 *      Author: tom
 */

#ifndef CORE_APPLICATION_H_
#define CORE_APPLICATION_H_
#include "3rd/RTOS.h"
namespace Fx {
class Application {
public:
	Application();
	virtual ~Application();
	static void exec();
	static void quit();
};
}
#endif /* CORE_APPLICATION_H_ */
