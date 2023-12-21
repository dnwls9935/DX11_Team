#pragma once

#include "ParticleSystem.h"

class FountainParticle : public ParticleSystem
{
public:		FountainParticle(void) noexcept;
public:		virtual ~FountainParticle(void) noexcept;

public:		void	UpdateXXX(float fElapsedTime) noexcept override;

public:		void	SetGravity(bool use, float value = 9.8f);
public:		void	SetDirection(FXMVECTOR dir);

private:	float		_gravity;
private:	XMFLOAT3	_direction;
private:	float		_velocity;
private:	float		_lifeTime;
};