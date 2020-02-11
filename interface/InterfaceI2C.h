#pragma once

#include "HWInterface.h"
#include "stm32f7xx_hal_i2c.h"

class InterfaceI2C : public HWInterface
{
public:
	InterfaceI2C();
	bool init(void* const instance, IRQn_Type irq, uint8_t irqPriority);
	bool deinit(void* const instance);
	int32_t read(uint8_t* data, uint32_t size, bool unblocking = false);
	int32_t write(uint8_t* data, uint32_t size, bool unblocking = false);
	bool isBusy();
	bool waitForFree(uint32_t timeout);
	void handler();
	void errorHandler();
private:
	I2C_HandleTypeDef hi2c;
};
