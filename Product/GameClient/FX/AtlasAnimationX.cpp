#include "pch.h"
#include "AtlasAnimationX.h"

#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Transform.h"

AtlasAnimationX::AtlasAnimationX(AtlasDesc desc, bool loop) noexcept
	: _desc(desc)
	, _loop(loop)
	, _isEnd(false)
{
}

AtlasAnimationX::~AtlasAnimationX(void) noexcept
{
}

void AtlasAnimationX::Start(void) noexcept
{
	if (ASSETMANAGER->GetTextureData(_desc._fullPath.c_str()) == nullptr)
	{
		ASSETMANAGER->LoadFile(_desc._fullPath.c_str());
	}

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
		if (ASSETMANAGER->GetTextureData(_desc._emissivePath.c_str()) == nullptr)
		{
			ASSETMANAGER->LoadFile(_desc._emissivePath.c_str());
		}
		_renderer->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData(_desc._emissivePath.c_str()));
		_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_desc._emissiveColor.x, _desc._emissiveColor.y, _desc._emissiveColor.z, _desc._emissiveColor.w));
	}

	XMFLOAT2 uv = { _u, _v };
	_renderer->SetAtlasOption(uv, _desc._amount);
}

void AtlasAnimationX::Update(float fElapsedTime) noexcept
{
	if (true == _isEnd)
		return;

	_u += fElapsedTime * _desc._speed;

	if (_u >= _desc._amount.x)
	{
		_u = 0.f;
		_v++;

		if (_v >= _desc._amount.y)
		{
			_u = 0;
			_v = 0;
			if (false == _loop)
			{
				_u = 0;
				_v = 0;
				XMFLOAT2 uv = { _u, _v };
				_renderer->SetAtlasOption(uv, _desc._amount);
				_lerpTime = 0.f;
				_alpha = 1.f;
				_sizeX = _desc._size.x;
				_sizeY = _desc._size.y;

				SetAlpha(0);
				_renderer->SetSize(0, 0);

				_isEnd = true;
				return;
			}
		}
	}

	if (true == _desc._fadeOut)
	{
		_lerpTime += (fElapsedTime *_desc._fadeOutSpeed );
		_alpha = CETween::Lerp(1, 0, _lerpTime, _desc._lerpType);
		SetAlpha(_alpha);

		_sizeX = CETween::Lerp(_desc._size.x, _desc._lerpSize.x, _lerpTime, _desc._lerpType);
		_sizeY = CETween::Lerp(_desc._size.y, _desc._lerpSize.y, _lerpTime, _desc._lerpType);
		_renderer->SetSize(_sizeX, _sizeY);
	}
	else {
		SetAlpha(1);
		_renderer->SetSize(_desc._size.x, _desc._size.y);
	}

	XMFLOAT2 uv = { _u, _v };
	_renderer->SetAtlasOption(uv, _desc._amount);

	//_renderer->GetMaterial()->GetShader().SetRawValue("emissiveColor", &_desc._emissiveColor, sizeof(float) * 4);
}

bool AtlasAnimationX::GetAnimationEnd(void) noexcept
{
	return _isEnd;
}

void AtlasAnimationX::ResetUV() noexcept
{
	_isEnd = false;

	gameObject->SetActive(true);
	_u = 0;
	_v = 0;
}

void AtlasAnimationX::SetBillboadEularAngle(Vector3 angle) noexcept
{
	_renderer->SetAdditiveBillboardEuler(angle);
}

void AtlasAnimationX::SetBillboadEularAngle(float x, float y, float z) noexcept
{
	_renderer->SetAdditiveBillboardEuler(x, y, z);
}

void AtlasAnimationX::SetAlpha(float alpha) noexcept
{
	if (alpha <= 0.05f)
		alpha = 0.f;

	_desc._color.w = alpha;
	_desc._emissiveColor.w = alpha;

	_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_desc._color.x, _desc._color.y, _desc._color.z, alpha));

	if(0 < _desc._emissivePath.length())
		_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_desc._emissiveColor.x, _desc._emissiveColor.y, _desc._emissiveColor.z, alpha));
}

void AtlasAnimationX::SetSize(XMFLOAT2 size) noexcept
{
	_desc._size = size;
	_renderer->SetSize(size.x, size.y);
}

void AtlasAnimationX::SetLerpSize(XMFLOAT2 size) noexcept
{
	_desc._lerpSize = size;
}

void AtlasAnimationX::SetLerpType(CETween::EaseType type) noexcept
{
	_desc._lerpType = type;
}

void AtlasAnimationX::SetSpeed(float speed) noexcept
{
	_desc._speed = speed;
}

void AtlasAnimationX::SetFadeSpeed(float speed) noexcept
{
	_desc._fadeOutSpeed = speed;
}

void AtlasAnimationX::SetColor(Vector4 color) noexcept
{
	_desc._color = color;
	_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_desc._color.x, _desc._color.y, _desc._color.z, _desc._color.w));
}

void AtlasAnimationX::SetEmissiveColor(Vector4 color) noexcept
{
	_desc._emissiveColor = color;
	_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_desc._emissiveColor.x, _desc._emissiveColor.y, _desc._emissiveColor.z, _desc._emissiveColor.w));
}

void AtlasAnimationX::SetRenderPass(SpriteRenderer::RenderOption pass) noexcept
{
	_desc._renderPass = pass;
	_renderer->SetPassType(_desc._renderPass);
}

float AtlasAnimationX::GetAnimationPercent(void) noexcept
{
	if (false == gameObject->GetActive())
		return 0;

	float max = _desc._amount.x * _desc._amount.y;
	float current = _u + (_desc._amount.x * _v);


	return current / max;
}

void AtlasAnimationX::SetFadeOut(bool fadeOut) noexcept
{
	_desc._fadeOut = fadeOut;
}

void AtlasAnimationX::SetUV(XMFLOAT2 uv) noexcept
{
	_u = uv.x;
	_v = uv.y;
}
