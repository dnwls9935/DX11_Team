#pragma once
#include "Behaviour.h"
#include "FlyingSquirrel.h"

class Warning;

class FlyingSquirrelSummoner : public Behaviour
{
public:		FlyingSquirrelSummoner(void) noexcept = default;
public:		virtual ~FlyingSquirrelSummoner(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		OnTriggerEnter(Collider* mine, Collider* other) noexcept override;

public:		void		AddSquirrel(FlyingSquirrel* squirrel) noexcept;

private:	bool					_summon = false;
private:	FlyingSquirrel::TList	_squirrels;
private:	Warning*				_warning;
};

