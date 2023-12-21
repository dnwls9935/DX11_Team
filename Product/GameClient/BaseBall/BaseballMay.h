#pragma once
#include "Actor.h"

namespace ce
{
	class HierarcyNode;
}

class BaseballLever;

class BaseballMay final : public Actor
{
public:		BaseballMay(bool bOperationPad) noexcept;
public:		virtual ~BaseballMay(void) noexcept;

public:		virtual	void	Start(void) noexcept override;
public:		virtual void	FixedUpdate(float fElapsedTime) noexcept override;
public:		virtual void	Update(float fElapsedTime) noexcept override;
public:		virtual void	LateUpdate(float fElapsedTime) noexcept override;

public:		virtual void	Idle(void) noexcept override { __noop; }
public:		virtual void	Attack(void) noexcept override { __noop; }
public:		virtual void	Die(void) noexcept override { __noop; }
public:		virtual void	Walk(void) noexcept override { __noop; }
public:		virtual void	Run(void) noexcept override { __noop; }
public:		virtual void	GetHit(float damage, int8 attacker) noexcept override { __noop; }
public:		virtual void	Dash(void) noexcept override { __noop; }
public:		virtual void	Jump(void) noexcept override { __noop; }
public:		virtual void	DoubleJump(void) noexcept override { __noop; }
public:		virtual void	Falling(void) noexcept override { __noop; }
public:		virtual void	Interaction(void) noexcept { __noop; }

private:	BaseballLever*			_lever;
};

