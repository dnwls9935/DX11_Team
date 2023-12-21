#pragma once
#include "Projectile.h"

namespace ce
{
	class SpriteRenderer;
	class ParticleSystem;
}

class EffectPlayer;

class MageMissile : public Projectile
{
public:		MageMissile(Vector3 direction) noexcept;
public:		virtual		~MageMissile(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		OnTriggerEnter(Collider* mine, Collider* other) noexcept override;

private:	bool		_shot;
private:	bool		_isReady;
private:	float		_deltaTime;
private:	SpriteRenderer* _spriteRenderer;
private:	ParticleSystem* _cloudParticle;
private:	Collider*		_collider;

private:	AudioAsset* _impact;
private:	EffectPlayer* _effectPlayer;
};

