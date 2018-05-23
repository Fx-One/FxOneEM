/*
 * ByteArray.h
 *
 *  Created on: 2016年8月19日
 *      Author: tom
 */

#ifndef CORE_BYTEARRAY_H_
#define CORE_BYTEARRAY_H_

#include <string>
#include <vector>
#include "Object.h"
namespace Fx {
class ByteArray {
public:
	ByteArray();
	virtual ~ByteArray();

	void append(char ch);
	void append(const std::string &str);
	void append(const ByteArray &array);
	char operator[] (size_t i) const;

	void clear();
	static ByteArray number(int16_t num, Endian endian = BigEndian);
	static ByteArray number(int32_t num, Endian endian = BigEndian);
	static ByteArray number(int64_t num, Endian endian = BigEndian);
	static ByteArray number(float num, Endian endian = BigEndian);


	ByteArray left(size_t len) const;
	ByteArray right(size_t len) const;
	ByteArray mid(size_t pos, size_t len) const;

	bool startsWith(const ByteArray &ba) const;
	bool endsWith(const ByteArray &ba) const;

	ByteArray &remove(size_t pos, size_t len);

	std::string toStdString() const;
	ByteArray toHex() const;

	int16_t toInt16(Endian endian = BigEndian) const;
	int32_t toInt32(Endian endian = BigEndian) const;
	int64_t toInt64(Endian endian = BigEndian) const;
	float toFloat(Endian endian = BigEndian) const;

	bool operator==(const ByteArray &value) const;

protected:
	std::vector<char> data;
	static ByteArray number(int64_t num, int bytes, Endian endian = BigEndian);
	int64_t toInt(int bytes, Endian endian = BigEndian) const;
	//get & set
public:
	size_t size() const;
	bool isEmpty() const;
};
}
#endif /* CORE_BYTEARRAY_H_ */
