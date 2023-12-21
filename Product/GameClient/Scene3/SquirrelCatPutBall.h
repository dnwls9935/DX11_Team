#pragma once
#include "Behaviour.h"

namespace ce
{
	class Rigidbody;
	class Collider;
}

class SquirrelCatPutBall : public Behaviour
{
public:		SquirrelCatPutBall(void) noexcept = default;
public:		virtual ~SquirrelCatPutBall(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		FixedUpdate(float fElapsedTime) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		LateUpdate(float fElapsedTime) noexcept override;

public:		void		OnEnable(void) noexcept override;
public:		void		OnDisable(void) noexcept override;

public:		void		Fire(FXMVECTOR force) noexcept;

private:	Rigidbody*	_rigidbody;
private:	Collider*	_collider;
};

