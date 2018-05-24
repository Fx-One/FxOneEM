/*
 * Mutex.cpp
 *
 * Created on: 2016年8月9日
 * Author: tom
 */

#include "Thread/Mutex.h"
namespace Fx {
Mutex::Mutex() {
	mMutexHandle = xSemaphoreCreateMutex();
}

Mutex::~Mutex() {
	xSemaphoreGive(mMutexHandle);
	vSemaphoreDelete(mMutexHandle);
}

void Mutex::lock() {
	xSemaphoreTake(mMutexHandle, portMAX_DELAY);
}

void Mutex::unlock() {
	xSemaphoreGive(mMutexHandle);
}
}
