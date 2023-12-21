#include "pch.h"
#include "WoodenBarricade.h"
#include "GameObject.h"
#include "WorldHP.h"
#include "Camera.h"
#include "ModelRenderer.h"
#include "Material.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Animator.h"
#include "Transform.h"
#include "AudioSource.h"
#include "EffectPlayer.h"


WoodenBarricade::WoodenBarricade(void) noexcept
	:MonsterActor(100)
{
}

void WoodenBarricade::Start(void) noexcept
{
	_hpBar = new WorldHP(this, Camera::GetMainCamera(), Vector4(1, 0, 0, 1), _isPreInstance);
	gameObject->AddComponent(_hpBar);
	_hpBar->Invisible();

	_renderer = gameObject->GetComponent<ModelRenderer>();
	if (_renderer == nullptr)
	{
		XMMATRIX offset = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		_renderer = ModelRenderer::Create();
		_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Break_Wall_01_Attackable\\Break_Wall_01_Attackable.fbx"));
		_renderer->SetOffsetMatrix(offset);
		gameObject->AddComponent(_renderer);

		Rigidbody* rigid = Rigidbody::Create(true);
		gameObject->AddComponent(rigid);

		_collider0 = Collider::CreateBoxCollider(0.5f, 2, 2.6f, XMVectorSet(0, 2, 0, 0));
		gameObject->AddComponent(_collider0);

		_collider1 = Collider::CreateBoxCollider(0.6f, 2, 2.6f, XMVectorSet(0, 2, 0, 0));
		_collider1->SetIsTrigger(true);
		gameObject->AddComponent(_collider1);
	}


	gameObject->SetTag(GameObjectTag::MONSTER);

	_breakEffectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_breakEffectPlayer);

	_breakAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\enviroment_Sound\\Play_World_Playroom_Castle_Dungeon_Destructible_WoodenBarricade.ogg");

	_smash = false;
}

void WoodenBarricade::Update(float fElapsedTime) noexcept
{
	if (_visibleHp)
	{
		_visibleTime += fElapsedTime;
		if (_visibleTime >= 3.f)
		{
			_visibleHp = false;
			_hpBar->Invisible();
		}
	}

	__super::Update(fElapsedTime);


	if (true == _smash)
	{
		transform->Translate(0.f, -fElapsedTime * 2.f, 0.f, false);
		if (XMVectorGetY(transform->GetWorldPosition()) <= -10.f)
		{
			gameObject->SetActive(false);
		}
	}
}

void WoodenBarricade::GetHit(float damage, int8 attacker) noexcept
{
	__super::GetHit(damage, attacker);

	if (_isDead)
	{
		Die();
		return;
	}

	if (false == _visibleHp)
	{
		_visibleHp = true;
		_hpBar->Visible();
	}
	_visibleTime = 0;
}

void WoodenBarricade::Die(void) noexcept
{
	if (Actor::State::DIE == _state)
		return;

	_state = Actor::State::DIE;

	_hpBar->Invisible();
	_renderer->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DEPTHEFFECT, Vector4(0, 0, 0, 0));

	//ºÎ½¤Áö´Â ¾Ö´Ï¸ÞÀÌ¼Ç
	//gameObject->SetDestroy();


	XMMATRIX offset = XMMatrixScaling(0.008f, 0.008f, 0.008f) * XMMatrixRotationY(CE_MATH::ToRadian(180));
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Break_Wall_01_Attackable\\BreakWall\\Break_Wall_01_Attackable1.fbx"));
	_renderer->SetOffsetMatrix(offset);

	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play((uint16)0, false, std::bind(&WoodenBarricade::Idle, this), 0.7f, std::bind(&WoodenBarricade::Walk, this));
	_animator->SetPlaySpeed(1.f);

	_collider0->SetEnable(false);
	_collider1->SetEnable(false);

	_breakEffectPlayer->Play(_breakAsset, 0.5f);

}

void WoodenBarricade::Idle(void) noexcept
{
	// ÇÔ¼ö ¸¸µé±â ±ÍÂú¾Æ¼­ ÀÌ°Å¾¸
	gameObject->SetDestroy();
}

void WoodenBarricade::Walk(void) noexcept
{
	_smash = true;
}

void WoodenBarricade::Attack(void) noexcept
{
}
