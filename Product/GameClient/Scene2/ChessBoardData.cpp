#include "pch.h"
#include "ChessBoardData.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Transform.h"
#include "AtlasAnimation.h"
#include "ParticleSystem.h"
#include "Actor.h"

ChessBoardData::ChessBoardData(int index) noexcept
	:	_index(index)
{
}

void ChessBoardData::Start(void) noexcept
{
	/*Rigidbody* rigidbody = Rigidbody::Create(true);
	gameObject->AddComponent(rigidbody);

	Collider* collider = Collider::CreateSphereCollider(1.5f,XMVectorSet(0.f,1.5f,0.f,1.f));
	collider->SetIsTrigger(true);
	gameObject->AddComponent(collider);

	gameObject->SetTag(GameObjectTag::OBJECT);*/

	_smoke = GameObject::Instantiate("ChessBoardSmoke");
	_smoke->GetTransform()->SetParent(transform);
	_smoke->GetTransform()->SetLocalPosition(0, 0.1f, 0);
	AtlasAnimation::AtlasDesc desc;
	desc._fullPath = "Asset\\Texture\\smokeloop_01.dds";
	desc._billboard = false;
	desc._amount = XMFLOAT2(8, 8);
	desc._color = Vector4(0.2f, 0.2f, 0.2f, 1);
	desc._duration = 1;
	desc._emissiveColor = Vector4(0, 0, 0, 1);
	desc._emissivePath = "Asset\\Texture\\smokeloop_01.dds";
	desc._fadeOut = false;
	desc._lerpSize = XMFLOAT2(1, 1);
	desc._lerpType = CETween::EaseType::easeLiner;
	desc._renderPass = SpriteRenderer::RenderOption::ATLASANIM;
	desc._size = XMFLOAT2(5, 5);
	desc._speed = 60.f;
	desc._offsetVector = XMVectorSet(XMConvertToRadians(90.f), 0, 0, 1);

	_animator = new AtlasAnimation(desc, true);
	_smoke->AddComponent(_animator);
	_smoke->SetActive(false);


	_particleObject = GameObject::Instantiate("ChessBoardParticle");
	_particleObject->GetTransform()->SetParent(transform);
	_particleObject->GetTransform()->SetLocalPosition(0, 0.5f, 0);
	

	_particle = ParticleSystem::Create();
	_particle->SetStartSize(0.5f);
	_particle->SetEndSize(0.0f);
	_particle->SetLifeTime(3.f);
	_particle->SetRange(Vector3::one * 2.f);
	_particle->SetParticlePerSecond(0.5f);
	_particle->SetVelocity(2.f);
	_particle->SetStartColor(Vector4(0.3f, 0.3f, 1, 1.f));
	_particle->SetEndColor(Vector4(0, 1, 1, 0.1f));
	_particle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\Point12.dds"));
	_particle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\Point12_C4.dds"));
	_particle->SetColorLerpType(CETween::EaseType::easeLiner);
	_particle->SetEndEmissiveColor(Vector4(0, 0, 1, 1.f));
	_particle->SetStartEmissiveColor(Vector4(0, 1, 1, 0.4f));
	_particle->SetPass(ParticleSystem::PASS::ONEBLEDING);

	_particleObject->AddComponent(_particle);
	_particleObject->SetLayer(GameObjectLayer::TRANSLUCENT);

	_reverse = false;

}

void ChessBoardData::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	if (false == _play)
		return;
	GameObject* obj = other->GetGameObject();

	if (GameObjectTag::PLAYER == obj->GetTag())
	{
		obj->GetComponent<Actor>()->GetHit(2);
	}
}

void ChessBoardData::Update(float fElapsedTime) noexcept
{
	if (false == _play)
		return;

	_lifeTime += fElapsedTime;
	_lerpTime += fElapsedTime;

	if (false == _reverse)
	{
		_lerpAlpha = CETween::Lerp(0, 1, std::min(1.f, _lerpTime), CETween::EaseType::easeLiner);
		_animator->SetAlpha(_lerpAlpha);

		if (_lifeTime >= 2.f)
		{
			_reverse = true;
			_lerpTime = 0.f;
			_lifeTime = 0.f;
			_particle->Stop();
		}
	}
	else
	{
		_lerpAlpha = CETween::Lerp(1, 0, std::min(1.f, _lerpTime), CETween::EaseType::easeLiner);
		_animator->SetAlpha(_lerpAlpha);

		if (_lifeTime >= 2.f)
			Stop();
	}

	return;
}

void ChessBoardData::Play(void) noexcept
{
	_play = true;
	_reverse = false;
	_lifeTime = 0.f;
	_lerpTime = 0.f;
	_lerpAlpha = 0.f;
	_smoke->SetActive(true);
	_particle->Play();
}

bool ChessBoardData::isPlaying(void) noexcept
{
	return _smoke->GetActive();
}

void ChessBoardData::Stop(void) noexcept
{
	_play = false;
	_smoke->SetActive(false);
	_lifeTime = 0.f;
	_lerpTime = 0.f;
	_lerpAlpha = 0.f;
	_particle->Stop();
	_animator->SetAlpha(0);
}
