/*
 * EEPRomDevice.h
 *
 *  Created on: 2017年5月5日
 *      Author: tom
 */

#ifndef IO_EEPROM_EEPROMDEVICE_H_
#define IO_EEPROM_EEPROMDEVICE_H_

#include "IO/IODevice.h"
#include "IO/SerialBus/I2CBus/I2CDevice.h"
#include "Thread/Mutex.h"

namespace Fx {

class EEPRomDevice: public virtual IODevice {
public:
	EEPRomDevice(I2CDevice::DriverPtr driver);
	virtual ~EEPRomDevice();
	virtual bool open(OpenMode mode = ReadWrite);
	virtual bool open(int device);
	virtual void close();
	virtual ByteArray read(uint32_t maxLen);
	virtual ByteArray read(uint32_t pos, uint32_t maxLen);
	virtual ByteArray readAll();
	virtual bool write(const ByteArray &data);
	virtual bool write(uint32_t pos, const ByteArray &data);
	virtual bool waitForReadyRead(int msecs);

protected:
	I2CDevice device;
	uint32_t pos;
	uint32_t pageSize;
	uint32_t size;
	Mutex mutex;
	bool splitAndWrite(const ByteArray &data);
public:
	int getDevice() const;
	void setDevice(int device);

	uint32_t getSize() const;
	void setSize(uint32_t size);

	uint32_t getPos() const;
	void setPos(uint32_t pos);

	uint32_t getPageSize() const;
	void setPageSize(uint32_t size);

	WordLength getRegisterAddressLength() const;
	void setRegisterAddressLength(WordLength addressLen);
};

} /* namespace Fx */

#endif /* IO_EEPROM_EEPROMDEVICE_H_ */
