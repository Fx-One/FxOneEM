/*
 * Application.cpp
 *
 *  Created on: 2016年8月27日
 *  Author: tom
 */

#include "Core/Application.h"
#include "3rd/RTOS.h"

namespace Fx {
Application::Application() {
	// TODO Auto-generated constructor stub

}

Application::~Application() {
	// TODO Auto-generated destructor stub
}

void Application::exec() {
	vTaskStartScheduler();
}
void Application::quit() {
	vTaskEndScheduler();
}
}
extern "C"
void vApplicationStackOverflowHook( TaskHandle_t pxTask, char *pcTaskName )
{
	/* This function will get called if a task overflows its stack.   If the
	parameters are corrupt then inspect pxCurrentTCB to find which was the
	offending task. */

	( void ) pxTask;
	( void ) pcTaskName;

	for( ;; );
}

/*-----------------------------------------------------------*/
extern "C"
void assert_failed( unsigned char *pucFile, unsigned long ulLine )
{
	( void ) pucFile;
	( void ) ulLine;

	for( ;; );
}
