/*
 * ByteArrayDemo.cpp
 *
 *  Created on: 2016年11月8日
 *      Author: tom
 */

#include "ByteArrayDemo.h"
#include <Debug>
using namespace Fx;
ByteArrayDemo::ByteArrayDemo() {
	// TODO Auto-generated constructor stub

}

void ByteArrayDemo::exec() {
	ByteArray array;
	array.append("123456HELLO");
	ByteArray so;
	so.append("1234");

	ByteArray eo;
	eo.append("HELLO");
	if(array.startsWith(so)) {
		debug() << "START WITH";
	}

	if(array.endsWith(eo)) {
		debug() << "END WITH";
	}


	ByteArray larray = array.left(5);
	ByteArray rarray = array.right(5);

	debug() << larray.toStdString();

	debug() << rarray.toStdString();

	array.remove(1, 5);

	debug() << array.toStdString();

	debug() << array.toHex().toStdString();
}
