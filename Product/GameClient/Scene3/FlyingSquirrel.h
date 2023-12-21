#pragma once
#include "MonsterActor.h"

namespace ce
{
	class PathFollower;
	class PathData;
	class Rigidbody;
	class Collider;
}

class AcornShooter;
class ExplosionFX;
class SpriteHP;

class FlyingSquirrel : public MonsterActor
{
public:		FlyingSquirrel(PathData* data, float speed) noexcept;
public:		virtual ~FlyingSquirrel(void) noexcept = default;
public:		using TList = std::vector<FlyingSquirrel*>;

public:		void			Awake(void) noexcept override;
public:		void			Start(void) noexcept override;
public:		void			FixedUpdate(float fElapsedTime) noexcept override;
public:		void			Update(float fElapsedTime) noexcept override;
public:		void			LateUpdate(float fElapsedTime) noexcept override;

public:		void			OnTriggerEnter(Collider* mine, Collider* other) noexcept override;

public:		void			OnEnable(void) noexcept override;
public:		void			OnDisable(void) noexcept override;

public:		void			OnDestroy(void) noexcept override;
private:	void			OnEndPath(void) noexcept;

public:		void		Idle(void) noexcept override { __noop; }
public:		void		Walk(void) noexcept override { __noop; }
public:		void		Attack(void) noexcept override { __noop; }
public:		void		Die(void) noexcept override;
public:		void		GetHit(float damage, int8 attacker = 0) noexcept override;

private:	Rigidbody*		_rigid;
private:	Collider*		_collider;

private:	AcornShooter*	_left;
private:	AcornShooter*	_right;
private:	PathFollower*	_follower;
private:	PathData*		_data;
private:	ExplosionFX*	_fx;
private:	float			_speed;

private:	bool			_visibleHp;
private:	SpriteHP*		_spriteHpBar;
private:	Transform*		_player;

};

