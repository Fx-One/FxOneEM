/*
 * SerialPort.h
 *
 *  Created on: 2016年8月9日
 *      Author: tom
 */

#ifndef IO_SERIALPORT_H_
#define IO_SERIALPORT_H_
#include "Core/ByteArray.h"
#include "IO/IODevice.h"
namespace Fx {
class SerialPort:public IODevice {
public:
    enum Direction  {
        Input = 1,
        Output = 2,
        AllDirections = Input | Output
    };
    enum BaudRate {
        Baud1200 = 1200,
        Baud2400 = 2400,
        Baud4800 = 4800,
        Baud9600 = 9600,
        Baud19200 = 19200,
        Baud38400 = 38400,
        Baud57600 = 57600,
        Baud115200 = 115200,
        UnknownBaud = -1
    };

    enum DataBits {
        Data5 = 5,
        Data6 = 6,
        Data7 = 7,
        Data8 = 8,
        UnknownDataBits = -1
    };

    enum Parity {
        NoParity = 0,
        EvenParity = 2,
        OddParity = 3,
        SpaceParity = 4,
        MarkParity = 5,
        UnknownParity = -1
    };

    enum StopBits {
        OneStop = 1,
        OneAndHalfStop = 3,
        TwoStop = 2,
        UnknownStopBits = -1
    };

    enum FlowControl {
        NoFlowControl,
        HardwareControl,
        SoftwareControl,
        UnknownFlowControl = -1
    };

	SerialPort();
	virtual ~SerialPort();

	virtual bool open(OpenMode mode);
	virtual void close();
	virtual ByteArray read(uint32_t maxLen);
	virtual ByteArray readAll();
	virtual bool write(const ByteArray &data);
	virtual bool waitForReadyRead(int msecs);

protected:
	BaudRate mBaudRate;
	DataBits mDataBits;
	Parity mParity;
	StopBits mStopBits;
	FlowControl mFlowControl;
};
}
#endif /* IO_SERIALPORT_H_ */
