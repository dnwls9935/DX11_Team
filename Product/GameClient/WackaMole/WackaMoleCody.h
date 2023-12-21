#pragma once

#include "WackaMolePlayer.h"

class WackamoleHole;
class EffectPlayer;

class WackaMoleCody : public WackaMolePlayer
{
private:	enum class POSITION {UP, DOWN, LEFT, RIGHT, END };

public:		WackaMoleCody(void) noexcept;
public:		virtual ~WackaMoleCody(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	FixedUpdate(float ElapsedTime) noexcept override;
public:		void	Update(float ElapsedTime) noexcept override;
public:		void	LateUpdate(float ElapsedTime) noexcept override;

public:		virtual void	AnimationUp(void) noexcept;
public:		virtual void	AnimationDown(void) noexcept;
public:		virtual void	AnimationLeft(void) noexcept;
public:		virtual void	AnimationRight(void) noexcept;
public:		virtual void	AnimationIdle(void) noexcept;

public:		void	OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void	OnTriggerExit(Collider* mine, Collider* other) noexcept override;


private:	WackamoleHole*		_hole[(int)POSITION::END];
private:	XMVECTOR			_position;
private:	bool				_on;
private:	float				_lerpTime;

private:	float				_scoreTime;

private:	bool				_clearPosition;
private:	EffectPlayer*		_effectPlayer;
	   
};

