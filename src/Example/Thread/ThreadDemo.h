/*
 * ThreadDemo.h
 *
 *  Created on: 2016年11月22日
 *      Author: tom
 */

#ifndef THREADDEMO_H_
#define THREADDEMO_H_
#include "MyThread.h"
#include "../LED/BlinkLed.h"

class ThreadDemo {
public:
	ThreadDemo();
	virtual ~ThreadDemo();
	void start();
private:
	MyThread thread;
	BlinkLed led;
};

#endif /* THREADDEMO_H_ */
