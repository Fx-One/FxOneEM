/*
 * I2CDevice.cpp
 *
 *  Created on: 2017年4月16日
 *      Author: tom
 */

#include "I2CDevice.h"
#include "Core/Debug.h"
#include <string>

using namespace std;
namespace Fx {

I2CDevice::DriverList I2CDevice::driverList;

const int WRITE_MASK = 0xFE;
const int READ_MASK = 0x01;

I2CDevice::DriverPtr I2CDevice::getDriverInstance(DriverPtr &driver)
{
	DriverPtr driverPtr = DriverPtr(NULL);
	for(uint i = 0; i < driverList.size(); ++i) {
		if(driverList[i]->getId() == driver->getId()) {
			driverPtr = driverList[i];
		}
	}

	if(driverPtr.get() == NULL) {
		driverList.push_back(driver);
		driverPtr = driver;
	}
	return driverPtr;
}

I2CDevice::I2CDevice(DriverPtr driver)
{
	regAddressLen = WORD_16BITS;
	this->driver = getDriverInstance(driver);
}

I2CDevice::~I2CDevice()
{

}

bool I2CDevice::open(OpenMode /*mode*/)
{
	if(this->driver == NULL) {
		return false;
	}
	if(!this->driver->open()) {
		//
		return false;
	}
	return true;
}

bool I2CDevice::open(int device, OpenMode mode)
{
	this->device = device;
	return open(mode);
}

void I2CDevice::close()
{

}

ByteArray I2CDevice::read(uint32_t dataLen)
{
	driver->lock();
	ByteArray data;
	driver->start();
	//写入Device地址
	if(!writeDevice()) {
		driver->unlock();
		return data;
	}

	//写入寄存器地址
	if(!writeRegister()) {
		driver->unlock();
		return data;
	}

	//发送读取信号
	driver->start();
	driver->write(device | READ_MASK);
	if(!driver->waitForAck()) {
		driver->stop();
		driver->unlock();
		return data;
	}

	//连续读取数据
	for(uint i = 0; i < dataLen; ++i) {
		data.append(driver->read());
		driver->ack();
	}

	driver->noAck(); //noAck代表读取结束
	driver->stop();	//发送操作结束信号
	driver->unlock();
	return data;
}

ByteArray I2CDevice::read(uint32_t reg, uint32_t dataLen)
{
	setRegister(reg);
	return read(dataLen);
}

ByteArray I2CDevice::read(uint32_t reg, WordLength addressLen, uint32_t dataLen)
{
	setRegister(reg);
	setRegisterAddressLength(addressLen);
	return read(dataLen);
}

ByteArray I2CDevice::readAll()
{
	return read(1);
}

bool I2CDevice::write(const ByteArray &data)
{
	if(driver == NULL) {
		debug() << "Driver is NULL";
		return false;
	}
	driver->lock();

	bool writeSuccessful = true;

	driver->start();
	//写入Device地址
	if(!writeDevice()) {
		driver->unlock();
		return false;
	}

	//写入寄存器地址
	if(!writeRegister()) {
		driver->unlock();
		return false;
	}

	//写入数据
	for(size_t i = 0; i < data.size(); ++i) {
		if(!driver->write(data[i])) {
			writeSuccessful = false;
			break;
		}

		if(!driver->waitForAck()) {
			writeSuccessful = false;
			break;
		}
	}

	driver->stop();

	driver->unlock();
	return writeSuccessful;
}

bool I2CDevice::write(uint32_t reg, const ByteArray &data)
{
	setRegister(reg);
	return write(data);
}
bool I2CDevice::write(uint32_t reg, WordLength addressLen, const ByteArray &data)
{
	setRegister(reg);
	setRegisterAddressLength(addressLen);
	return write(data);
}

bool I2CDevice::writeDevice()
{
	//写入Device地址
	driver->write(device & WRITE_MASK);

	if(!driver->waitForAck()) {
		driver->stop();
		return false;
	}

	return true;
}
bool I2CDevice::writeRegister()
{
	//写入寄存器地址(大端模式)
	for(int i = regAddressLen - 1; i >=0; --i) {
		char byte = (reg >> (i * 8)) & 0xFF;
		driver->write(byte);
		if(!driver->waitForAck()) {
			driver->stop();
			return false;
		}
	}
	return true;
}

bool I2CDevice::waitForReadyRead(int /*msecs*/)
{
	return true;
}

//get & set
int I2CDevice::getDevice() const
{
	return device;
}

void I2CDevice::setDevice(int device)
{
	this->device = device;
}

uint32_t I2CDevice::getRegister() const
{
	return reg;
}
void I2CDevice::setRegister(uint32_t reg)
{
	this->reg = reg;
}

WordLength I2CDevice::getRegisterAddressLength() const
{
	return regAddressLen;
}
void I2CDevice::setRegisterAddressLength(WordLength addressLen)
{
	this->regAddressLen = addressLen;
}
} /* namespace Fx */
