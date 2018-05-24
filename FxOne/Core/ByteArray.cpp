/*
 * ByteArray.cpp
 *
 * Created on: 2016年8月19日
 * Author: tom
 */

#include "Core/ByteArray.h"
using namespace std;
namespace Fx {
ByteArray::ByteArray() {

}

ByteArray::~ByteArray() {

}

void ByteArray::append(char ch)
{
	data.push_back(ch);
}
void ByteArray::append(const string &str)
{
	for(unsigned int i = 0; i < str.size(); ++i) {
		data.push_back(str[i]);
	}
}

void ByteArray::append(const ByteArray &array)
{
	for(size_t i = 0; i < array.size(); ++i) {
		append(array[i]);
	}
}

char ByteArray::operator[](size_t i) const {
	return data[i];
}

void ByteArray::clear() {
	data.clear();
}

ByteArray &ByteArray::remove(size_t pos, size_t len) {
	vector<char>::iterator first = data.begin();
	vector<char>::iterator last;
	first += pos;
	if(first > data.end() || first < data.begin()) {
		return *this;
	}
	last = first + len;
	//防止越界
	if(last > data.end()) {
		last = data.end();
	}
	data.erase(first, last);
	return *this;
}

ByteArray ByteArray::number(int16_t num, Endian endian)
{
	return number(num, 2, endian);
}
ByteArray ByteArray::number(int32_t num, Endian endian)
{
	return number(num, 4,endian);
}

ByteArray ByteArray::number(int64_t num, Endian endian)
{
	return number(num, 8,endian);
}

ByteArray ByteArray::number(int64_t num, int bytes, Endian endian)
{
	ByteArray array;
	if(endian == BigEndian) {
		for(int i = bytes - 1; i >=0 ; --i) {
			char byte = (num >> (8*i)) & 0xFF;
			array.append(byte);
		}
	}
	else {
		for(int i = 0; i < bytes; ++i) {
			char byte = (num >> (8*i)) & 0xFF;
			array.append(byte);
		}
	}

	return array;
}

ByteArray ByteArray::number(float num, Endian endian)
{
	ByteArray array;
	int bytes = 4;
	uint32_t number = (uint32_t) num;
	if(endian == BigEndian) {
		for(int i=0; i<bytes; ++i) {
			char byte = (number >> (8*i)) & 0xFF;
			array.append(byte);
		}
	}
	else {
		for(int i=0; i<bytes; ++i) {
			char byte = (number >> (8*i)) & 0xFF;
			array.append(byte);
		}
	}

	return array;
}


ByteArray ByteArray::left(size_t len) const {
	ByteArray array;
	for(size_t i=0; i < len && i < data.size(); ++i) {
		array.append(data[i]);
	}
	return array;
}
ByteArray ByteArray::right(size_t len) const {
	ByteArray array;
	int first = data.size() - len;
	if(first < 0) {
		first = 0;
	}
	for(size_t i = first; i < data.size(); ++i) {
		array.append(data[i]);
	}
	return array;
}
ByteArray ByteArray::mid(size_t pos, size_t len) const {
	ByteArray array;
	for(size_t i = pos;  i < len && i < data.size(); ++i) {
		array.append(data[i]);
	}
	return array;
}

bool ByteArray::startsWith(const ByteArray &ba) const {
	bool result = true;
	if(ba.size() > data.size()) {
		return false;
	}
	for(size_t i = 0; i < ba.size(); ++i) {
		if(data[i] != ba[i]) {
			result = false;
		}
	}
	return result;
}
bool ByteArray::endsWith(const ByteArray &ba) const {
	bool result = true;
	if(ba.size() > data.size()) {
		return false;
	}
	for(size_t i = 1; i <= ba.size(); ++i) {
		if(data[data.size() - i] != ba[ba.size() - i]) {
			result = false;
		}
	}
	return result;
}

string ByteArray::toStdString() const {
	string str;
	for(unsigned int i = 0; i < data.size(); ++i) {
		str.push_back(data[i]);
	}
	return str;
}
ByteArray ByteArray::toHex() const {
	char hexTable[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
	ByteArray hexArray;
	for(unsigned int i = 0; i < data.size(); ++i) {
		char byte = data[i];
		char hiOrder = hexTable[byte >> 4];
		char lowOrder = hexTable[byte & 0xF];
		hexArray.append(hiOrder);
		hexArray.append(lowOrder);
	}
	return hexArray;
}

int16_t ByteArray::toInt16(Endian endian) const
{
	return (int16_t) toInt(2, endian);
}

int32_t ByteArray::toInt32(Endian endian) const
{
	return (int32_t) toInt(4, endian);
}

int64_t ByteArray::toInt64(Endian endian) const
{
	return toInt(8, endian);
}

int64_t ByteArray::toInt(int bytes, Endian endian) const
{
	uint64_t value = 0;
	int count = bytes < (int)data.size() ? bytes : data.size();
	if(endian == BigEndian) {
		for(int i = 0; i < count; ++i) {
			uint64_t byte = data[i];
			value = value | (byte << ((count - i - 1) * 8));
		}
	}
	else {
		for(int i = 0; i < count; ++i) {
			uint64_t byte = data[i];
			value = value | (byte << (i * 8));
		}
	}
	return (int64_t)value;
}

float ByteArray::toFloat(Endian endian) const
{
	uint32_t value = 0;
	int bytes = 4;
	int count = bytes < (int)data.size() ? bytes : data.size();
	if(endian == BigEndian) {
		for(int i = 0; i < count; ++i) {
			uint32_t byte = data[i];
			value = value | (byte << ((count - i - 1) * 8));
		}
	}
	else {
		for(int i = 0; i < count; ++i) {
			uint32_t byte = data[i];
			value = value | (byte << (i * 8));
		}
	}
	return (float)value;
}

bool ByteArray::operator==(const ByteArray &value) const {
	return this->data == value.data;
}

//get & set
size_t ByteArray::size() const {
	return data.size();
}

bool ByteArray::isEmpty() const {
	return data.size() == 0;
}
}
