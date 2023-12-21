#pragma once

#include "InputHandler.h"

class WackaMolePlayer;

class WackaInput : public InputHandler
{
public:		WackaInput(bool isUsePad) noexcept;
public:		virtual ~WackaInput(void) noexcept = default;

public:		void	FixedUpdate(float fElapsedTime) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	LateUpdate(float fElapsedTime) noexcept override;

public:		void	SetWackaMolePlayer(WackaMolePlayer* actor);

public:		uint8	GetKeyInput(void) noexcept { return _input; };

private:						WackaMolePlayer*	_wackaActor;
private:						bool				_isUsePad;
private:						uint8				_input;
private:	static	constexpr	uint8				INPUT_UP	= 1 << 0;
private:	static	constexpr	uint8				INPUT_DOWN	= 1 << 1;
private:	static	constexpr	uint8				INPUT_LEFT	= 1 << 2;
private:	static	constexpr	uint8				INPUT_RIGHT = 1 << 3;
};

