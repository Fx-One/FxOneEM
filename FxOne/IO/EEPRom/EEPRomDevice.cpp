/*
 * EEPRomDevice.cpp
 *
 *  Created on: 2017年5月5日
 *      Author: tom
 */

#include "EEPRomDevice.h"
#include "Thread/MutexLocker.h"
#include "Core/Time.h"
namespace Fx {

EEPRomDevice::EEPRomDevice(I2CDevice::DriverPtr driver): device(driver) {
	pageSize = 128;
	pos = 0;
	size = 0;
}

EEPRomDevice::~EEPRomDevice() {

}

bool EEPRomDevice::open(OpenMode mode) {
	return device.open(mode);
}

bool EEPRomDevice::open(int device) {

	this->device.setDevice(device);
	return open(ReadWrite);
}

void EEPRomDevice::close() {

}
ByteArray EEPRomDevice::read(uint32_t maxLen) {
	MutexLocker locker(mutex);
	return device.read(pos, maxLen);
}
ByteArray EEPRomDevice::read(uint32_t pos, uint32_t maxLen) {
	MutexLocker locker(mutex);
	setPos(pos);
	return device.read(pos, maxLen);
}
ByteArray EEPRomDevice::readAll()
{
	MutexLocker locker(mutex);
	return device.read(pos, size - pos);
}

bool EEPRomDevice::splitAndWrite(const ByteArray &data)
{
	uint32_t reg = pos;
	uint32_t i = 0;
	while(i < data.size()) {
		uint32_t len = pageSize - (pos % pageSize);
		ByteArray part = data.mid(i, len);
		if(!device.write(reg, part)) {
			return false;
		}
		reg += len;
		i += len;
	}
	pos += data.size();
	Time::awkDelay(10);
	return true;
}

bool EEPRomDevice::write(const ByteArray &data)
{
	MutexLocker locker(mutex);
	return splitAndWrite(data);
}

bool EEPRomDevice::write(uint32_t pos, const ByteArray &data)
{
	MutexLocker locker(mutex);
	setPos(pos);
	return splitAndWrite(data);
}

bool EEPRomDevice::waitForReadyRead(int /*msecs*/)
{
	return true;
}

//get & set
int EEPRomDevice::getDevice() const {
	return device.getDevice();
}
void EEPRomDevice::setDevice(int device) {
	this->device.setDevice(device);
}

uint32_t EEPRomDevice::getSize() const {
	return size;
}

void EEPRomDevice::setSize(uint32_t size) {
	this->size = size;
}

uint32_t EEPRomDevice::getPos() const {
	return pos;
}

void EEPRomDevice::setPos(uint32_t pos) {
	this->pos = pos;
}

uint32_t EEPRomDevice::getPageSize() const {
	return pageSize;
}

void EEPRomDevice::setPageSize(uint32_t size) {
	this->pageSize = size;
}
WordLength EEPRomDevice::getRegisterAddressLength() const {
	return device.getRegisterAddressLength();
}

void EEPRomDevice::setRegisterAddressLength(WordLength addressLen) {
	device.setRegisterAddressLength(addressLen);
}

} /* namespace Fx */
