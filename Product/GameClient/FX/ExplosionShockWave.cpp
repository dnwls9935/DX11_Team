#include "pch.h"
#include "ExplosionShockWave.h"
#include "GameObject.h"
#include "Transform.h"
#include "SpriteRenderer.h"

ExplosionShockWave::ExplosionShockWave(void) noexcept
{
}

void ExplosionShockWave::Start(void) noexcept
{
	transform->SetLocalPosition(0, 0, 0);

	_renderer = SpriteRenderer::Create(true);
	gameObject->AddComponent(_renderer);

	ASSETMANAGER->LoadFile("Asset\\Texture\\Explosion\\SphereNormal.dds");
	ASSETMANAGER->LoadFile("Asset\\Texture\\Explosion\\shockwave.dds");

	_renderer->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Explosion\\SphereNormal.dds"));
	_renderer->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_ALPHA, ASSETMANAGER->GetTextureData("Asset\\Texture\\Explosion\\shockwave.dds"));
	_renderer->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_DISTORTION, ASSETMANAGER->GetTextureData("Asset\\Texture\\Explosion\\SphereNormal.dds"));
	_renderer->SetPassType(SpriteRenderer::RenderOption::DISTORTION);

	float distortionLV = 0;
	_renderer->GetMaterial()->SetValue(ShaderSemantic::Type::FLOAT_DISTORTION, &distortionLV, sizeof(float));
	_renderer->SetSize(0, 0);
	_renderer->SetEnable(false);
	
	gameObject->SetLayer(GameObjectLayer::TRANSLUCENT);
}

void ExplosionShockWave::Update(float fElapsedTime) noexcept
{
	if (false == _start)
		return;

	_lerpTime += fElapsedTime;

	float size = CETween::Lerp(0, 100, std::min(0.3f, _lerpTime), CETween::EaseType::easeOutQuint);

	_renderer->SetSize(size, size);

	if (_lerpTime >= 0.3f)
	{
		_start = false;
		_renderer->SetSize(0, 0);
		_renderer->SetEnable(false);
		_lerpTime = 0;
	}
}

void ExplosionShockWave::StartFX(void) noexcept
{
	_start = true;
	_renderer->SetEnable(true);
}