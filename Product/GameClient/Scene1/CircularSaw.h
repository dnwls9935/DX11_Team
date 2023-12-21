#pragma once

#include "Behaviour.h"
#include "GameObject.h"

namespace ce
{
	class Animator;
	class Rigidbody;
}
class EffectPlayer;

class CircularSaw : public Behaviour
{
public:		enum class PLAYER {MAY, CODY, END};
public:		enum class eAudioType{ SUCCESS, SPARK, START, LOOP, END};

public:		CircularSaw(void) noexcept;
public:		virtual ~CircularSaw(void) noexcept;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		void OnSuccessAniamtionEnd() noexcept;
public:		void OnIdleRunAimation() noexcept;
public:		void OnEndStartAudio() noexcept;

public:		void SetAwake(PLAYER player, bool awake) { _awake[(int)player] = awake; };

public:		XMMATRIX GetGrabMatrix(PLAYER player);
public:		bool	GetRun() { return _run; };


private:	XMVECTOR _defaultRotation;

private:	bool	_awake[2];
private:	bool	_run;
private:	bool	_start;

private:	Animator* _animator;

private:	GameObject* _mayGrab;
private:	GameObject* _codyGrab;

private:	Collider* _collider;
private:	Rigidbody* _rigid;

private:	AudioAsset* _audioAsset[static_cast<uint>(eAudioType::END)];
private:	EffectPlayer* _audioSource;
};

