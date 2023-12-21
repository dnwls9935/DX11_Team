#pragma once

#include "Behaviour.h"

namespace ce
{
	class Rigidbody;
	class Collider;
}

class FlyParticle : public Behaviour
{
public: FlyParticle(float damage) noexcept : _damage(damage) {};
public:	virtual ~FlyParticle(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void		StartFX(void) noexcept;
public:		void		ClearForce(void) noexcept;
public:		void		DestroyFX(void) noexcept;

private:	float		_damage;
private:	Rigidbody*	_rigid;
private:	Collider*	_noneTriggerCollider;
private:	class ChessBoardDataManager* _board;
};

