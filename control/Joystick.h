#pragma once
#include "BCModelBase.hpp"
#include "Manager.h"

class Joystick : System::Manager
{
public:
	Joystick(UCdevice& uc, uint8_t inputX, uint8_t inputY, uint8_t inputButton, uint8_t inputPwr,
		float offsetX, float offsetY, float scaleX, float scaleY, float zeroX, float zeroY, byte actTolerance) :
		inX(inputX), inY(inputY), inButton(inputButton), inPower(inputPwr),
		xOffset(offsetX), yOffset(offsetY), xScale(scaleX), yScale(scaleY),
		xZeroOffset(zeroX), yZeroOffset(zeroY), axisActivityTolerance(actTolerance), ucDev(&uc) {}

	enum class JoystickButton
	{
		Left = 0,
		Right
	};

	float getX();
	float getY();
	bool getButtonState(JoystickButton button);
	bool changed();
	bool inZero();
	void manager();
	void setActivityCheckState(bool state);
	void onDisconnect();
	bool isConnected();

private:
	uint8_t inX, inY, inButton, inPower;

	float xVal = 0;
	float oldXVal = 0;
	float yVal = 0;
	float oldYVal = 0;
	byte buttonState = 0;
	bool axisActitityCheckEnabled = true;

	byte axisActivityTolerance;
	float xOffset;
	float yOffset;
	float xZeroOffset;
	float yZeroOffset;
	float xScale;
	float yScale;
	
	bool connected = false;

	uint8_t startupTimeCnt = 0;
	const uint8_t startupTime = 60;

	UCdevice* ucDev = NULL;
};
