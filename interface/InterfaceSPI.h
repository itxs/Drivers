#pragma once

#include "HWInterface.h"
#include "stm32f7xx_hal_spi.h"

class InterfaceSPI : public HWInterface
{
public:
	InterfaceSPI();
	bool init(void* const instance, IRQn_Type irq, uint8_t irqPriority);
	bool deinit(void* const instance);
	int32_t read(uint8_t* data, uint32_t size, bool unblocking = false);
	int32_t write(uint8_t* data, uint32_t size, bool unblocking = false);
	int32_t readwrite(uint8_t* txData, uint8_t* rxData, uint32_t size, bool unblocking = false);
	int32_t writeByte(uint8_t data, bool unblocking = false);
	int32_t readByte(uint8_t* data, bool unblocking = false);
	int32_t writeShort(uint16_t data, bool unblocking = false);
	int32_t readShort(uint16_t* data, bool unblocking = false);
	int32_t writeQuad(uint32_t data, bool unblocking = false);
	int32_t readQuad(uint32_t* data, bool unblocking = false);
	bool isBusy();
	bool waitForFree(uint32_t timeout);
	void handler();
	void errorHandler();
	void select();
	void deselect();
private:
	SPI_HandleTypeDef hspi;
	uint32_t singlePacket;
};
