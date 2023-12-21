#pragma once
#include "Behaviour.h"
#include "INoncopyable.h"
#include "SpriteRenderer.h"
#include "SpriteAnimator.h"
#include "SpriteAnimation.h"
#include "GameObject.h"

class AnimateSpriteTrail final : public Behaviour, INoncopyable
{
public:		AnimateSpriteTrail( SpriteAnimation*	animation
								, const uint32		poolSize
								, const float		emitTime
								, const bool		isPreLoad) noexcept;
public:		virtual ~AnimateSpriteTrail(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		FixedUpdate(float fElapsedTime) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		LateUpdate(float fElapsedTime) noexcept override;

public:		void		OnEnable(void) noexcept override;
public:		void		OnDisable(void) noexcept override;

public:		void		OnEndAniXXX(const uint32 index) noexcept;

public:		void		Play(void) noexcept;
public:		void		Stop(void) noexcept;
public:		void		StopImmediate(void) noexcept;

private:	const float				_emitTime;
private:	float					_deltaTime;

private:	const uint32			_poolSize;
private:	uint32					_index;

private:	bool					_playing;

private:	GameObject::TList		_objs;
private:	SpriteRenderer::TList	_renderers;
private:	SpriteAnimator::TList	_animators;
};

