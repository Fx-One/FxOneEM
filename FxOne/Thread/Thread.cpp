/*
 * Thread.cpp
 *
 *  Created on: 2016年8月8日
 *      Author: tom
 */

#include "Thread/Thread.h"
namespace Fx {
void ThreadTask(void * pvParameters) {
	Thread *thread = (Thread *) pvParameters;
	thread->run();
	thread->terminate();
}

Thread::Thread() {
	stackSize = 400;
	taskHandle = NULL;
}

Thread::~Thread() {

}

void Thread::terminate()
{
	if(taskHandle != NULL) {
		vTaskDelete(taskHandle);
		taskHandle = NULL;
	}
}

bool Thread::start(int priority)
{
	return xTaskCreate(ThreadTask, "", 	(unsigned short int)stackSize,
			this, priority, &taskHandle);
}

void Thread::sleep(int ms){
	vTaskDelay(ms / portTICK_RATE_MS);
}

void Thread::setStackSize(int size){
	stackSize = size;
}

}
