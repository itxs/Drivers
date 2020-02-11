#pragma once

#include "stdint.h"
#include "Types.h"

class Storage
{
public:
	enum Status
	{
		CMD_OK = 0,
		IF_ERROR = -1,
		BUSY_ERROR = -2,
		INIT_ERROR = -3,
		SIZE_ERROR = -4
	};
	void setCurrentPos(uint32_t pos);
	uint32_t getCurrentReadPos();
	uint32_t getCurrentWritePos();
	uint32_t getCapacity();
	virtual Status write(uint8_t* data, uint16_t size, uint16_t address = 0xFFFF) = 0;
	virtual Status read(uint8_t* data, uint16_t size, uint16_t address = 0xFFFF) = 0;
	virtual Status erase() = 0;
protected:
	uint32_t memCapacity = 0;
	uint32_t currentReadPos = 0;
	uint32_t currentWritePos = 0;
};
