#pragma once

#include "Behaviour.h"

class AtlasAnimationX;

class TinyExplosion : public Behaviour
{
public:		TinyExplosion(void) noexcept;
public:		virtual ~TinyExplosion(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	OnDestroy(void) noexcept override;

public:		void	StartFX(void) noexcept;
	  
private:	GameObject*			_tinyExplosionObject[6];
private:	Transform*			_tinyExplosionTransform[6];
private:	AtlasAnimationX*	_tinyExplosionAnimation[6];

private:	float				_lerpTimeAcc;

private:	bool				_start;
private:	int					_lerpType;
};

