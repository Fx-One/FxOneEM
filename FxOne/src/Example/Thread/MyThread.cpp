/*
 * MyThread.cpp
 *
 *  Created on: 2016年8月8日
 *      Author: tom
 */

#include "MyThread.h"
#include <Debug>
using namespace Fx;

MyThread::MyThread():Thread() {
	blinkLed = NULL;
}

MyThread::~MyThread() {
	// TODO Auto-generated destructor stub
}

void MyThread::run() {
	name = "thread";
	id = 123;

	blinkLed->powerUp();
	while (1)
	{
		debug() << name;
		blinkLed->turnOn();
		sleep(5);
		blinkLed->turnOff();
		sleep(5);

	}
}
