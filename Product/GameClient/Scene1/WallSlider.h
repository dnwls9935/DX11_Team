#pragma once

#include "Behaviour.h"

namespace ce
{
	class Rigidbody;
	class Collider;
}
class EffectPlayer;

class WallSlider : public Behaviour
{
public:		WallSlider(void) noexcept;
public:		virtual ~WallSlider(void) noexcept;

public:		void Start(void) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		float GetAngle() {return _angle;}

public:		void SetBtn(GameObject* btnA, GameObject* btnB) { _btnA = btnA; _btnB = btnB; };
private:	float	_angle;
private:	bool	_reverse;
private:	bool	_playAni;
private:	bool	_playSound;

private:	GameObject* _rigid;
private:	GameObject* _collider;
private:	GameObject* _body;

private:	GameObject* _btnA;
private:	GameObject* _btnB;


private:	float	_lerpTime;

private:	AudioAsset* _flipSound;
private:	EffectPlayer* _audioSource;
};

