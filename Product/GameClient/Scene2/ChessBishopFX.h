#pragma once

#include "Behaviour.h"

namespace ce
{
	class SpriteRenderer;
	class SpriteAnimator;
	class ParticleSystem;
}

class ChessBishopFX : public Behaviour
{
public:		ChessBishopFX(Transform* _bishop) noexcept;
public:		virtual ~ChessBishopFX(void) noexcept;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	LateUpdate(float fElapsedTime) noexcept override;

public:		void	StartFX(void) noexcept;
public:		void	StopFX(void) noexcept;
public:		void	Explosion(void) noexcept;
public:		void	DestroyFX(void) noexcept;


private:	GameObject*			_noiseObject[4];
private:	SpriteRenderer*		_noiseRenderer[4];
private:	SpriteAnimator*		_noiseAnimator[4];

private:	GameObject*			_flyObject[4];
private:	ParticleSystem*		_flyParticle[4];
private:	Collider*			_collider[4];

private:	Transform*			_parent;

private:	float				_lerpTime;
private:	bool				_start;
private:	bool				_hasParent = true;
private:	bool				_explosion;
};