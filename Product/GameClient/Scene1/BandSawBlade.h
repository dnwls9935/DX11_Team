#pragma once

#include "Behaviour.h"
class EffectPlayer;

class BandSawBlade : public Behaviour
{
public:		BandSawBlade(void) noexcept;
public:		virtual ~BandSawBlade(void) noexcept;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;

public:		void SetBtn(GameObject* btnA, GameObject* btnB) { _btnA = btnA; _btnB = btnB; };


private:	float _originY;

private:	bool b;
private:	bool _playSound;

private:	AudioAsset* _bandsaw;
private:	EffectPlayer* _audioSource;

private:	GameObject* _btnA;
private:	GameObject* _btnB;
};

