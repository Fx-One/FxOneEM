/*
 * Mutex.h
 *
 * Created on: 2016年8月9日
 * Author: tom
 */

#ifndef THREAD_MUTEX_H_
#define THREAD_MUTEX_H_
#include "3rd/RTOS.h"
namespace Fx {
class Mutex {
public:
	Mutex();
	~Mutex();
	void lock();
	void unlock();
protected:
	xSemaphoreHandle mMutexHandle;
};
}
#endif /* THREAD_MUTEX_H_ */
