#pragma once
#include "Projectile.h"

namespace ce
{
	class ModelRenderer;
	class SpriteRenderer;
	class ParticleSystem;
}

class AcornBulletFX;
class BulletFlare;

class AcornBullet : public Projectile
{
public:		AcornBullet(const FXMVECTOR			direction
						, const float			speed
						, const float			damage
						, const GameObjectTag	enemyTag) noexcept;
public:		virtual ~AcornBullet(void) noexcept = default;

public:		void			Awake(void) noexcept override;
public:		void			Start(void) noexcept override;
public:		void			FixedUpdate(float fElapsedTime) noexcept override;
public:		void			Update(float fElapsedTime) noexcept override;
public:		void			LateUpdate(float fElapsedTime) noexcept override;

public:		void			OnEnable(void) noexcept override;
public:		void			OnDisable(void) noexcept override;
public:		void			OnDestroy(void) noexcept override;

public:		void			OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void			OnTriggerExit(Collider* mine, Collider* other) noexcept override;

private:	XMFLOAT3			_direction;

private:	const float			_speed;
private:	const float			_damage;
private:	const GameObjectTag	_enemyTag;

private:	ModelRenderer*		_renderer;
private:	ModelRenderer*		_subRenderer;
private:	Transform*			_subTransform;

private:	AcornBulletFX*		_fx;
private:	BulletFlare*		_explosionFX;
};

