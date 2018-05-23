/*
 * I2CDevice.h
 *
 *  Created on: 2017年4月16日
 *      Author: tom
 */

#ifndef I2CBUS_I2CDEVICE_H_
#define I2CBUS_I2CDEVICE_H_

#include "IO/IODevice.h"
#include <string>
#include <vector>
#include <memory>
#include "Thread/Mutex.h"
#include "Core/Object.h"

namespace Fx {

class I2CDevice: public IODevice {
public:
	class Driver;
	using DriverPtr = std::shared_ptr<Driver>;
	using DriverList = std::vector<DriverPtr>;

	I2CDevice(DriverPtr driver);

	virtual ~I2CDevice();
	virtual bool open(OpenMode mode);
	virtual bool open(int device, OpenMode mode = ReadWrite);
	virtual void close();
	virtual ByteArray read(uint32_t dataLen);
	virtual ByteArray read(uint32_t reg, uint32_t dataLen);
	virtual ByteArray read(uint32_t reg, WordLength addressLen, uint32_t dataLen);
	virtual ByteArray readAll();

	virtual bool write(const ByteArray &data);
	virtual bool write(uint32_t reg, const ByteArray &data);
	virtual bool write(uint32_t reg, WordLength addressLen, const ByteArray &data);

	virtual bool waitForReadyRead(int msecs);
protected:
	int device;
	DriverPtr  driver;
	uint32_t reg;
	WordLength regAddressLen;

	static DriverList driverList;
	static DriverPtr getDriverInstance(DriverPtr &driver);

	bool writeDevice();
	bool writeRegister();
public:
	int getDevice() const;
	void setDevice(int device);

	uint32_t getRegister() const;
	void setRegister(uint32_t reg);

	WordLength getRegisterAddressLength() const;
	void setRegisterAddressLength(WordLength addressLen);

	////////////////////////////////////
	// Drivers
public:
	class Driver {
	public:
		virtual std::string getId() const = 0;
		virtual bool open() = 0;
		virtual bool close() = 0;
		virtual bool start() = 0;
		virtual bool stop() = 0;
		virtual bool write(char byte) = 0;
		virtual char read() = 0;
		virtual bool waitForAck() = 0;
		virtual bool ack() = 0;
		virtual bool noAck() = 0;
		virtual void lock() = 0;
		virtual void unlock() = 0;
	};

	/////////////////////////////////////////////////////////////
	/// GPIO Driver

	class GPIODriver: public Driver {
	public:
		enum Speed{
		  Speed_10MHz = 1,
		  Speed_2MHz,
		  Speed_50MHz
		};

		enum Port {
			Port_A = 0,
			Port_B,
			Port_C,
			Port_D,
			Port_E,
			Port_F
		};

		static DriverPtr build(Port port, int sda, int scl, Speed speed = Speed_50MHz);

		virtual std::string getId() const;
		virtual bool open();
		virtual bool close();
		virtual bool start();
		virtual bool stop();
		virtual bool write(char byte);
		virtual char read();
		virtual bool waitForAck();
		virtual bool ack();
		virtual bool noAck();
		virtual void lock();
		virtual void unlock();
	protected:
		bool opened;
		int port;
		int sda;
		int scl;
		Speed speed;
		Mutex mutex;
		GPIODriver(Port port, int sda, int scl, Speed speed = Speed_50MHz);
		bool setInputMode();
		bool setOutputMode();
		void sleep(uint32_t us);
		void setBits(int pin);
		void resetBits(int pin);
		int readInputDataBit(int pin);
	};
};
using I2CDevicePtr = std::shared_ptr<I2CDevice>;
} /* namespace Fx */

#endif /* IO_SERIALBUS_I2CBUS_I2CDEVICE_H_ */
