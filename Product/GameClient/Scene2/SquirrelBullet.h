#pragma once
#include "Projectile.h"

namespace ce
{
	class ModelRenderer;
	class SpriteRenderer;
	class ParticleSystem;
}

class SquirrelBullet  : public Projectile
{
public:	SquirrelBullet(Vector3 direction) noexcept;
public:	virtual ~SquirrelBullet(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		OnTriggerEnter(Collider* mine, Collider* other) noexcept override;

private:	bool		_shot;
private:	bool		_isReady;
private:	float		_deltaTime;

private:	ModelRenderer* _modelRenderer;
private:	SpriteRenderer* _spriteRenderer;
private:	ParticleSystem* _smokeParticle;
private:	Collider* _collider;


};

