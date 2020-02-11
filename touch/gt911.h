#ifndef _GT911_H
#define _GT911_H

#include "stm32f7xx_hal.h"

////////////////////////////////////////// Register description //////////////////////////////////////////

#define GT911_REG_COMMAND					0x8040
#define GT911_REG_CFG_VER					0x8047 //Version of the configuration
#define GT911_REG_X_OUT_MAX_L				0x8048 //Resolution of X axis
#define GT911_REG_X_OUT_MAX_H				0x8049
#define GT911_REG_Y_OUT_MAX_L				0x804A //Resolution of Y axis
#define GT911_REG_Y_OUT_MAX_H				0x804B
#define GT911_REG_TOUCH_NUM					0x804C //1..5
#define GT911_REG_MODULE_SWITCH1			0x804D
#define GT911_REG_MODULE_SWITCH2			0x804E
#define GT911_REG_SHAKE_COUNT				0x804F
#define GT911_REG_FILTER					0x8050
#define GT911_REG_LARGE_TOUCH				0x8051 //Number of touch in large area
#define GT911_REG_NOISE_REDUCTION			0x8052
#define GT911_REG_SCREEN_TOUCH_LVL			0x8053 //Threshold of touch points on screen from none to availability
#define GT911_REG_SCREEN_LEAVE_LVL			0x8054 //Threshold of touch points on screen from availability to none
#define GT911_REG_LOW_PWR_CTRL				0x8055
#define GT911_REG_REFRESH_RATE				0x8056
#define GT911_REG_X_THRESHOLD				0x8057
#define GT911_REG_Y_THRESHOLD				0x8058
#define GT911_REG_X_SPEED_LIMIT				0x8059
#define GT911_REG_Y_SPEED_LIMIT				0x805A
#define GT911_REG_SPACE_TOP_BOTTOM			0x805B
#define GT911_REG_SPACE_LEFT_RIGHT			0x805C
#define GT911_REG_STRETCH_RATE				0x805D
#define GT911_REG_STRETCH_R0				0x805E //Interval 1 coefficient
#define GT911_REG_STRETCH_R1				0x805F //Interval 2 coefficient
#define GT911_REG_STRETCH_R2				0x8060 //Interval 3 coefficient
#define GT911_REG_STRETCH_RM				0x8061 //All intervals base number
#define GT911_REG_DRV_GROUP_A_NUM			0x8062
#define GT911_REG_DRV_GROUP_B_NUM			0x8063
#define GT911_REG_SENSOR_NUM				0x8064
#define GT911_REG_FREQ_A_FACTOR				0x8065 /* Driver frequency double frequency coefficient of Driver group A
													GroupA_Frequence = Multiplier factor * baseband */
#define GT911_REG_FREQ_B_FACTOR				0x8066 /* Driver frequency double frequency coefficient of Driver group B
													GroupB_Frequence = Multiplier factor * baseband */
#define GT911_REG_PANEL_BITFREQ_L			0x8067 //Baseband of Driver group A\B (1526HZ<baseband<14600Hz)
#define GT911_REG_PANEL_BITFREQ_H			0x8068
#define GT911_REG_PANEL_SENSOR_TIME_L		0x8069
#define GT911_REG_PANEL_SENSOR_TIME_H		0x806A
#define GT911_REG_PANEL_TX_GAIN				0x806B
#define GT911_REG_PANEL_RX_GAIN				0x806C
#define GT911_REG_PANEL_DUMP_SHIFT			0x806D
#define GT911_REG_DRV_FRAME_CTRL			0x806E
#define GT911_REG_FREQ_HOPPING_START		0x807A //Frequency hopping start frequency (Unit: 2KHz, 50 means 100KHz )
#define GT911_REG_FREQ_HOPPING_END			0x807B //Frequency hopping stop frequency (Unit: 2KHz, 150 means 300KHz )
#define GT911_REG_NOISE_DETECT_TIMES		0x807C
#define GT911_REG_FREQ_HOPPING_FLAG			0x807D
#define GT911_REG_FREQ_HOPPING_THRESHOLD	0x807E
#define GT911_REG_NOISE_THRESHOLD			0x807F //Threshold of noise level
#define GT911_REG_HOPPING_SEG1_BITFREQ_L	0x8082 //Frequency hopping segment band 1 central frequency (for driver A/B)
#define GT911_REG_HOPPING_SEG1_BITFREQ_H	0x8083
#define GT911_REG_HOPPING_SEG1_FACTOR		0x8084 //Frequency hopping segment 1 central frequency coefficient
#define GT911_REG_HOPPING_SEG2_BITFREQ_L	0x8085 //Frequency hopping segment band 2 central frequency (for driver A/B)
#define GT911_REG_HOPPING_SEG2_BITFREQ_H	0x8086
#define GT911_REG_HOPPING_SEG2_FACTOR		0x8087 //Frequency hopping segment 2 central frequency coefficient
#define GT911_REG_HOPPING_SEG3_BITFREQ_L	0x8088 //Frequency hopping segment band 3 central frequency (for driver A/B)
#define GT911_REG_HOPPING_SEG3_BITFREQ_H	0x8089
#define GT911_REG_HOPPING_SEG3_FACTOR		0x808A //Frequency hopping segment 3 central frequency coefficient
#define GT911_REG_HOPPING_SEG4_BITFREQ_L	0x808B //Frequency hopping segment band 4 central frequency (for driver A/B)
#define GT911_REG_HOPPING_SEG4_BITFREQ_H	0x808C
#define GT911_REG_HOPPING_SEG4_FACTOR		0x808D //Frequency hopping segment 4 central frequency coefficient
#define GT911_REG_HOPPING_SEG5_BITFREQ_L	0x808E //Frequency hopping segment band 5 central frequency (for driver A/B)
#define GT911_REG_HOPPING_SEG5_BITFREQ_H	0x808F
#define GT911_REG_HOPPING_SEG5_FACTOR		0x8090 //Frequency hopping segment 5 central frequency coefficient
#define GT911_REG_KEY_1						0x8093	//	Key 1 Position: 0-255 valid (0 means no touch, it means independent touch key when 4 of the keys are 8 multiples, Reserved
#define GT911_REG_KEY_2						0x8094	//	Key 2 position, Reserved
#define GT911_REG_KEY_3						0x8095	//	Key 3 position, Reserved
#define GT911_REG_KEY_4						0x8096	//	Key 4 position, Reserved
#define GT911_REG_KEY_AREA            		0x8097
#define GT911_REG_KEY_TOUCH_LEVEL			0x8098	//	Key threshold of touch key, Reserved
#define GT911_REG_KEY_LEAVE_LEVEL			0x8099	//	Key threshold of touch key, Reserved
#define GT911_REG_KEY_SENS_1_2				0x809A	//	sensitivity coefficients of key 1 and 2
#define GT911_REG_KEY_SENS_3_4				0x809B	//	sensitivity coefficients of key 3 and 4
#define GT911_REG_KEY_RESTRAIN				0x809C	//	Finger from screen left after inhibition of key time (Unit:100ms,0 means 600ms), The independent button pro key inhibition parameters
#define GT911_REG_ITO_SENSOR_CH_NUM			0x80B7	//	Sensor_CH0 - Sensor_CH13 ITO Sensor corresponding chip channel number
#define GT911_REG_ITO_DRIVER_CH_NUM			0x80D5	//	Driver_CH0 - Driver_CH25 ITO Driver corresponding chip channel number
#define GT911_REG_CONFIG_CHKSUM				0x80FF	//	configuration information verify (the complement number of total byte from 0x8047 to 0x80FE)
#define GT911_REG_CONFIG_FRESH				0x8100	//	signal of updated configuration (the host writes)

#define GT911_REG_PID_BYTE1_ASCII			0x8140 //(R)	Product ID (first byte, ASCII)
#define GT911_REG_PID_BYTE2_ASCII			0x8141 //(R)	Product ID (second byte, ASCII)
#define GT911_REG_PID_BYTE3_ASCII			0x8142 //(R)	Product ID (third byte, ASCII)
#define GT911_REG_PID_BYTE4_ASCII			0x8143 //(R)	Product ID (forth byte, ASCII)
#define GT911_REG_FW_VER_HEX_L				0x8144 //(R)	Firmware version (HEX.low byte)
#define GT911_REG_FW_VER_HEX_H				0x8145 //(R)	Firmware version (HEX.high byte)
#define GT911_REG_X_COORD_RES_L				0x8146 //(R)	x coordinate resolution (low byte)
#define GT911_REG_X_COORD_RES_H				0x8147 //(R)	x coordinate resolution (high byte)
#define GT911_REG_Y_COORD_RES_L				0x8148 //(R)	y coordinate resolution (low byte)
#define GT911_REG_Y_COORD_RES_H				0x8149 //(R)	y coordinate resolution (high byte)
#define GT911_REG_VID						0x814A //(R)	Vendor_id (current module option information)
#define GT911_REG_STATUS					0x814E //(R/W)	buffer status large detect number of touch points
#define GT911_REG_TRACK_ID1					0x814F //(R) 	track id 1
#define GT911_REG_POINT_1_X_COORD_L			0x8150 //(R) 	point 1 x coordinate (low byte)
#define GT911_REG_POINT_1_X_COORD_H			0x8151 //(R) 	point 1 x coordinate (high byte)
#define GT911_REG_POINT_1_Y_COORD_L			0x8152 //(R) 	point 1 y coordinate (low byte)
#define GT911_REG_POINT_1_Y_COORD_H			0x8153 //(R) 	point 1 y coordinate (high byte)
#define GT911_REG_POINT_1_SIZE_L			0x8154 //(R) 	Point 1 size (low byte)
#define GT911_REG_POINT_1_SIZE_H			0x8155 //(R) 	point 1 size (high byte)
#define GT911_REG_TRACK_ID2					0x8157 //(R) 	track id 2
#define GT911_REG_POINT_2_X_COORD_L			0x8158 //(R) 	point 2 x coordinate (low byte)
#define GT911_REG_POINT_2_X_COORD_H			0x8159 //(R) 	point 2 x coordinate (high byte)
#define GT911_REG_POINT_2_Y_COORD_L			0x815A //(R) 	point 2 y coordinate (low byte)
#define GT911_REG_POINT_2_Y_COORD_H			0x815B //(R) 	point 2 y coordinate (high byte)
#define GT911_REG_POINT_2_SIZE_L			0x815C //(R) 	point 2 size (low byte)
#define GT911_REG_POINT_2_SIZE_H			0x815D //(R) 	point 2 size (high byte)
#define GT911_REG_TRACK_ID3					0x815F //(R) 	track id 3
#define GT911_REG_POINT_3_X_COORD_L			0x8160 //(R) 	point 3 x coordinate (low byte)
#define GT911_REG_POINT_3_X_COORD_H			0x8161 //(R) 	point 3 x coordinate (high byte)
#define GT911_REG_POINT_3_Y_COORD_L			0x8162 //(R) 	point 3 y coordinate (low byte)
#define GT911_REG_POINT_3_Y_COORD_H			0x8163 //(R) 	point 3 y coordinate (high byte)
#define GT911_REG_POINT_3_SIZE_L			0x8164 //(R) 	point 3 size (low byte)
#define GT911_REG_POINT_3_SIZE_H			0x8165 //(R) 	point 3 size (high byte)
#define GT911_REG_TRACK_ID4					0x8167 //(R) 	track id 4
#define GT911_REG_POINT_4_X_COORD_L			0x8168 //(R) 	point 4 x coordinate (low byte)
#define GT911_REG_POINT_4_X_COORD_H			0x8169 //(R) 	point 4 x coordinate (high byte)
#define GT911_REG_POINT_4_Y_COORD_L			0x816A //(R) 	point 4 y coordinate (low byte)
#define GT911_REG_POINT_4_Y_COORD_H			0x816B //(R) 	point 4 y coordinate (high byte)
#define GT911_REG_POINT_4_SIZE_L			0x816C //(R) 	point 4 size (low byte)
#define GT911_REG_POINT_4_SIZE_H			0x816D //(R) 	point 4 size (high byte)
#define GT911_REG_TRACK_ID5					0x816F //(R) 	track id 5
#define GT911_REG_POINT_5_X_COORD_L			0x8170 //(R) 	point 5 x coordinate (low byte)
#define GT911_REG_POINT_5_X_COORD_H			0x8171 //(R) 	point 5 x coordinate (high byte)
#define GT911_REG_POINT_5_Y_COORD_L			0x8172 //(R) 	point 5 y coordinate (low byte)
#define GT911_REG_POINT_5_Y_COORD_H			0x8173 //(R) 	point 5 y coordinate (high byte)
#define GT911_REG_POINT_5_SIZE_L			0x8174 //(R) 	point 5 size (low byte)
#define GT911_REG_POINT_5_SIZE_H			0x8175 //(R) 	point 5 size (high byte)

#define GT911_I2C_ADDR			0xBA //0xBB for reading
#define GT911_I2C_TIMEOUT_MS	10
#define GT911_TURN_ON_DELAY_MS	200
#define GT911_TOUCH_DATA_SIZE	8

///////////////////////////////////////////// Bit description /////////////////////////////////////////////

#define GT911_REG_STATUS_DATA_VALID_MASK		0x80
#define GT911_REG_STATUS_LARGE_DETECT_MASK		0x40
#define GT911_REG_STATUS_TOUCH_POINTS_NUM_MASK	0x07

typedef enum
{
	ReadCoord = 0U, ReadDiffOrRaw, SoftReset, BaselineUdate, BaselineCalibration, ScreenOff
} GT911_Cmd;

typedef struct
{
	uint16_t x;
	uint16_t y;
}__attribute__((packed, aligned)) point2D;

typedef struct
{
	uint8_t trackID;
	point2D point;
	uint16_t size;
	uint8_t reserved;
}__attribute__((packed, aligned)) GT911_PointData;

typedef struct
{
	I2C_HandleTypeDef* hi2c;
	I2C_TypeDef* i2c;
	uint8_t sclPin;
	uint8_t sdaPin;
	GPIO_TypeDef* i2cPort;
	uint8_t i2cPinAF;
	GPIO_TypeDef* resetPort;
	uint8_t resetPin;
	GPIO_TypeDef* intPort;
	uint8_t intPin;
	uint8_t allowedTouchNumber;
	uint8_t newData;
} GT911;

typedef struct
{
	uint8_t largeDetected;
	uint8_t pointsNumber;
	GT911_PointData points[5];
}__attribute__((packed, aligned)) GT911_OutputData;

uint8_t GT911_Init(GT911* gt911);
uint8_t GT911_DeInit(GT911* gt911);
void GT911_Reset(GT911* gt911);
void GT911_Wakeup(GT911* gt911);
void GT911_SendCmd(GT911* gt911, GT911_Cmd cmd);
uint8_t GT911_ReadCoord(GT911* gt911, GT911_OutputData* data);
void GT911_GetPID(GT911* gt911, uint8_t* buffer);
void GT911_GetFWVersion(GT911* gt911, uint16_t* buffer);
void GT911_GetCoordResolution(GT911* gt911, uint16_t* x_res, uint16_t* y_res);
void GT911_GetOutputMax(GT911* gt911, uint16_t* x_max, uint16_t* y_max);
void GT911_SetOutputMax(GT911* gt911, uint16_t x_max, uint16_t y_max);
void GT911_UpdateConfig(GT911* gt911);
void GT911_GetRefreshRate(GT911* gt911, uint8_t* rate);
void GT911_SetRefreshRate(GT911* gt911, uint8_t rate);
void GT911_GetTouchThreshold(GT911* gt911, uint8_t* threshold_start, uint8_t* threshold_end);
void GT911_SetTouchThreshold(GT911* gt911, uint8_t threshold_start, uint8_t threshold_end);
void GT911_GetAllConfig(GT911* gt911, uint8_t* config, uint8_t size);
void GT911_INTHandler(GT911* gt911);

#endif
