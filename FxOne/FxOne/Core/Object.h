/*
 * Object.h
 *
 * Created on: 2017年3月12日
 * Author: tom
 */

#ifndef CORE_OBJECT_H_
#define CORE_OBJECT_H_

namespace Fx {


class Object {
public:
	Object();
	virtual ~Object();
};

enum Endian{
	LittleEndian = 0,
	BigEndian
};

enum WordLength {
	WORD_8BITS = 1,
	WORD_16BITS,
	WORD_24BITS,
	WORD_32BITS
};
} /* namespace Fx */

#endif /* CORE_OBJECT_H_ */
