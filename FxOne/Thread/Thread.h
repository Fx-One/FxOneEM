/*
 * Thread.h
 *
 * Created on: 2016年8月8日
 * Author: tom
 */

#ifndef THREAD_THREAD_H_
#define THREAD_THREAD_H_
#include "3rd/RTOS.h"
#include "Core/Object.h"
namespace Fx {
class Thread : public Object {
public:
	Thread();
	virtual ~Thread();
	bool start(int priority);
	void terminate();
	static void sleep(int ms);
	friend void ThreadTask(void * pvParameters);

protected:
	virtual void run(){}
private:
	xTaskHandle taskHandle;
	int stackSize;
public:
	void setStackSize(int size);
	int getStackSize() const;
};
}
#endif /* THREAD_THREAD_H_ */
