#pragma once

#include "Behaviour.h"

namespace ce
{
	class SpriteRenderer;
}

#include "SpriteRenderer.h"

class AtlasAnimationX : public Behaviour
{
public:		struct AtlasDesc 
			{
				std::string						_fullPath;
				XMFLOAT2						_amount;
				XMFLOAT2						_size;
				Vector4							_color;
				XMVECTOR						_offsetVector;
				XMMATRIX						_offsetMatrix;
				std::string						_emissivePath;
				XMFLOAT4						_emissiveColor;
				float							_speed;
				bool							_billboard;
				CETween::EaseType				_lerpType = CETween::EaseType::easeLiner;
				XMFLOAT2						_lerpSize;
				SpriteRenderer::RenderOption	_renderPass = SpriteRenderer::RenderOption::ATLASANIM_MSAA;
				bool							_fadeOut = false;
				float							_fadeOutSpeed;
			};

public:		AtlasAnimationX(AtlasDesc desc, bool loop = false) noexcept;
public:		virtual ~AtlasAnimationX(void) noexcept;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		bool		GetAnimationEnd(void) noexcept;
public:		void		ResetUV() noexcept;

public:		void		SetBillboadEularAngle(Vector3 angle) noexcept;
public:		void		SetBillboadEularAngle(float x, float y, float z) noexcept;
public:		void		SetAlpha(float alpha) noexcept;
public:		void		SetSize(XMFLOAT2 size) noexcept;
public:		void		SetLerpSize(XMFLOAT2 size) noexcept;
public:		void		SetLerpType(CETween::EaseType type) noexcept;
public:		void		SetSpeed(float speed) noexcept;
public:		void		SetFadeSpeed(float speed) noexcept;
public:		void		SetColor(Vector4 color) noexcept;
public:		void		SetEmissiveColor(Vector4 color) noexcept;
public:		void		SetRenderPass(SpriteRenderer::RenderOption pass) noexcept;
public:		void		SetFadeOut(bool fadeOut) noexcept;
public:		void		SetUV(XMFLOAT2 uv) noexcept;

public:		float		GetAnimationPercent(void) noexcept;
public:		float		GetAlpha(void) noexcept { return _desc._color.w; }
public:		XMFLOAT2	GetSize(void) noexcept { return _desc._size; }
public:		XMFLOAT2	GetUV(void) noexcept { return XMFLOAT2(_u, _v); }

protected:	AtlasDesc			_desc;

protected:	float				_u;
protected:	float				_v;

protected:	SpriteRenderer*		_renderer;
protected:	bool				_loop;
protected:	bool				_isEnd;

protected:	float				_lerpTime;
protected:	float				_alpha;
protected:	float				_sizeX;
protected:	float				_sizeY;
protected:	float				_percent;
};

