#include "Config.h"

#if (PLATFORM == STM32F10x)
#include "stm32f10x_gpio.h"

#define GPIOx(_N)		((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))
#define PIN_MASK(_N)    (1 << (_N))
#define RCC_MASKx(_N)   (RCC_APB2Periph_GPIOA << (_N))

#include "../I2CDevice.h"
#include "Core/Debug.h"
#include <string>

using namespace std;
namespace Fx {

const int DELAY_TIME = 2;

I2CDevice::DriverPtr I2CDevice::GPIODriver::build(Port port, int sda, int scl,
		Speed speed) {
	return DriverPtr(new GPIODriver(port, sda, scl, speed));
}

I2CDevice::GPIODriver::GPIODriver(Port port, int sda, int scl, Speed speed) {
	opened = false;
	this->port = port;
	this->sda = sda;
	this->scl = scl;
	this->speed = speed;
}

string I2CDevice::GPIODriver::getId() const {
	return "GPID-" + ByteArray::number((int16_t) port).toHex().toStdString()
			+ ByteArray::number((int16_t) sda).toHex().toStdString()
			+ ByteArray::number((int16_t) scl).toHex().toStdString();
}
bool I2CDevice::GPIODriver::open() {
	// Enable GPIO Peripheral clock
	if (opened) {
		return true;
	}
	RCC_APB2PeriphClockCmd(RCC_MASKx(port), ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_MASK(sda) | PIN_MASK(scl);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef) speed;
	GPIO_Init(GPIOx(port), &GPIO_InitStructure);
	opened = true;
	return true;
}

bool I2CDevice::GPIODriver::close() {
	return true;
}

bool I2CDevice::GPIODriver::start() {
	setOutputMode();
	setBits(sda);
	setBits(scl);
	sleep(DELAY_TIME);
	resetBits(sda);
	sleep(DELAY_TIME);
	resetBits(scl);
	return true;
}

bool I2CDevice::GPIODriver::stop() {
	setOutputMode();
	resetBits(sda);
	resetBits(scl);
	sleep(DELAY_TIME);
	setBits(scl);
	sleep(DELAY_TIME);
	setBits(sda);
	return true;
}

bool I2CDevice::GPIODriver::write(char byte) {
	const int BITS = 8;
	setOutputMode();
	resetBits(scl);
	for (int i = 0; i < BITS; ++i) {
		sleep(DELAY_TIME);
		char mask = 0x80 >> i; //0x80 = 1000 0000(二进制)
		if (byte & mask) {
			setBits(sda);
		} else {
			resetBits(sda);
		}
		sleep(DELAY_TIME);
		setBits(scl);
		sleep(DELAY_TIME);
		resetBits(scl);
	}
	return true;
}

char I2CDevice::GPIODriver::read() {
	int data = 0;
	setInputMode();
	for (int i = 0; i < 8; ++i) {
		sleep(DELAY_TIME);
		setBits(scl);
		data <<= 1;
		sleep(DELAY_TIME);
		if (readInputDataBit(sda)) {
			data |= 0x01;
		}
		resetBits(scl);
	}
	return data;
}

bool I2CDevice::GPIODriver::waitForAck() {
	setInputMode();
	setBits(sda);
	setBits(scl);
	sleep(DELAY_TIME);
	if (readInputDataBit(sda)) {
		debug() << "Wait for ACK fail";
		resetBits(scl);
		return false;
	} else {
		resetBits(scl);
		return true;
	}
}

bool I2CDevice::GPIODriver::ack() {
	setOutputMode();
	resetBits(scl);
	resetBits(sda);
	sleep(DELAY_TIME);
	setBits(scl);
	sleep(DELAY_TIME);
	resetBits(scl);
	return true;
}

bool I2CDevice::GPIODriver::noAck() {
	setOutputMode();
	resetBits(scl);
	sleep(DELAY_TIME);
	setBits(scl);
	sleep(DELAY_TIME);
	resetBits(scl);
	return true;
}
void I2CDevice::GPIODriver::lock() {
	mutex.lock();
}
void I2CDevice::GPIODriver::unlock() {
	mutex.unlock();
}

void I2CDevice::GPIODriver::sleep(uint32_t us) {
	us = us * (SystemCoreClock / 1000000);
	while (us > 0) {
		--us;
	}
}

bool I2CDevice::GPIODriver::setInputMode() {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_MASK(sda);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef) speed;
	GPIO_Init(GPIOx(port), &GPIO_InitStructure);
	return true;
}

bool I2CDevice::GPIODriver::setOutputMode() {
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = PIN_MASK(sda);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = (GPIOSpeed_TypeDef) speed;
	GPIO_Init(GPIOx(port), &GPIO_InitStructure);
	return true;
}

void I2CDevice::GPIODriver::setBits(int pin) {
	GPIO_SetBits(GPIOx(port), PIN_MASK(pin));
}
void I2CDevice::GPIODriver::resetBits(int pin) {
	GPIO_ResetBits(GPIOx(port), PIN_MASK(pin));
}

int I2CDevice::GPIODriver::readInputDataBit(int pin) {
	return GPIO_ReadInputDataBit(GPIOx(port), PIN_MASK(pin));
}

}
#endif

#if (PLATFORM == STM32F405x)
#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

#define GPIOx(_N)       ((GPIO_TypeDef *)(GPIOA_BASE + (GPIOB_BASE-GPIOA_BASE)*(_N)))
#define PIN_MASK(_N)    (1 << (_N))
#define RCC_MASKx(_N)   (RCC_AHB1ENR_GPIOAEN << (_N))

#endif
