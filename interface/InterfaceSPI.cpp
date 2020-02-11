#include "InterfaceSPI.h"
#include "stm32f7xx_hal.h"

InterfaceSPI::InterfaceSPI()
{
	__SPI2_CLK_ENABLE();

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_1 | GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	deselect();
}

bool InterfaceSPI::init(void* const instance, IRQn_Type irq, uint8_t irqPriority)
{
	hspi.Instance = (SPI_TypeDef*)instance;
	hspi.Init.Mode = SPI_MODE_MASTER;
	hspi.Init.Direction = SPI_DIRECTION_2LINES;
	hspi.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi.Init.NSS = SPI_NSS_SOFT;
	hspi.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
	hspi.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi.Init.CRCPolynomial = 7;
	hspi.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
	hspi.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;

	if (HAL_SPI_Init(&hspi) != HAL_OK)
	{
		errorHandler();
		return true;
	}
	else
	{
		//setIRQEnabled(getIRQn(instance));
		/* Peripheral interrupt init */
		//HAL_NVIC_SetPriority(SPI2_IRQn, 0, 0);
		//HAL_NVIC_EnableIRQ(SPI2_IRQn);
		return false;
	}
}

bool InterfaceSPI::deinit(void* const instance)
{
	return (HAL_SPI_DeInit(&hspi) == HAL_OK);
}

int32_t InterfaceSPI::read(uint8_t* data, uint32_t size, bool unblocking)
{
	uint8_t status = HAL_OK;

	if (unblocking)
		status = HAL_SPI_Receive_IT(&hspi, data, size);
	else
		status = HAL_SPI_Receive(&hspi, data, size, timeout_ms);

	if (status == HAL_OK)
		return hspi.RxXferSize - hspi.RxXferCount;
	else
		return IF_COMM_ERROR;
}

int32_t InterfaceSPI::write(uint8_t* data, uint32_t size, bool unblocking)
{
	uint8_t status = HAL_OK;

	if (unblocking)
		status = HAL_SPI_Transmit_IT(&hspi, data, size);
	else
		status = HAL_SPI_Transmit(&hspi, data, size, timeout_ms);

	if (status == HAL_OK)
		return hspi.TxXferSize - hspi.TxXferCount;
	else
		return IF_COMM_ERROR;
}

int32_t InterfaceSPI::readwrite(uint8_t* txData, uint8_t* rxData, uint32_t size, bool unblocking)
{
	uint8_t status = HAL_OK;

	if (unblocking)
		status = HAL_SPI_TransmitReceive_IT(&hspi, txData, rxData, size);
	else
		status = HAL_SPI_TransmitReceive(&hspi, txData, rxData, size, timeout_ms);

	if (status == HAL_OK)
		return hspi.TxXferSize - hspi.TxXferCount;
	else
		return IF_COMM_ERROR;
}

int32_t InterfaceSPI::writeByte(uint8_t data, bool unblocking)
{
	uint8_t status = HAL_OK;
	singlePacket = ((uint32_t)data) << 24; // shift data due to Big-endian format

	if (unblocking)
		status = HAL_SPI_Transmit_IT(&hspi, (uint8_t*)&singlePacket, 1);
	else
		status = HAL_SPI_Transmit(&hspi, (uint8_t*)&data, 1, timeout_ms);

	if (status == HAL_OK)
		return hspi.TxXferCount;
	else
		return IF_COMM_ERROR;
}

int32_t InterfaceSPI::readByte(uint8_t* data, bool unblocking)
{
	uint8_t status = HAL_OK;

	if (unblocking)
		status = HAL_SPI_Receive_IT(&hspi, data, 1);
	else
		status = HAL_SPI_Receive(&hspi, data, 1, timeout_ms);

	if (status == HAL_OK)
		return hspi.TxXferCount;
	else
		return IF_COMM_ERROR;
}


int32_t InterfaceSPI::writeShort(uint16_t data, bool unblocking)
{
	uint8_t status = HAL_OK;
	singlePacket = ((uint32_t)data) << 16; // shift data due to Big-endian format

	if (unblocking)
		status = HAL_SPI_Transmit_IT(&hspi, (uint8_t*)&singlePacket, 2);
	else
		status = HAL_SPI_Transmit(&hspi, (uint8_t*)&data, 2, timeout_ms);

	if (status == HAL_OK)
		return hspi.TxXferCount;
	else
		return IF_COMM_ERROR;
}

int32_t InterfaceSPI::readShort(uint16_t* data, bool unblocking)
{
	uint8_t status = HAL_OK;

	if (unblocking)
		status = HAL_SPI_Receive_IT(&hspi, (uint8_t*)data, 2);
	else
		status = HAL_SPI_Receive(&hspi, (uint8_t*)data, 2, timeout_ms);

	if (status == HAL_OK)
		return hspi.TxXferCount;
	else
		return IF_COMM_ERROR;
}

int32_t InterfaceSPI::writeQuad(uint32_t data, bool unblocking)
{
	uint8_t status = HAL_OK;
	singlePacket = data;

	if (unblocking)
		status = HAL_SPI_Transmit_IT(&hspi, (uint8_t*)&singlePacket, 4);
	else
		status = HAL_SPI_Transmit(&hspi, (uint8_t*)&data, 4, timeout_ms);

	if (status == HAL_OK)
		return hspi.TxXferCount;
	else
		return IF_COMM_ERROR;
}

int32_t InterfaceSPI::readQuad(uint32_t* data, bool unblocking)
{
	uint8_t status = HAL_OK;

	if (unblocking)
		status = HAL_SPI_Receive_IT(&hspi, (uint8_t*)data, 4);
	else
		status = HAL_SPI_Receive(&hspi, (uint8_t*)data, 4, timeout_ms);

	if (status == HAL_OK)
		return hspi.TxXferCount;
	else
		return IF_COMM_ERROR;
}

void InterfaceSPI::select()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
}

void InterfaceSPI::deselect()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
}

bool InterfaceSPI::isBusy()
{
	HAL_SPI_StateTypeDef state = HAL_SPI_GetState(&hspi);
	bool instanceBusy = __HAL_SPI_GET_FLAG(&hspi, SPI_SR_BSY) || !(__HAL_SPI_GET_FLAG(&hspi, SPI_SR_TXE));
	return instanceBusy || (state == HAL_SPI_STATE_BUSY) || (state == HAL_SPI_STATE_BUSY_RX) || (state == HAL_SPI_STATE_BUSY_TX) || (state == HAL_SPI_STATE_BUSY_TX_RX);
}

bool InterfaceSPI::waitForFree(uint32_t timeout)
{
	if (isBusy())
	{
		uint32_t tickstart = HAL_GetTick();
		while ((HAL_GetTick() - tickstart) < timeout)
		{
			if (!isBusy())
				return false;
		}

		if ((0xFFFFFFFFU - tickstart) < timeout)
		{
			while (HAL_GetTick() < (timeout - (0xFFFFFFFFU - tickstart)))
			{
				if (!isBusy())
					return false;
			}
		}
		return true;
	}
}

void InterfaceSPI::handler()
{
	HAL_SPI_IRQHandler(&hspi);
}

void InterfaceSPI::errorHandler()
{
	HAL_SPI_ErrorCallback(&hspi);
}
