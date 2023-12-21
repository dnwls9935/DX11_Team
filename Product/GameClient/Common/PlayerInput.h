#pragma once
#include "InputHandler.h"
#include "Actor.h"

class PlayerInput : public InputHandler
{
public:		PlayerInput(bool bOperationPad = false) noexcept;
public:		virtual ~PlayerInput(void) noexcept;

public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Update(float fElapsedTime) noexcept override { __noop; };
public:		void LateUpdate(float fElapsedTime) noexcept override { __noop; };;

public:		void ClearAction() noexcept;

private:	void FixedKeyboard();
private:	void FixedPad();

public:		uint8 GetDirInput()							const	noexcept			 {	return _dirInput;};
public:		uint8 GetActionInput()						const	noexcept			 { return _actionInput; };
public:		bool  GetActionState(Actor::State state)	const noexcept;
public:		float GetAngle()							const   noexcept			 { return angle; };

public: bool	GetOperationPad() const noexcept {return _bOperationPad;}

private:	uint8	_dirInput;
private:	uint8	_actionInput;

private:	float	angle = 360;

private:	long	_mouseX;
private:	long	_mouseY;


private:	bool	INPUT_SPRINT = false;

private:	bool	_bOperationPad;

private:	static	constexpr	uint8 INPUT_FRONT		= 1 << 0;
private:	static	constexpr	uint8 INPUT_LEFT		= 1 << 1;
private:	static	constexpr	uint8 INPUT_RIGHT		= 1 << 2;
private:	static	constexpr	uint8 INPUT_BACKWARD	= 1 << 3;
private:	static	constexpr	uint8 INPUT_CAMUP		= 1 << 4;
private:	static	constexpr	uint8 INPUT_CAMLEFT		= 1 << 5;
private:	static	constexpr	uint8 INPUT_CAMRIGHT	= 1 << 6;
private:	static	constexpr	uint8 INPUT_CAMDOWN		= 1 << 7;



private:	static	constexpr	uint8 INPUT_JUMP		= 1 << 0;
private:	static	constexpr	uint8 INPUT_DASH		= 1 << 1;
private:	static	constexpr	uint8 INPUT_INTERACTION = 1 << 2;
private:	static	constexpr	uint8 INPUT_CROUCH		= 1 << 3;
private:	static	constexpr	uint8 INPUT_GOUNDPOUND  = 1 << 4;

private:	static	constexpr	uint8 INPUT_LEFTCLICK	= 1 << 5;
private:	static	constexpr	uint8 INPUT_RIGHTCLICK	= 1 << 6;

};

