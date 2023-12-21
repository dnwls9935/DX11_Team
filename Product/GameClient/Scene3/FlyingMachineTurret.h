#pragma once
#include "Behaviour.h"

class AcornShooter;
class Crosshair;
class EffectPlayer;

class FlyingMachineTurret : public Behaviour
{
public:		FlyingMachineTurret( const bool bOperationPad
								, Transform* cameraTrans) noexcept;
public:		virtual		~FlyingMachineTurret(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		LateUpdate(float fElapsedTime) noexcept override;

public:		void		SetCrosshair(Crosshair* crosshair) noexcept;

private:	bool			_bOperationPad;

private:	bool			_move;
private:	float			_startAngle;
private:	float			_moveAngle;

private:	Vector3			_preDirection;
private:	Transform*		_cameraTrans;
private:	Transform*		_machineTrans;

private:	Crosshair*		_cross;

private:	bool			_isRight;

private:	AcornShooter*	_leftShooter;
private:	AcornShooter*	_rightShooter;
private:	EffectPlayer*	_leftFire;
private:	EffectPlayer*	_rightFire;
private:	AudioAsset*		_audioAsset;
};

