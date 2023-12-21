#include "pch.h"
#include "SquirrelTurretHolder.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "SpriteHP.h"
#include "Camera.h"
#include "Damaged.h"
#include "ExplosionFX.h"
#include "AudioSource.h"
#include "NarrationManager.h"
#include "EffectPlayer.h"

bool SquirrelTurretHolder::_isExplode = false;

SquirrelTurretHolder::SquirrelTurretHolder(void) noexcept
	: MonsterActor(3)
	, _playActiveAni(false)
	, _aniDeltaTime(0)
{

}

void SquirrelTurretHolder::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create(false);
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Tree\\Defense\\SquirrelTurret01Holder.fbx"));
	gameObject->AddComponent(renderer);
	XMMATRIX mat = XMMatrixScaling(0.01f, 0.01f, 0.01f)
					* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(0)
																				, CE_MATH::ToRadian(0)
																				, CE_MATH::ToRadian(0)));

	renderer->SetOffsetMatrix(mat);

	/* Rigidbody */
	Rigidbody* rigid = Rigidbody::Create(false);
	rigid->SetIsKinematic(true);
	rigid->SetUseGravity(false);
	gameObject->AddComponent(rigid);

	Collider* collider = Collider::CreateBoxCollider(4, 4, 4);
	gameObject->AddComponent(collider);

	/* HP Bar */
	_spriteHpBar = new SpriteHP(this, Vector4(1, 0, 0, 1));
	gameObject->AddComponent(_spriteHpBar);
	_spriteHpBar->Invisible();

	/* Dead Fx */
	GameObject* obj = GameObject::Instantiate("SquirrelTurrentDieFX");
	obj->AddComponent(_fx = new ExplosionFX());

	gameObject->SetTag(GameObjectTag::MONSTER);

	_originPos = transform->GetWorldPosition();
	transform->SetWorldPosition(XMVectorSet(_originPos.x, _originPos.y - 10, _originPos.z, 0));

	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);

	_audioAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage3\\Play_GP_Weapon_Cannon_Ball_Explode.ogg");
}

void SquirrelTurretHolder::Update(float fElapsedTime) noexcept
{
	if (_playerTransform == nullptr)
	{
		GameObject* player = GameObject::FindObjectByTag(GameObjectTag::PLAYER);

		if (player == nullptr)
			return;

		_playerTransform = player->GetTransform();
	}
	else
	{
		XMVECTOR dist = _playerTransform->GetWorldPosition() - transform->GetWorldPosition();
		if (XMVectorGetX(XMVector3Length(dist)) < 300)
		{
			if (!_visibleHP && !_playActiveAni)
			{
				_playActiveAni	= true;
			}
		}

		XMVECTOR direction = XMVector3Normalize(dist);
		XMVECTOR forward = XMVector3Normalize(transform->GetForward());

		float dot = XMVectorGetX(XMVector3Dot(direction, forward));
		float angle = acosf(dot);

		if (fabsf(angle) >= 0.2f)
		{
			XMVECTOR cross = XMVector3Cross(transform->GetForward(), direction);
			if (XMVectorGetY(cross) < 0)
				angle *= -1;

			transform->Rotate(0, angle - CE_MATH::ToRadian(90.f), 0, false);
		}
	}

}

void SquirrelTurretHolder::LateUpdate(float fElapsedTime) noexcept
{
	if (_playActiveAni)
	{
		_aniDeltaTime	+= fElapsedTime;

		const float y	= CETween::Lerp( _originPos.y - 10
										, _originPos.y
										, std::min(1.0f, _aniDeltaTime * 1.1f)
										, CETween::EaseType::easeInOutBack);

		transform->SetWorldPosition(_originPos.x, y, _originPos.z);

		if (_aniDeltaTime * 1.5f >= 1)
		{
			_playActiveAni = false;

			if(_spriteHpBar != nullptr)
				_spriteHpBar->Visible();
			_visibleHP = true;
		}
	}
}

void SquirrelTurretHolder::GetHit(float damage, int8 attacker) noexcept
{
	if (_isDead)
		return;

	_hp = _hp - damage;

	if (_hp <= 0)
	{
		_hp = 0;
		_isDead = true;
		Die();
	}
	else
	{
		/*GameObject* obj = GameObject::Instantiate();
		obj->AddComponent(new Damaged(transform, damage, attacker));*/
	}
}

void SquirrelTurretHolder::Die(void) noexcept
{
	if (Actor::State::DIE == _state)
		return;

	_state = Actor::State::DIE;

	// Dead Explosion
	_fx->StartFX(transform->GetWorldMatrix().r[3]);

	_spriteHpBar->Invisible();
	
	// Destroy
	std::vector<Transform*> childs = transform->GetChilds();
	for (int i = 0; i < childs.size(); ++i)
	{
		GameObject::Destroy(childs[i]->GetGameObject());
	}

	_effectPlayer->Play(_audioAsset, 0.35f, false, [&]() {
	gameObject->SetDestroy();
	});


	if (!_isExplode)
	{
		_isExplode = true;
		if (!NarrationManager::This().SetCsvData("Asset\\Narrative\\Tree\\TurretExplosion.csv"))
		{
			CE_ASSERT("rldnr", "NarrationData가 없습니다.");
		}
		NarrationManager::This().SetNarrationVolume(2.f);
		NarrationManager::This().Play();
	}
}
