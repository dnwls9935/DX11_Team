#pragma once
#include "Behaviour.h"
#include "Actor.h"
class Projectile :  public Behaviour
{
public:		Projectile(Vector3 direction, float speed = 3, GameObjectTag enemyTag = GameObjectTag::MONSTER, float damage = 0.f, float deletetime = 2.f, bool rotate = true) noexcept;
public:		virtual		~Projectile(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		void		OnTriggerEnter(Collider* mine, Collider* other) noexcept override;

protected:	void		DestroyObject();

protected:	Vector3			_direction;
protected:	float			_speed;
protected:  bool			_rotate;

protected:	GameObjectTag	_enemyTag;
protected:	float			_damage;

protected:	float			_time = 0;
protected:	float			_deleteTime = 2.f;
};

