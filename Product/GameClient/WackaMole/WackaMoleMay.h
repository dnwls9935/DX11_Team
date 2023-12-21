#pragma once

#include "WackaMolePlayer.h"

class EffectPlayer;

class WackaMoleMay : public WackaMolePlayer
{
private:	enum class TURN { L90, R90, R180, NONE, END };

public:		WackaMoleMay(void) noexcept;
public:		virtual ~WackaMoleMay(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	FixedUpdate(float ElapsedTime) noexcept override;
public:		void	Update(float ElapsedTime) noexcept override;
public:		void	LateUpdate(float ElapsedTime) noexcept override;
public:		void	OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void	OnTriggerExit(Collider* mine, Collider* other) noexcept override;

private:	TURN	Turn(DIRECTION dir) noexcept;
private:	void	Hit(void) noexcept;
private:	void	HitEnd(void) noexcept;
private:	void	IdleHit(void) noexcept;

private:	GameObject*		_hammerObject;
private:	Transform*		_hammerTransform;

private:	TURN			_turn;

private:	bool			_isTurn;
private:	float			_turnTimeAcc;
private:	EffectPlayer*	_effectPlayer;
};
