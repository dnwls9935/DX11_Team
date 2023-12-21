#pragma once
#include "MonsterActor.h"

class Lock;
class EffectPlayer;
class LockHint : public MonsterActor
{
public:		LockHint(Lock* lock) noexcept;
public:		~LockHint(void) noexcept = default;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;

public:		void GetHit(float damage, int8 attacker) noexcept override;
public:		void Die(void) noexcept override;

private:	void OnEndAnimation(void) noexcept;

	  // Don't use
public:		void Idle(void) noexcept override;
public:		void Walk(void) noexcept override;
public:		void Attack(void) noexcept override;
	  //

private:	bool	_visibleHp;
private:	float	_visibleTime;

private:	bool	_endRigid;

private:	GameObject* _child;
private:	Lock*		_lock;

private:	Rigidbody* _rigid;

private:	EffectPlayer*	_ChainDropEffectPlayer;
private:	AudioAsset*		_ChainDropAsset;

private:	EffectPlayer*	_DestroyEffectPlayer;
private:	AudioAsset*		_DestroyAsset;
};

