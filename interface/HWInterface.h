#pragma once

#include "Types.h"
#include "stm32f7xx.h"
#include "stm32f7xx_hal.h"

constexpr auto IF_OK = (0);
constexpr auto IF_COMM_ERROR = (-1);
constexpr auto IF_BUSY = (-2);
constexpr auto IF_INIT_ERROR = (-3);

constexpr IRQn_Type getIRQn(void* const instance)
{
	if (instance == SPI1)
		return SPI1_IRQn;
	else if (instance == SPI2)
		return SPI2_IRQn;
	else if (instance == SPI3)
		return SPI3_IRQn;
	if (instance == I2C1)
		return I2C1_EV_IRQn;
	else if (instance == I2C2)
		return I2C2_EV_IRQn;
	else if (instance == I2C3)
		return I2C3_EV_IRQn;
	else if (instance == I2C4)
		return I2C4_EV_IRQn;
}

//bool waitFor(uint32_t timeout, bool(*getFlag)())
//{
//	uint32_t tickstart = HAL_GetTick();
//	while ((HAL_GetTick() - tickstart) < timeout)
//	{
//		if (!getFlag())
//			return false;
//	}
//
//	if ((0xFFFFFFFFU - tickstart) < timeout)
//	{
//		while (HAL_GetTick() < (timeout - (0xFFFFFFFFU - tickstart)))
//		{
//			if (!getFlag())
//				return false;
//		}
//	}
//	return true;
//}

class HWInterface
{
public:
	virtual bool init(void* const instance, IRQn_Type irq, uint8_t irqPriority) = 0;
	virtual bool deinit(void* const instance) = 0;
	virtual int32_t read(uint8_t* data, uint32_t size, bool unblocking = false) = 0;
	virtual int32_t write(uint8_t* data, uint32_t size, bool unblocking = false) = 0;
	virtual bool isBusy() = 0;
	virtual bool waitForFree(uint32_t timeout) = 0;
	virtual void handler() = 0;
	virtual void errorHandler() = 0;
	void setTimeout(uint32_t timeout)
	{
		timeout_ms = timeout;
	}
	void setIRQEnabled(const bool state)
	{
		if (irqn != NonMaskableInt_IRQn)
		{
			if (state)
				NVIC_EnableIRQ(irqn);
			else
				NVIC_DisableIRQ(irqn);
		}
	}
	void setIRQPriority(uint8_t priority)
	{
		if (irqn != NonMaskableInt_IRQn)
			NVIC_SetPriority(irqn, priority);
	}

private:
	IRQn_Type irqn = NonMaskableInt_IRQn;
protected:
	uint32_t timeout_ms = 20;
};
