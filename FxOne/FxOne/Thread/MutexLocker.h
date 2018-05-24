/*
 * MutexLocker.h
 *
 * Created on: 2016年8月9日
 * Author: tom
 */

#ifndef THREAD_MUTEXLOCKER_H_
#define THREAD_MUTEXLOCKER_H_
#include "Thread/Mutex.h"
namespace Fx {
class MutexLocker {
public:
	MutexLocker(Mutex &mutex);
	virtual ~MutexLocker();
protected:
	Mutex *mMutex;
};
}
#endif /* THREAD_MUTEXLOCKER_H_ */
