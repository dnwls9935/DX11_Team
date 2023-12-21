#pragma once

#include "Behaviour.h"

namespace ce
{
	class SpriteRenderer;
}

#include "SpriteRenderer.h"

class AtlasAnimation : public Behaviour
{
public:		struct AtlasDesc 
			{
				std::string			_fullPath;
				XMFLOAT2			_amount;
				XMFLOAT2			_size;
				Vector4				_color;
				XMVECTOR			_offsetVector;
				XMMATRIX			_offsetMatrix;

				std::string			_emissivePath;
				XMFLOAT4			_emissiveColor;

				float				_speed;
				float				_duration;

				bool				_billboard;

				bool							_fadeOut = false;
				CETween::EaseType				_lerpType = CETween::EaseType::easeLiner;
				XMFLOAT2						_lerpSize;

				SpriteRenderer::RenderOption	_renderPass = SpriteRenderer::RenderOption::ATLASANIM_MSAA;
			};

public:		AtlasAnimation(AtlasDesc desc, bool loop = false) noexcept;
public:		virtual ~AtlasAnimation(void) noexcept;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;

public:		bool	GetAnimationEnd(void) noexcept;
public:		void	ResetUV();

public:		void	SetAlpha(float alpha);
public:		void	SetSize(XMFLOAT2 size);
public:		void	SetSpeed(float speed);
public:		void	SetDuration(float duration);
public:		void	SetColor(Vector4 color);
public:		void	SetEmissiveColor(Vector4 color);

public:		void	FadeOut(bool fadeOut) { _desc._fadeOut = fadeOut; };

private:		AtlasDesc			_desc;

private:		float				_u;
private:		float				_v;

private:		SpriteRenderer*		_renderer;
private:		float				_accTime;
private:		bool				_loop;
private:		bool				_isEnd;

private:		float				_lerpTime;
private:		float				_alpha;
private:		float				sizeX;
private:		float				sizeY;
};

