#include "gt911.h"
#include "touchgfx/hal/Types.hpp"

void I2C_Init();
void I2C_Deinit();

uint8_t GT911_Init(GT911* gt911)
{
	if (gt911 && gt911->hi2c && gt911->i2cPort && gt911->intPort && gt911->resetPort)
	{
		GT911_SetOutputMax(gt911, 800, 480);
		GT911_UpdateConfig(gt911);
		return 0;
	}
	else
		return 1;
}

uint8_t GT911_DeInit(GT911* gt911)
{
	return 0;
}

void GT911_Reset(GT911* gt911)
{
	HAL_GPIO_WritePin(gt911->resetPort, 1 << (uint32_t)gt911->resetPin, GPIO_PIN_RESET);
	I2C_Deinit();
	HAL_Delay(10);
	I2C_Init();
	HAL_GPIO_WritePin(gt911->resetPort, 1 << (uint32_t)gt911->resetPin, GPIO_PIN_SET);
	HAL_Delay(10);
	__HAL_GPIO_EXTI_CLEAR_IT(1 << (uint32_t)gt911->intPin);
}

void GT911_Wakeup(GT911* gt911)
{
	gt911->intPort->MODER |= 1 << ((uint32_t)gt911->intPin * 4);
	gt911->intPort->OTYPER &= ~(1 << (uint32_t)gt911->intPin);
	gt911->intPort->BSRR |= 1 << (uint32_t)gt911->intPin;
	HAL_Delay(3);
	gt911->intPort->BSRR |= 1 << ((uint32_t)gt911->intPin + 16);
	gt911->intPort->MODER &= ~(1 << ((uint32_t)gt911->intPin * 4));
}

void GT911_SendCmd(GT911* gt911, GT911_Cmd cmd)
{
	uint8_t cmd_data = (uint8_t)cmd;
	if (HAL_I2C_Mem_Write(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_COMMAND, 2, &cmd_data, 1, GT911_I2C_TIMEOUT_MS) != HAL_OK)
		GT911_Reset(gt911);
}

void GT911_ClearStatusReg(GT911* gt911)
{
	uint8_t cmd_data = 0;
	if (HAL_I2C_Mem_Write(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_STATUS, 2, &cmd_data, 1, GT911_I2C_TIMEOUT_MS) != HAL_OK)
		GT911_Reset(gt911);
}

uint8_t GT911_ReadCoord(GT911* gt911, GT911_OutputData* data)
{
	uint8_t status_reg = 0;
	data->pointsNumber = 0;
	
	// Проверяем наличие касания
	if (HAL_I2C_Mem_Read(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_STATUS, 2, &status_reg, 1, GT911_I2C_TIMEOUT_MS) != HAL_OK)
	{
		GT911_Reset(gt911);
		return 0;
	}
	
	// Если есть касание, то считываем все координаты
	if (status_reg & GT911_REG_STATUS_DATA_VALID_MASK)
	{
		HAL_I2C_Mem_Read(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_TRACK_ID1, 2, (uint8_t*)data->points,
						gt911->allowedTouchNumber * GT911_TOUCH_DATA_SIZE, GT911_I2C_TIMEOUT_MS);
		GT911_ClearStatusReg(gt911);

		// Проверка на валидность id касания (у первого касания id = 0), иногда тач сам срабатывает без касания с некорректным id
		if (data->points[0].trackID == 0)
		{
			data->pointsNumber = status_reg & GT911_REG_STATUS_TOUCH_POINTS_NUM_MASK;
			data->largeDetected = (status_reg & GT911_REG_STATUS_LARGE_DETECT_MASK) != 0;
			return data->pointsNumber;
		}
	}
	return 0;
}

void GT911_GetOutputMax(GT911* gt911, uint16_t* x_max, uint16_t* y_max)
{
	uint16_t buffer[2] = { 0, 0 };
	HAL_I2C_Mem_Read(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_X_OUT_MAX_L, 2, (uint8_t*)buffer, 4, GT911_I2C_TIMEOUT_MS);
	*x_max = buffer[0];
	*y_max = buffer[1];
}

void GT911_SetOutputMax(GT911* gt911, uint16_t x_max, uint16_t y_max)
{
	uint8_t buffer[] = { (uint8_t)(x_max & 0xFF), (uint8_t)((x_max >> 8) & 0xFF), (uint8_t)(y_max & 0xFF), (uint8_t)((y_max >> 8) & 0xFF) };
	HAL_I2C_Mem_Write(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_X_OUT_MAX_L, 2, buffer, sizeof(buffer), GT911_I2C_TIMEOUT_MS);
}

void GT911_UpdateConfig(GT911* gt911)
{
	uint8_t config[0x80FE - 0x8047];
	HAL_I2C_Mem_Read(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_CFG_VER, 2, config, sizeof(config), GT911_I2C_TIMEOUT_MS);

	uint8_t buffer = 0;
	for (uint16_t i = 0; i < sizeof(config) - 2; i++)
	{
		buffer += config[i];
	}
	buffer = ~buffer + 1;
	HAL_I2C_Mem_Write(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_CONFIG_CHKSUM, 2, &buffer, 1, GT911_I2C_TIMEOUT_MS);
	buffer = 1;
	HAL_I2C_Mem_Write(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_CONFIG_FRESH, 2, &buffer, 1, GT911_I2C_TIMEOUT_MS);
}

void GT911_GetPID(GT911* gt911, uint8_t* buffer)
{
	HAL_I2C_Mem_Read(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_PID_BYTE1_ASCII, 2, buffer, 4, GT911_I2C_TIMEOUT_MS);
}

void GT911_GetFWVersion(GT911* gt911, uint16_t* buffer)
{
	HAL_I2C_Mem_Read(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_FW_VER_HEX_L, 2, (uint8_t*)buffer, 2, GT911_I2C_TIMEOUT_MS);
}

void GT911_GetRefreshRate(GT911* gt911, uint8_t* rate)
{
	HAL_I2C_Mem_Read(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_REFRESH_RATE, 2, rate, 1, GT911_I2C_TIMEOUT_MS);
}

void GT911_SetRefreshRate(GT911* gt911, uint8_t rate)
{
	HAL_I2C_Mem_Write(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_REFRESH_RATE, 2, &rate, 1, GT911_I2C_TIMEOUT_MS);
}

void GT911_GetTouchThreshold(GT911* gt911, uint8_t* threshold_start, uint8_t* threshold_end)
{
	uint8_t buffer[2] = { 0, 0 };
	if (HAL_I2C_Mem_Read(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_SCREEN_TOUCH_LVL, 2, buffer, 2, GT911_I2C_TIMEOUT_MS) != HAL_OK)
	{
		GT911_Reset(gt911);
		return;
	}
	*threshold_start = buffer[0];
	*threshold_end = buffer[1];
}

void GT911_SetTouchThreshold(GT911* gt911, uint8_t threshold_start, uint8_t threshold_end)
{
	uint8_t buffer[2] = { 0, 0 };
	buffer[0] = threshold_start;
	buffer[1] = threshold_end;
	if (HAL_I2C_Mem_Write(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_SCREEN_TOUCH_LVL, 2, buffer, 2, GT911_I2C_TIMEOUT_MS) != HAL_OK)
		GT911_Reset(gt911);
}

void GT911_GetCoordResolution(GT911* gt911, uint16_t* x_res, uint16_t* y_res)
{
	uint16_t buffer[2] = { 0, 0 };
	if (HAL_I2C_Mem_Read(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_FW_VER_HEX_L, 2, (uint8_t*)buffer, 4, GT911_I2C_TIMEOUT_MS) != HAL_OK)
	{
		GT911_Reset(gt911);
		return;
	}
	*x_res = buffer[0];
	*y_res = buffer[1];
}

void GT911_GetAllConfig(GT911* gt911, uint8_t* config, uint8_t size)
{
	if (HAL_I2C_Mem_Read(gt911->hi2c, GT911_I2C_ADDR, GT911_REG_CFG_VER, 2, config, size, GT911_I2C_TIMEOUT_MS * 2) != HAL_OK)
		GT911_Reset(gt911);
}

void GT911_INTHandler(GT911* gt911)
{
	gt911->newData = 1;
}
