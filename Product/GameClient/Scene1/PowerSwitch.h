#pragma once

#include "Behaviour.h"
#include "GameObject.h"
#include "Transform.h"

class EffectPlayer;

class PowerSwitch : public Behaviour
{
public:		PowerSwitch(void) noexcept;
public:		virtual ~PowerSwitch(void) noexcept;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		void OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void OnTriggerExit(Collider* mine, Collider* other) noexcept override;

public:		float GetCurrentAngle() { return _currentAngle; }

public:		XMMATRIX GetCodyGrabMatrix();
public:		XMMATRIX GetMayGrabMatrix();

public:		void SetLower(int index, bool b) { _check[index] = b; }


private:	bool _lower;
private:	bool _oneGrab;
private:	float _lowerAngle;
private:	float _currentAngle;

private:	XMVECTOR _defaultPosition;
private:	XMVECTOR _defaultRotation;

private:	bool _check[2];


private:	GameObject* _mayGrab;
private:	GameObject* _codyGrab;

private:	Collider* _collider;
private:	Rigidbody* _rigid;

private:	AudioAsset*		_doubleinteract;
private:	AudioAsset*		_activated;
private:	AudioAsset*		_oneinteract;

private:	EffectPlayer*	_audioSource;

};
