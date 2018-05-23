/*
 * ThreadDemo.cpp
 *
 *  Created on: 2016年11月22日
 *      Author: tom
 */

#include "ThreadDemo.h"


ThreadDemo::ThreadDemo() {
	thread.blinkLed = &led;
}

ThreadDemo::~ThreadDemo() {

}


void ThreadDemo::start() {
	thread.start(1);
}
