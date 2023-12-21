#pragma once
#include "Behaviour.h"

namespace ce
{
	class SpriteRenderer;
}

class NoiseFireFX : public Behaviour
{
public:		NoiseFireFX(void) noexcept;
public:		virtual ~NoiseFireFX(void) = default;

public:		void	Start(void) noexcept override;
public:		void	FixedUpdate(float fElapsedTime) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	LateUpdate(float fElapsedTime) noexcept override;

public:		void	OnEnable(void) noexcept override;
public:		void	OnDisable(void) noexcept override;

public:		void	Play(const float time) noexcept;

private:	bool				_isPlaying = false;

private:	float				_deltaTime = 0.0f;
private:	float				_endTime = 0.0f;

private:	SpriteRenderer*		_renderer;
};

