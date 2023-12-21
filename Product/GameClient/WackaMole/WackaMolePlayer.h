#pragma once

#include "Behaviour.h"

namespace ce
{
	class ModelRenderer;
	class Rigidbody;
	class Collider;
	class Animator;
}

class InputHandler;
class WackaInput;
class ScoreBanner;
class MiniGameUI;

class WackaMolePlayer : public Behaviour
{
public:		enum class TYPE {MAY, CODY, END};
public:		enum class DIRECTION { UP, DOWN, LEFT, RIGHT, END };

public:		WackaMolePlayer(TYPE type) noexcept;
public:		virtual ~WackaMolePlayer(void) noexcept;
public:		void	Start(void) noexcept override;
public:		void	FixedUpdate(float fElapsedTime) noexcept override;
public:		void	Update(float fElaspedTime) noexcept override;
public:		void	LateUpdate(float fElapsedTime) noexcept override;

public:		void	OnTriggerEnter(Collider* mine, Collider* other) noexcept override PURE;
public:		void	OnTriggerExit(Collider* mine, Collider* other) noexcept override PURE;

public:		virtual void	AnimationUp(void) noexcept;
public:		virtual void	AnimationDown(void) noexcept;
public:		virtual void	AnimationLeft(void) noexcept;
public:		virtual void	AnimationRight(void) noexcept;
public:		virtual void	AnimationIdle(void) noexcept { __noop; };

public:		virtual void	PadAnimationUp(void) noexcept;
public:		virtual void	PadAnimationDown(void) noexcept;
public:		virtual void	PadAnimationLeft(void) noexcept;
public:		virtual void	PadAnimationRight(void) noexcept;
public:		virtual void	PadAnimationIdle(void) noexcept { __noop; };


public:		void			OnStartAnimation(void) noexcept;
public:		void			OnEndAnimation(void) noexcept;
public:		bool			GetAnimation(void) noexcept;
public:		int				GetScore(void) noexcept { return _score; };
public:		TYPE			GetType(void) noexcept { return _type; };

protected:	TYPE			_type;

protected:	bool			_isDirty;
protected:	DIRECTION		_direction;
protected:	DIRECTION		_dirtyDirection;

protected:	WackaInput*	_inputHandler;
protected:	ModelRenderer*	_renderer;
protected:	Rigidbody*		_rigid;
protected:	Collider*		_collider;
protected:	Animator*		_animator;

protected:	bool			_isAnimating;

protected:	bool			_checkKey;
protected:	int				_score;

protected:	ScoreBanner*	_scoreBanner;
};

