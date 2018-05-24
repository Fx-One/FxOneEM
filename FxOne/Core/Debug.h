/*
 * Debug.h
 *
 * Created on: 2016年11月21日
 * Author: tom
 */

#ifndef CORE_DEBUG_H_
#define CORE_DEBUG_H_
#include "diag/Trace.h"
#include "ByteArray.h"
#include <string>
namespace Fx {
class Debug {
public:
	friend Debug debug();
	Debug &operator << (const std::string &str);
	Debug &operator << (const int &number);
	Debug &operator << (const unsigned int &number);
	Debug &operator << (const float &number);
	Debug &operator << (const double &number);
	Debug &operator << (const ByteArray &array);
protected:
	static bool first;
	Debug();
};
Debug debug();
}
#endif /* CORE_DEBUG_H_ */
