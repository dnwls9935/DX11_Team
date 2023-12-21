#pragma once
#include "Behaviour.h"

class EffectPlayer;

class BigFan_Vertical : public Behaviour
{
public:		BigFan_Vertical(void) noexcept;
public:		virtual ~BigFan_Vertical(void) noexcept;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		void		SetRotate(bool b) { _rotate = b; }

private:	bool		_rotate;
private:	bool		_sound;
private:	bool		_rotatesound;

private:	AudioAsset* _start;
private:	AudioAsset* _loop;

private:	EffectPlayer* _effectPlayer;
};

