#pragma once

#include "Storage.h"
#include "stm32f7xx_hal_i2c.h"

class Logger
{
public:
	struct Record
	{
		void* ptr;
		uint32_t size;
	};
	Logger(Storage& _storage) : storage(_storage) {};
	bool log(void* data, uint32_t size, bool overwrite = false);
	void* getRecord(uint32_t num);
private:
	Storage& storage;
};
