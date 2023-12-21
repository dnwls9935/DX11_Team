#pragma once

#include "Behaviour.h"

class AtlasAnimationX;

class BigExplosion : public Behaviour
{
public:		BigExplosion(void) noexcept;
public:		virtual ~BigExplosion(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	OnDestroy(void) noexcept override;
public:		void	StartFX(void) noexcept;

private:	GameObject*			_explosionObject[25];
private:	Transform*			_explosionTransform[25];
private:	AtlasAnimationX*	_explosionAnimation[25];

private:	float				_lerpTimeAcc[25];
private:	XMFLOAT3			_lerpPos[25];

private:	bool				_start;
};

