#include "input.h"

declfield(ioInput::bUseJoystick)(0x679674);
declfield(ioInput::bUseKeyboard)(0x679675);

float ioInput::ioAddDeadZone(float originalValue, float deadZone)
{
	float val;

	if (originalValue > deadZone) {
		val = (originalValue - deadZone) / (1.0f - deadZone);
	}
	else if (originalValue < -deadZone) {
		val = (originalValue + deadZone) / (1.0f - deadZone);
	}
	else {
		val = 0.0f;
	}

	return val;
}
