#include "pch.h"
#include "NoiseFireFX.h"
#include "SpriteRenderer.h"
#include "GameObject.h"
#include "Transform.h"

NoiseFireFX::NoiseFireFX(void) noexcept
{

}

void NoiseFireFX::Start(void) noexcept
{
	_renderer = SpriteRenderer::Create();
	_renderer->SetBillboard(true);

	_renderer->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Test\\fire01.dds"));
	_renderer->GetMaterial()->SetTexture( ShaderSemantic::Type::TEXTURE_EMISSIVE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\Test\\fire01.dds"));
	_renderer->GetMaterial()->SetTexture( ShaderSemantic::Type::TEXTURE_NOISE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\Test\\noise01.dds"));
	_renderer->GetMaterial()->SetTexture( ShaderSemantic::Type::TEXTURE_ALPHA
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\Test\\alpha01.dds"));
	_renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1, 1, 1, 1));
	_renderer->SetPassType(SpriteRenderer::RenderOption::EFFECT_NOISE);

	gameObject->AddComponent(_renderer);

	SpriteRenderer::EffectNoiseDesc enDesc;
	ZeroMemory(&enDesc, sizeof(SpriteRenderer::EffectNoiseDesc));

	enDesc.scrollSpeeds		= DirectX::XMFLOAT3(1.1f, 2.1f, 3.1f);
	enDesc.scales			= DirectX::XMFLOAT3(1, 4, 5);
	enDesc.distortion[0]	= DirectX::XMFLOAT2(0.1f, 0.4f);
	enDesc.distortion[1]	= DirectX::XMFLOAT2(0.1f, 0.3f);
	enDesc.distortion[2]	= DirectX::XMFLOAT2(0.1f, 0.2f);
	enDesc.distortionScale	= 0.5f;
	enDesc.distortionBias	= 0.8f;
	enDesc.setCheck			= true;

	_renderer->SetEffectNoiseDesc(enDesc);
	gameObject->SetActive(false);
}

void NoiseFireFX::FixedUpdate(float fElapsedTime) noexcept
{

}

void NoiseFireFX::Update(float fElapsedTime) noexcept
{
	if (_isPlaying)
	{
		_deltaTime += fElapsedTime;

		if (_deltaTime >= _endTime)
		{
			_isPlaying	= false;
			gameObject->SetActive(false);
		}
		else
		{
			float t = _deltaTime / _endTime;
			float scale = 1.3f - CETween::Lerp(0, 1.3f, t, CETween::EaseType::easeInQuart);

			transform->SetLocalScale(scale, scale, scale);
		}
	}
}

void NoiseFireFX::LateUpdate(float fElapsedTime) noexcept
{

}

void NoiseFireFX::OnEnable(void) noexcept
{

}

void NoiseFireFX::OnDisable(void) noexcept
{

}

void NoiseFireFX::Play(const float time) noexcept
{
	transform->SetParent(nullptr);
	transform->SetLocalEulerAngle(0, 0, 0);
	gameObject->SetLayer(GameObjectLayer::TRANSLUCENT);

	_isPlaying	= true;
	_deltaTime	= 0.0f;
	_endTime	= time;

	_renderer->SetPivotY(0.f);
	gameObject->SetActive(true);

	transform->SetLocalScale(1.3f, 1.3f, 1.3f);

	SpriteRenderer::EffectNoiseDesc enDesc;
	ZeroMemory(&enDesc, sizeof(SpriteRenderer::EffectNoiseDesc));

	enDesc.scrollSpeeds		= DirectX::XMFLOAT3(1.1f, 2.1f, 3.1f);
	enDesc.scales			= DirectX::XMFLOAT3(1, 4, 5);
	enDesc.distortion[0]	= DirectX::XMFLOAT2(0.1f, CE_MATH::Random(1, 5) * 0.1f);
	enDesc.distortion[1]	= DirectX::XMFLOAT2(0.1f, CE_MATH::Random(1, 5) * 0.1f);
	enDesc.distortion[2]	= DirectX::XMFLOAT2(0.1f, CE_MATH::Random(1, 5) * 0.1f);
	enDesc.distortionScale	= CE_MATH::Random(3, 10) * 0.1f;
	enDesc.distortionBias	= CE_MATH::Random(6, 10) * 0.1f;
	enDesc.setCheck			= true;

	_renderer->SetEffectNoiseDesc(enDesc);
}
