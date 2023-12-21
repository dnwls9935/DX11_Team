#pragma once

#include "Behaviour.h"
#include "GameObject.h"
#include "Transform.h"
#include "Animator.h"
#include "FuseLight.h"

namespace ce
{
	class HierarcyNode;
	class ParticleSystem;
}

class EffectPlayer;
class AnimateSpriteTrail;

class FuseCrab : public Behaviour
{

public:		FuseCrab(int sequence) noexcept;
public:		virtual ~FuseCrab(void) noexcept;


public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		void StartAnimation() noexcept;
public:		void PickUp() noexcept ;
public:		void SetHierarcyNode(HierarcyNode* hierarcynode) noexcept { _playerAlign = hierarcynode; }
public:		void SetInteractTransform(Transform* playerTransform) noexcept { _playerTransform = playerTransform; }
public:		void ClearTarget(void) noexcept;
public:		void SetbPickUp(bool bpickup) noexcept { _pickUp = bpickup; }
public:		void PlayInsertSound() noexcept;
public:		void PlayLightSound() noexcept;

private:	void OnIntroAnimationFinished();
private:	void OnSlideAnimationFinished();

private:	int					_sequence;

private:	bool				_pickUp;

private:	Animator*			_animator;

private:	XMFLOAT4			_defaultPosition;
private:	XMFLOAT4			_defaultRotation;

private:	Collider*			_collider;
private:	Rigidbody*			_rigid;

private:	HierarcyNode*		_playerAlign;
private:	Transform*			_playerTransform;

private:	FuseLight*			_fuselight;

private:	AnimateSpriteTrail* _spriteanimaion;
private:	ParticleSystem*		_smokeparticle;

private:	AudioAsset*			_insertaudio;
private:	AudioAsset*			_lightaudio;
private:	AudioAsset*			_jumpoutaudio;

private:	EffectPlayer*		_audioSource;
};



