/*
 * IODevice.h
 *
 *  Created on: 2016年8月9日
 *      Author: tom
 */

#ifndef IO_IODEVICE_H_
#define IO_IODEVICE_H_
#include <cstdint>
#include "Core/ByteArray.h"
#include "Core/Object.h"
using namespace std;
namespace Fx {
class IODevice : public Object {
public:
    enum OpenMode {
        ReadOnly = 0x0001,
        WriteOnly = 0x0002,
        ReadWrite = ReadOnly | WriteOnly,
        Append = 0x0004,
        Truncate = 0x0008,
        Text = 0x0010
    };

	virtual ~IODevice();
	virtual bool open(OpenMode mode) = 0;
	virtual void close() = 0;
	virtual ByteArray read(uint32_t maxLen) = 0;
	virtual ByteArray readAll() = 0;
	virtual bool write(const ByteArray &data) = 0;

	virtual bool waitForReadyRead(int msecs) = 0;

};
}
#endif /* IO_IODEVICE_H_ */
