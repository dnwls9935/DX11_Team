#include "pch.h"
#include "AtlasAnimation.h"

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Transform.h"

AtlasAnimation::AtlasAnimation(AtlasDesc desc, bool loop) noexcept
	: _desc(desc)
	, _loop(loop)
	, _isEnd(false)
{
}

AtlasAnimation::~AtlasAnimation(void) noexcept
{
}

void AtlasAnimation::Start(void) noexcept
{
	_renderer = SpriteRenderer::Create(_desc._billboard);
	_renderer->SetTexture(ASSETMANAGER->GetTextureData(_desc._fullPath.c_str()));
	gameObject->AddComponent(_renderer);

	_renderer->SetSize(_desc._size.x, _desc._size.y);
	_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, _desc._color);
	_renderer->SetPassType(_desc._renderPass);

	gameObject->SetLayer(GameObjectLayer::TRANSLUCENT);

	transform->SetLocalEulerAngle(_desc._offsetVector);

	_u = 0;
	_v = 0;

	if(false == _loop)
		gameObject->SetActive(false);
	else
		gameObject->SetActive(true);


	if (0 < _desc._emissivePath.length())
	{
		_renderer->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData(_desc._emissivePath.c_str()));
		_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_desc._emissiveColor.x, _desc._emissiveColor.y, _desc._emissiveColor.z, _desc._emissiveColor.w));
	}


	XMFLOAT2 uv = { _u, _v };
	_renderer->SetAtlasOption(uv, _desc._amount);
}

void AtlasAnimation::Update(float fElapsedTime) noexcept
{
	if(false == _loop)
		_accTime += fElapsedTime;

	if (_accTime >= _desc._duration)
	{
		_accTime = 0.f;
		_u = 0;
		_v = 0;
		XMFLOAT2 uv = { _u, _v };
		_renderer->SetAtlasOption(uv, _desc._amount);
		gameObject->SetActive(false);

		_lerpTime = 0.f;
		_alpha = 1.f;
		sizeX = _desc._size.x;
		sizeY = _desc._size.y; 
		
		_isEnd = true;
		return;
	}


	_u += fElapsedTime * _desc._speed;

	if (_u >= _desc._amount.x)
	{
		_u = 0.f;
		_v++;
	}

	if (_v >= _desc._amount.y)
	{
		_u = 0;
		_v = 0;
	}


	if (true == _desc._fadeOut)
	{
		_lerpTime += fElapsedTime * 1.5f;
		_alpha = CETween::Lerp(1, 0, _lerpTime, _desc._lerpType);
		SetAlpha(_alpha);

		sizeX = CETween::Lerp(_desc._size.x, _desc._lerpSize.x, _lerpTime, _desc._lerpType);
		sizeY = CETween::Lerp(_desc._size.y, _desc._lerpSize.y, _lerpTime, _desc._lerpType);
		_renderer->SetSize(sizeX, sizeY);

	}

	XMFLOAT2 uv = { _u, _v };
	_renderer->SetAtlasOption(uv, _desc._amount);

	//_renderer->GetMaterial()->GetShader().SetRawValue("emissiveColor", &_desc._emissiveColor, sizeof(float) * 4);
}

bool AtlasAnimation::GetAnimationEnd(void) noexcept
{
	return _isEnd;
}

void AtlasAnimation::ResetUV()
{
	_isEnd = false;

	gameObject->SetActive(true);
	_u = 0;
	_v = 0;
	_accTime = 0.f;
}

void AtlasAnimation::SetAlpha(float alpha)
{
	if (alpha <= 0.05f)
		alpha = 0.f;

	_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_desc._color.x, _desc._color.y, _desc._color.z, alpha));

	if(0 < _desc._emissivePath.length())
		_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_desc._emissiveColor.x, _desc._emissiveColor.y, _desc._emissiveColor.z, alpha));
}

void AtlasAnimation::SetSize(XMFLOAT2 size)
{
	_renderer->SetSize(size.x, size.y);
}

void AtlasAnimation::SetSpeed(float speed)
{
	_desc._speed = speed;
}

void AtlasAnimation::SetDuration(float duration)
{
	_desc._duration = duration;
}

void AtlasAnimation::SetColor(Vector4 color)
{
	_desc._color = color;
	_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_desc._color.x, _desc._color.y, _desc._color.z, _desc._color.w));
}

void AtlasAnimation::SetEmissiveColor(Vector4 color)
{
	_desc._emissiveColor = color;
	_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_desc._emissiveColor.x, _desc._emissiveColor.y, _desc._emissiveColor.z, _desc._emissiveColor.w));
}
