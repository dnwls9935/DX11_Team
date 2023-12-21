#pragma once
#include "MonsterActor.h"

namespace ce
{
	class ParticleSystem;
	class Animator;
	class Collider;
}
class EffectPlayer;

class Destruction : public MonsterActor
{
public:		Destruction(float maxHp) noexcept;
public:		~Destruction(void) noexcept = default;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;

public:		void GetHit(float damage, int8 attacker) noexcept override;
public:		void Die(void) noexcept override;

 // Don't use
public:		void Idle(void) noexcept override;
public:		void Walk(void) noexcept override;
public:		void Attack(void) noexcept override;
protected:	void OnEndAnimation(void) noexcept;
 //

protected:	ParticleSystem*		_particle;
protected:	Animator*			_animator;
protected:	Collider*			_collider0;
protected:	Collider*			_collider1;

protected: EffectPlayer* _breakEffectPlayer;
protected: AudioAsset* _breakAsset;

protected:	bool				_smashed;
};

