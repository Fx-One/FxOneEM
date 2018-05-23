/*
 * MyThread.h
 *
 *  Created on: 2016年8月8日
 *      Author: tom
 */

#ifndef MYTHREAD_H_
#define MYTHREAD_H_
#include "../LED/BlinkLed.h"
#include <string>

#include "Thread/Thread.h"
using namespace std;

class MyThread:public Fx::Thread {
public:
	MyThread();
	virtual ~MyThread();
	BlinkLed *blinkLed;
protected:
	void run();

private:
	string name;
	int id;
};

#endif /* MYTHREAD_H_ */
