#include "pch.h"
#include "PlayerInput.h"
#include "Actor.h"
#include "GameObject.h"
#include "Rigidbody.h"

PlayerInput::PlayerInput(bool bOperationPad) noexcept
	:_bOperationPad(bOperationPad)
{
	if (bOperationPad)
		PADINPUT->SetUsePad(true);
}

PlayerInput::~PlayerInput(void) noexcept
{

}

void PlayerInput::FixedUpdate(float fElapsedTime) noexcept
{
	_dirInput = 0;
	_actionInput = 0;

	if (!_bOperationPad)
		FixedKeyboard();
	else
		FixedPad();

	angle = 360;

	if (_dirInput & INPUT_FRONT)
	{
		if (_dirInput & INPUT_RIGHT)
			angle = 45;
		else if (_dirInput & INPUT_LEFT)
			angle = 315;
		else
			angle = 360;
	}
	else if (_dirInput & INPUT_BACKWARD)
	{
		if (_dirInput & INPUT_RIGHT)
			angle = 135;
		else if (_dirInput & INPUT_LEFT)
			angle = 225;
		else
			angle = 180;
	}
	else if (_dirInput & INPUT_LEFT)
	{
		angle = 270;
	}
	else if (_dirInput & INPUT_RIGHT)
	{
		angle = 90;
	}

	//if (_dirInput != 0)
	//	_actor->SetDirection(angle);
}

void PlayerInput::ClearAction() noexcept
{
	switch (_actor->GetState())
	{
	case Actor::State::DASH:
		_actionInput &= ~INPUT_DASH;
		break;
	case Actor::State::JUMP: case Actor::State::FALLING:
		_actionInput &= ~INPUT_JUMP;
		break;
	case Actor::State::ATTACK:
		if (!_bOperationPad)
		{
			_actionInput &= ~INPUT_INTERACTION;
			_actionInput &= ~INPUT_LEFTCLICK;
			_actionInput &= ~INPUT_RIGHTCLICK;
		}
		else
		{
			_actionInput &= ~INPUT_INTERACTION;
			_actionInput &= ~INPUT_DASH;
			_actionInput &= ~INPUT_CROUCH;
		}
		break;
	}
}

void PlayerInput::FixedKeyboard()
{
	if (INPUT->GetKeyStay('W'))
		_dirInput |= INPUT_FRONT;
	if (INPUT->GetKeyStay('A'))
		_dirInput |= INPUT_LEFT;
	if (INPUT->GetKeyStay('D'))
		_dirInput |= INPUT_RIGHT;
	if (INPUT->GetKeyStay('S'))
		_dirInput |= INPUT_BACKWARD;

	if (INPUT->GetKeyDown(VK_LBUTTON))
		_actionInput |= INPUT_LEFTCLICK;
	if (INPUT->GetKeyDown(VK_RBUTTON))
		_actionInput |= INPUT_RIGHTCLICK;

	if (INPUT->GetKeyDown('E'))
		_actionInput |= INPUT_INTERACTION;
	if (INPUT->GetKeyDown(VK_SPACE))
		_actionInput |= INPUT_JUMP;
	if (INPUT->GetKeyDown(VK_LSHIFT))
		_actionInput |= INPUT_DASH;
	if (INPUT->GetKeyDown(VK_LCONTROL))
		_actionInput |= INPUT_CROUCH;

	if (INPUT->GetKeyDown(VK_CAPITAL))
		INPUT_SPRINT = true;
	else if (INPUT->GetKeyUp(VK_CAPITAL))
		INPUT_SPRINT = false;

	_mouseX = DIRECTINPUT->GetMouseMovementX();
	_mouseY = DIRECTINPUT->GetMouseMovementY();
}

void PlayerInput::FixedPad()
{
	if (PADINPUT->GetLeftStickY() > 0)
		_dirInput |= INPUT_FRONT;
	else if(PADINPUT->GetLeftStickY() < 0)
		_dirInput |= INPUT_BACKWARD;

	if (PADINPUT->GetLeftStickX() > 0)
		_dirInput |= INPUT_RIGHT;
	else if(PADINPUT->GetLeftStickX() < 0)
		_dirInput |= INPUT_LEFT;


	if (PADINPUT->GetButtonDown(PadInput::BUTTON_A))
		_actionInput |= INPUT_JUMP;
	if (PADINPUT->GetButtonDown(PadInput::BUTTON_X))
		_actionInput |= INPUT_DASH;
	if (PADINPUT->GetButtonDown(PadInput::BUTTON_Y))
		_actionInput |= INPUT_INTERACTION;
	if (PADINPUT->GetButtonDown(PadInput::BUTTON_B))
		_actionInput |= INPUT_CROUCH;

	if (PADINPUT->GetButtonDown(PadInput::SHOULDER_L))
		INPUT_SPRINT = true;
	else if (PADINPUT->GetButtonUp(PadInput::SHOULDER_L))
		INPUT_SPRINT = false;
}

bool PlayerInput::GetActionState(Actor::State state) const noexcept
{
	switch (state)
	{
	case Actor::State::IDLE:
		break;
	case Actor::State::WALK:
		return _dirInput;
		break;
	case Actor::State::RUN:
		return INPUT_SPRINT;
		break;
	case Actor::State::ATTACK:
		if(!_bOperationPad)
			return (_actionInput & INPUT_LEFTCLICK) || (_actionInput & INPUT_RIGHTCLICK) || (_actionInput & INPUT_INTERACTION);
		else
			return (_actionInput & INPUT_CROUCH) || (_actionInput & INPUT_DASH) || (_actionInput & INPUT_INTERACTION);
		break;
	case Actor::State::DASH:
		return _actionInput & INPUT_DASH;
		break;
	case Actor::State::JUMP:
		return (_actionInput & INPUT_JUMP && _dirInput == 0);
		break;
	case Actor::State::JUMPFWD:
		return (_actionInput & INPUT_JUMP && _dirInput != 0);
		break;
	case Actor::State::FALLING:
		break;
	case Actor::State::GETHIT:
		break;
	case Actor::State::DIE:
		break;
	case Actor::State::END:
		break;
	case Actor::State::INTERACTION:
		return _actionInput & INPUT_INTERACTION;
		break;
	case Actor::State::CROUCHSTART:
		return _actionInput & INPUT_CROUCH;
		break;
	default:
		break;
	}

	return false;
}
