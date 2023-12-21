#pragma once
#include "MonsterActor.h"
class TargetDummy : public MonsterActor
{
public:		TargetDummy(void) noexcept;
public:		virtual		~TargetDummy(void) noexcept = default;

public:		virtual void		Start(void) noexcept override;
public:		virtual void		Update(float fElapsedTime) noexcept override;

public:		void		GetHit(float damage, int8 attacker) noexcept override;

public:		void		Idle(void) noexcept override;
public:		void		Walk(void) noexcept override;
public:		void		Attack(void) noexcept override;
public:		void		Die(void) noexcept override;

};

