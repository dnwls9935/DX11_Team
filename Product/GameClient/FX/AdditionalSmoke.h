#pragma once

#include "Behaviour.h"

class AtlasAnimationX;

class AdditionalSmoke : public Behaviour
{
public:		AdditionalSmoke(void) noexcept;
public:		virtual ~AdditionalSmoke(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	StartFX(void) noexcept;

private:	GameObject*			_smokeObject[10];
private:	Transform*			_smokeTransform[10];
private:	AtlasAnimationX*	_smokeAnimation[10];

private:	float				_lerpTimeAcc[10];
private:	XMFLOAT3			_lerpPos[10];

private:	bool				_start;
};

