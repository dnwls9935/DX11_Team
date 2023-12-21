#include "pch.h"
#include "MageMissile.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "SpriteRenderer.h"
#include "ParticleSystem.h"
#include "Light.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

MageMissile::MageMissile(Vector3 direction) noexcept
	: Projectile(direction, 3, GameObjectTag::PLAYER, 5.f)
	, _deltaTime(0)
{
}

void MageMissile::Start(void) noexcept
{
	_spriteRenderer = SpriteRenderer::Create(true);
	_spriteRenderer->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\T_Ring_01.dds"));
	_spriteRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.5f, 0, 1, 1));
	_spriteRenderer->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\T_Ring_01_emissive.dds"));
	//_spriteRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1, 1, 1, 0.4f));
	_spriteRenderer->SetSize(0.2f,0.2f);
	_spriteRenderer->SetPassType(SpriteRenderer::RenderOption::EFFECTBLEND);

	gameObject->AddComponent(_spriteRenderer);
	gameObject->SetLayer(GameObjectLayer::TRANSLUCENT);

	Rigidbody* rigidbody = Rigidbody::Create(false);
	rigidbody->SetIsKinematic(true);
	gameObject->AddComponent(rigidbody);

	_collider = Collider::CreateSphereCollider(0.5f);
	_collider->SetIsTrigger(true);
	gameObject->AddComponent(_collider);
	_collider->SetEnable(false);

	/* Light */
	Light* light = Light::CreatePoint();
	light->SetRange(2.f);
	light->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.5f, 0, 1, 1));
	gameObject->AddComponent(light);

	GameObject* obj = GameObject::Instantiate();
	obj->GetTransform()->SetParent(transform);
	obj->GetTransform()->SetLocalPosition(0, 0, -1);

	_cloudParticle = ParticleSystem::Create();
	_cloudParticle->SetStartSize(1.5f);
	_cloudParticle->SetEndSize(1.f);
	_cloudParticle->SetLifeTime(1.f);
	_cloudParticle->SetRange(Vector3::one * 0.2f);
	_cloudParticle->SetParticlePerSecond(0.01f);
	_cloudParticle->SetVelocity(0.5f);
	_cloudParticle->SetStartColor(Vector4(0.7f,0.5f, 1, 1));
	_cloudParticle->SetEndColor(Vector4(0.4f, 0, 0.7f, 1));
	_cloudParticle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
		, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\SoftCloud\\SoftCloud_0.dds"));

	obj->AddComponent(_cloudParticle);
	obj->SetLayer(GameObjectLayer::TRANSLUCENT);

	obj = GameObject::Instantiate();
	obj->GetTransform()->SetParent(transform);
	obj->GetTransform()->SetLocalPosition(0, 0, -1);

	_impact = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\ToyMage\\Play_Gameplay_Abilities_Spells_ToyMage_MagicBalls_Impact.ogg");

	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);

	//Rotate
	__super::Start();

}

void MageMissile::Update(float fElapsedTime) noexcept
{

	if (_isReady)
	{
		__super::Update(fElapsedTime);

		if (!_effectPlayer->GetAudioSource()->IsPlaying() && _shot)
			DestroyObject();
	}
	else
	{
		_deltaTime += fElapsedTime;
		if (_deltaTime >= 2.f)
		{
			_deltaTime = 2.f;
			_isReady = true;
			_cloudParticle->Play(true);
			_collider->SetEnable(true);
		}

		float size = CETween::Lerp(0.2f, 1.f, std::min(1.f, _deltaTime));
		_spriteRenderer->SetSize(size, size);

	}
}

void MageMissile::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{

	if (other->GetGameObject()->GetTag() == _enemyTag)
	{
		other->GetGameObject()->GetComponent<Actor>()->GetHit(_damage);
	}

	if (other->GetGameObject()->GetTag() == GameObjectTag::DEFAULT ||
		other->GetGameObject()->GetTag() == _enemyTag)
	{
		_shot = true;
		_effectPlayer->Play(_impact, 0.1f, false, nullptr, false, true);
		gameObject->SetActive(false);
	}
}
