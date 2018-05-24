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

}

Application::~Application() {

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
