/*
 * MutexLocker.cpp
 *
 *  Created on: 2016年8月9日
 *      Author: tom
 */

#include "Thread/MutexLocker.h"
namespace Fx {
MutexLocker::MutexLocker(Mutex &mutex) {
	mMutex = &mutex;
	mMutex->lock();
}

MutexLocker::~MutexLocker() {
	mMutex->unlock();
}
}
