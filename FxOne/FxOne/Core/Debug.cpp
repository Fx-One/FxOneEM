/*
 * Debug.cpp
 *
 * Created on: 2016年11月21日
 * Author: tom
 */

#include "Core/Debug.h"

#include "diag/Trace.h"
#include <string>
#include <sstream>
namespace Fx {
bool Debug::first(true);
Debug::Debug() {
	Debug::first = false;
}

Debug &Debug::operator << (const std::string &str)
{
	trace_printf(str.c_str());
	return *this;
}
Debug &Debug::operator << (const int &number)
{
	trace_printf("%d", number);
	return *this;
}

Debug &Debug::operator << (const unsigned int &number)
{
	trace_printf("%d", number);
	return *this;
}
Debug &Debug::operator << (const float &number)
{
	return *this;
}

Debug &Debug::operator << (const double &number)
{
	return *this;
}

Debug &Debug::operator << (const ByteArray &array)
{
	std::string hex = array.toHex().toStdString();
	for(uint32_t i = 0; i < hex.size(); i+=2) {
		trace_printf("%c", hex[i]);
		trace_printf("%c",hex[i+1]);
		trace_printf(" ");
	}
	return *this;
}

Debug debug() {
	if(!Debug::first) {
		trace_printf("\n");
	}
	return Debug();
}

}
