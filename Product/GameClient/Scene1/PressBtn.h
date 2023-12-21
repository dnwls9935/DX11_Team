#pragma once

#include "Behaviour.h"
#include "GameObject.h"

namespace ce
{
	class Rigidbody;
}
class EffectPlayer;

class PressBtn : public Behaviour
{
public:		PressBtn(void) noexcept;
public:		virtual ~PressBtn(void) noexcept;

public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		void OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void OnTriggerExit(Collider* mine, Collider* other) noexcept override;

public:		bool GetUp() { return _up; };
public:		bool GetPress() { return _pressed; }
public:		void SetUp(bool b) { _up = b; }

private:	bool			_pressed;
private:	bool			_up;

private:	float			_deltaTime;
private:	float			_y;
private:	float			_originY;

private:	Collider*		_collider;
private:	Rigidbody*		_rigidbody;

private:	AudioAsset*		_press;
private:	AudioAsset*		_reset;

private:	EffectPlayer* _audioSource;
};

