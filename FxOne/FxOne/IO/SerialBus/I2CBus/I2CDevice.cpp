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

/////////////////////////////////////////////////////////////
/// Driver

#if defined (STM32F10X_HD) || (defined STM32F10X_XL) || (defined STM32F10X_HD_VL)
#include "stm32f10x_gpio.h"

#define GPIOx(_N)		((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))
#define PIN_MASK(_N)    (1 << (_N))
#define RCC_MASKx(_N)   (RCC_APB2Periph_GPIOA << (_N))
#endif

#if defined(STM32F405xx) || defined(STM32F415xx) || defined(STM32F407xx)|| defined(STM32F417xx)
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define GPIOx(_N)       ((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))
#define PIN_MASK(_N)    (1 << (_N))
#define RCC_MASKx(_N)   (RCC_AHB1ENR_GPIOAEN << (_N))

#endif

const int DELAY_TIME = 2;

I2CDevice::DriverPtr I2CDevice::GPIODriver::build(Port port, int sda, int scl, Speed speed)
{
	return DriverPtr(new GPIODriver(port, sda, scl, speed));
}

I2CDevice::GPIODriver::GPIODriver(Port port, int sda, int scl, Speed speed)
{
	opened = false;
	this->port = port;
	this->sda = sda;
	this->scl = scl;
	this->speed = speed;
}

string I2CDevice::GPIODriver::getId() const
{
	return "GPID-" + ByteArray::number((int16_t)port).toHex().toStdString()
			+ ByteArray::number((int16_t)sda).toHex().toStdString()
			+ ByteArray::number((int16_t)scl).toHex().toStdString();
}
bool I2CDevice::GPIODriver::open()
{
	// Enable GPIO Peripheral clock
	if(opened) {
		return true;
	}
	RCC_APB2PeriphClockCmd(RCC_MASKx(port), ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_MASK(sda) | PIN_MASK(scl);
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)speed;
  	GPIO_Init(GPIOx(port), &GPIO_InitStructure);
  	opened = true;
  	return true;
}

bool I2CDevice::GPIODriver::close()
{
	return true;
}

bool I2CDevice::GPIODriver::start()
{
	setOutputMode();
	setBits(sda);
	setBits(scl);
	sleep(DELAY_TIME);
	resetBits(sda);
	sleep(DELAY_TIME);
	resetBits(scl);
	return true;
}

bool I2CDevice::GPIODriver::stop()
{
	setOutputMode();
	resetBits(sda);
	resetBits(scl);
	sleep(DELAY_TIME);
	setBits(scl);
	sleep(DELAY_TIME);
	setBits(sda);
	return true;
}

bool I2CDevice::GPIODriver::write(char byte)
{
	const int BITS = 8;
	setOutputMode();
	resetBits(scl);
	for(int i = 0; i < BITS; ++i) {
		sleep(DELAY_TIME);
		char mask = 0x80 >> i; //0x80 = 1000 0000(二进制)
		if(byte & mask) {
			setBits(sda);
		}else {
			resetBits(sda);
		}
		sleep(DELAY_TIME);
		setBits(scl);
		sleep(DELAY_TIME);
		resetBits(scl);
	}
	return true;
}

char I2CDevice::GPIODriver::read()
{
	int data = 0;
	setInputMode();
	for(int i = 0; i < 8 ; ++i)	{
		sleep(DELAY_TIME);
		setBits(scl);
		data<<=1;
		sleep(DELAY_TIME);
		if(readInputDataBit(sda)) {
			data |= 0x01 ;
		}
		resetBits(scl);
	}
	return data;
}

bool I2CDevice::GPIODriver::waitForAck()
{
	setInputMode();
	setBits(sda);
	setBits(scl);
	sleep(DELAY_TIME);
	if(readInputDataBit(sda)) {
		debug() << "Wait for ACK fail";
		resetBits(scl);
		return false;
	} else {
		resetBits(scl);
		return true;
	}
}

bool I2CDevice::GPIODriver::ack()
{
	setOutputMode();
	resetBits(scl);
	resetBits(sda);
	sleep(DELAY_TIME);
	setBits(scl);
	sleep(DELAY_TIME);
	resetBits(scl);
	return true;
}

bool I2CDevice::GPIODriver::noAck()
{
	setOutputMode();
	resetBits(scl);
	sleep(DELAY_TIME);
	setBits(scl);
	sleep(DELAY_TIME);
	resetBits(scl);
	return true;
}
void I2CDevice::GPIODriver::lock()
{
	mutex.lock();
}
void I2CDevice::GPIODriver::unlock()
{
	mutex.unlock();
}

void I2CDevice::GPIODriver::sleep(uint32_t us) {
	us = us * (SystemCoreClock / 1000000);
	while(us > 0) {
		--us;
	}
}

bool I2CDevice::GPIODriver::setInputMode()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_MASK(sda);
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
 	GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)speed;
  	GPIO_Init(GPIOx(port), &GPIO_InitStructure);
  	return true;
}

bool I2CDevice::GPIODriver::setOutputMode()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_MASK(sda);
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
 	GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef)speed;
  	GPIO_Init(GPIOx(port), &GPIO_InitStructure);
  	return true;
}

void I2CDevice::GPIODriver::setBits(int pin)
{
	GPIO_SetBits(GPIOx(port), PIN_MASK(pin));
}
void I2CDevice::GPIODriver::resetBits(int pin)
{
	GPIO_ResetBits(GPIOx(port), PIN_MASK(pin));
}

int I2CDevice::GPIODriver::readInputDataBit(int pin)
{
	return GPIO_ReadInputDataBit(GPIOx(port), PIN_MASK(pin));
}

} /* namespace Fx */
