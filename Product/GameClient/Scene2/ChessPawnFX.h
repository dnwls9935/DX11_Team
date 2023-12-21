#pragma once

#include "Behaviour.h"

namespace ce
{
	class SpriteRenderer;
}

class ChessPawnFX : public Behaviour
{
public:		ChessPawnFX(Transform* pawn) noexcept;
public:		virtual ~ChessPawnFX(void) noexcept = default;

public:		void	Start(void) noexcept;
public:		void	Update(float fElapsedTime) noexcept;

public:		void	StartFX(void) noexcept;
public:		void	StopFX(void) noexcept;


private:	void	SetUpMarkBottomCircle(void) noexcept;
private:	void	SetUpMarkBottomRotLarge(void) noexcept;
private:	void	SetUpMarkBottomRotSmall(void) noexcept;
private:	void	SetUpMarkPolar(void) noexcept;

private:	Transform*		_parent;

private:	GameObject*		_markPolar[5];
private:	GameObject*		_markBottomCircle;
private:	GameObject*		_markBottomRotLarge;
private:	GameObject*		_markBottomRotSmall0;
private:	GameObject*		_markBottomRotSmall1;

private:	Transform*		_markPolarTransform[5];
private:	Transform*		_markBottomCircleTransform;
private:	Transform*		_markBottomRotLargeTransform;
private:	Transform*		_markBottomRotSmall0Transform;
private:	Transform*		_markBottomRotSmall1Transform;


private:	SpriteRenderer* _markPolarRenderer[5];
private:	SpriteRenderer* _markBottomCircleRenderer;
private:	SpriteRenderer* _markBottomRotLargeRenderer;
private:	SpriteRenderer* _markBottomRotSmall0Renderer;
private:	SpriteRenderer* _markBottomRotSmall1Renderer;

private:	bool			_start;
private:	float			_lerpTime;
private:	float			_lerpSize;

private:	Vector4			_color;

};

