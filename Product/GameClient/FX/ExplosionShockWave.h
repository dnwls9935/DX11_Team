#pragma once

#include "Behaviour.h"

namespace ce
{
	class SpriteRenderer;
}

class ExplosionShockWave : public Behaviour
{
public:		ExplosionShockWave(void) noexcept;
public:		virtual ~ExplosionShockWave(void) noexcept = default;
public:		void	StartFX(void) noexcept;

public:		void	Start(void) noexcept;
public:		void	Update(float fElapsedTime) noexcept;

private:	SpriteRenderer*		_renderer;

private:	float				_lerpTime;
private:	bool				_start;
};

