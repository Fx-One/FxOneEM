//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#include "diag/Trace.h"

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"
#include <ByteArray>
#include <Debug>
#include <Thread/Thread.h>
#include <Application>

#include "Example/ByteArray/ByteArrayDemo.h"
#include "Example/Thread/ThreadDemo.h"

ThreadDemo threadDemo;

using namespace Fx;
int main(int argc, char* argv[])
{
	ByteArrayDemo baDemo;
	baDemo.exec();

	threadDemo.start();

	Application app;
	app.exec();
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
