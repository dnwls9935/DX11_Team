#pragma once

#include "Behaviour.h"

namespace ce
{
	class ModelRenderer;
	class SpriteRenderer;
}

class AtlasAnimation;

class CycloneFx : public Behaviour
{
public:		CycloneFx(void) noexcept;
public:		virtual ~CycloneFx(void) noexcept;

public:		void	Start(void) noexcept;
public:		void	Update(float fElapsedTime) noexcept;

public:		void	StartFx(void) noexcept;
public:		void	StopFx(void) noexcept;
public:		bool	GetUse(void) noexcept { return _use; }

private:	bool				_use;

private:	GameObject*			_flame[3];
private:	ModelRenderer*		_rendererFlame[3];

private:	XMFLOAT2			_uv;
private:	float				_frameTime;
private:	XMFLOAT3			_scrollSpeeds;
private:	XMFLOAT3			_scales;

private:	XMFLOAT2			_distortion[3];
private:	float				_distortionScale;
private:	float				_distortionBias;


private:	GameObject*			_ring[4];
private:	AtlasAnimation*		_rendererRing[4];
};

