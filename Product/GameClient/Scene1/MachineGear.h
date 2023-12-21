#pragma once

#include "Behaviour.h"

namespace ce
{
	class Rigidbody;
}
class EffectPlayer;

class MachineGear : public Behaviour
{
public:		MachineGear(void) noexcept;
public:		virtual ~MachineGear(void) noexcept;

public:		void Start(void) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		void SetBtn(GameObject* btnA, GameObject* btnB) { _btnA = btnA; _btnB = btnB; };
public:		const bool GetRunning() const { return _rotate; }

public:		void OnEndStart(void) noexcept;

private:	bool	_rotate;
private:	GameObject* _btnA;
private:	GameObject* _btnB;


private:	Collider* _collider;
private:	Rigidbody* _rigidbody;

private:	AudioAsset* _start;
private:	AudioAsset* _loop;
private:	AudioAsset* _cogLoop;

private:	EffectPlayer* _cogAudioSource;
private:	EffectPlayer* _gearAudioSource;

};

