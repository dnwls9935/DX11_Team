#include "pch.h"
#include "WackaInput.h"
#include "WackaMolePlayer.h"

WackaInput::WackaInput(bool isUsePad) noexcept
	: _isUsePad(isUsePad)
{
	/* _isUsePad == true == 패드사용 == 코디 */
	/* _isUsePad == false == 패드미사용 == 메이 */

	_input = 0;
}

void WackaInput::FixedUpdate(float fElapsedTime) noexcept
{
	if (false == _isUsePad)
	{
		_input = 0;
		if (true == _wackaActor->GetAnimation())
			return;

		if (INPUT->GetKeyDown(VK_UP))
			_input |= INPUT_UP;
		else if (INPUT->GetKeyDown(VK_DOWN))
			_input |= INPUT_DOWN;
		else if (INPUT->GetKeyDown(VK_LEFT))
			_input |= INPUT_LEFT;
		else if (INPUT->GetKeyDown(VK_RIGHT))
			_input |= INPUT_RIGHT;
	}
	else
	{
		if (PADINPUT->GetButtonStay(PadInput::PadButton::DPAD_UP))
		{
			_wackaActor->AnimationUp();
		}
		else if (PADINPUT->GetButtonStay(PadInput::PadButton::DPAD_DOWN))
		{
			_wackaActor->AnimationDown();
		}
		else if (PADINPUT->GetButtonStay(PadInput::PadButton::DPAD_RIGHT))
		{
			_wackaActor->AnimationRight();
		}
		else if (PADINPUT->GetButtonStay(PadInput::PadButton::DPAD_LEFT))
		{
			_wackaActor->AnimationLeft();
		}
		else
			_wackaActor->AnimationIdle();
	}
}

void WackaInput::Update(float fElapsedTime) noexcept
{
	if (nullptr == _wackaActor)
		return;

	if (false == _isUsePad)
	{
		if (_input & INPUT_UP)
			_wackaActor->AnimationUp();
		else if (_input & INPUT_DOWN)
			_wackaActor->AnimationDown();
		else if (_input & INPUT_LEFT)
			_wackaActor->AnimationLeft();
		else if (_input & INPUT_RIGHT)
			_wackaActor->AnimationRight();
	}
	else
	{
		/*if (_input & INPUT_UP)
			_wackaActor->AnimationUp();
		else if (_input & INPUT_DOWN)
			_wackaActor->AnimationDown();
		else if (_input & INPUT_LEFT)
			_wackaActor->AnimationLeft();
		else if (_input & INPUT_RIGHT)
			_wackaActor->AnimationRight();*/
	}
}

void WackaInput::LateUpdate(float fElapsedTime) noexcept
{
}

void WackaInput::SetWackaMolePlayer(WackaMolePlayer* actor)
{
	_wackaActor = actor;
}