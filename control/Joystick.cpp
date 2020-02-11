#include "Joystick.h"
#include "gui/model/Model.hpp"

float Joystick::getX()
{
	return xVal;
}

float Joystick::getY()
{
	return yVal;
}

bool Joystick::getButtonState(JoystickButton button)
{
	bool state = false;
	if (button == JoystickButton::Right)
	{
		state = (buttonState & 0x01) != 0;
	}
	if (button == JoystickButton::Left)
	{
		state = (buttonState & 0x02) != 0;
	}
	return state;
}

bool Joystick::changed()
{
	float oldX = oldXVal;
	float oldY = oldYVal;
	bool state = false;

	if (axisActitityCheckEnabled)
	{
		oldXVal = xVal;
		oldYVal = yVal;
		state = hypotf(fabsf(oldX - xVal), fabsf(oldY - yVal)) > axisActivityTolerance;
	}

	return state;
}

bool Joystick::inZero()
{
	bool state = true;
	if (axisActitityCheckEnabled)
	{
		state = hypotf(fabsf(xVal - xZeroOffset), fabsf(yVal - yZeroOffset)) < axisActivityTolerance;
	}
	return state;
}

void Joystick::manager()
{
	if (ucDev->input[inPower].value.Voltage > 4)
	{
		if (startupTimeCnt == startupTime)
		{
			connected = true;
			xVal = (ucDev->input[inX].value.Voltage - xOffset) * xScale;
			yVal = (ucDev->input[inY].value.Voltage - yOffset) * yScale;
			buttonState = (byte)(ucDev->input[inButton].value.Voltage > 3.0f) |
				((byte)(ucDev->input[inButton].value.Voltage < 2.0f) << 1);
		}
		else
		{
			startupTimeCnt++;
		}
	}
	else
	{
		if (startupTimeCnt != 0)
		{
			onDisconnect();
			startupTimeCnt = 0;
			connected = false;
		}
	}
}

void Joystick::setActivityCheckState(bool state)
{
	axisActitityCheckEnabled = state;
	changed();
}

void Joystick::onDisconnect()
{
	xVal = 0;
	oldXVal = 0;
	yVal = 0;
	oldYVal = 0;
	buttonState = 0;
}

bool Joystick::isConnected()
{
	return connected;
}
