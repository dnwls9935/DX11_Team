#pragma once
#include "Behaviour.h"

class EffectPlayer;

class BigFan_Module : public Behaviour
{
public:		BigFan_Module(void) noexcept;
public:		virtual ~BigFan_Module(void) noexcept;

public:		void		Start(void) noexcept override;
public:		void		FixedUpdate(float fElapsedTime) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		LateUpdate(float fElapsedTime) noexcept override;

private:	float		_lerpTime;
private:	bool		_reverse;
private:	bool		_lerp;
private:	bool		_playSound;

private:	float		_myX;

private:	AudioAsset* _rotate;
	   
private:	EffectPlayer* _effectPlayer;
};

