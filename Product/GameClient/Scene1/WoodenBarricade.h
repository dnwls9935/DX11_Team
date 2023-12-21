#pragma once
#include "MonsterActor.h"

namespace ce
{
	class Collider;
}
class EffectPlayer;

class WoodenBarricade : public MonsterActor
{
public:		WoodenBarricade(void) noexcept;
public:		~WoodenBarricade(void) noexcept	= default;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
		
public:		void GetHit(float damage, int8 attacker) noexcept override;
public:		void Die(void) noexcept override;

// Don't use
public:		void Idle(void) noexcept override;
public:		void Walk(void) noexcept override;
public:		void Attack(void) noexcept override;
//

private:	bool	_visibleHp;
private:	float	_visibleTime;

private:	bool	_smash;

private:	Collider*	_collider0;
private:	Collider*	_collider1;

private:	EffectPlayer*	_breakEffectPlayer;
private:	AudioAsset*		_breakAsset;

};

