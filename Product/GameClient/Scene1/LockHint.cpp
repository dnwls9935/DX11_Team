#include "pch.h"
#include "LockHint.h"
#include "GameObject.h"
#include "WorldHP.h"
#include "Camera.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Material.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Lock.h"
#include "ToyCrusher.h"
#include "Animator.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

LockHint::LockHint(Lock* lock) noexcept
	: MonsterActor(25)
	, _lock(lock)
	,_ChainDropEffectPlayer(nullptr)
	,_ChainDropAsset(nullptr)
	,_DestroyEffectPlayer(nullptr)
	,_DestroyAsset(nullptr)
{
}

void LockHint::Start(void) noexcept
{
	_hpBar = new WorldHP(this, Camera::GetMainCamera(), Vector4(1, 0, 0, 1), _isPreInstance);
	gameObject->AddComponent(_hpBar);
	_hpBar->Invisible();

	XMMATRIX offset = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	_renderer = ModelRenderer::Create();
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Crusher_Drawbridge_01_LockHintpoint_Breakable\\Crusher_Drawbridge_01_LockHintpoint_Breakable.fbx"));
	_renderer->SetOffsetMatrix(offset);
	gameObject->AddComponent(_renderer);

	_rigid = Rigidbody::Create(false);
	_rigid->SetIsKinematic(true);
	gameObject->AddComponent(_rigid);

	Collider* collider = Collider::CreateBoxCollider(0.6f, 1.7f, 0.6f, XMVectorSet(0, 1.18f, 0, 0));
	gameObject->AddComponent(collider);

	_child = GameObject::Instantiate("LockHint ");
	_child->GetTransform()->SetParent(transform);
	_child->GetTransform()->SetLocalPosition(0, 0, 0);

	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Crusher_Drawbridge_01_LockHintpoint\\Crusher_Drawbridge_01_LockHintpoint.fbx"));
	renderer->SetOffsetMatrix(offset);
	_child->AddComponent(renderer);

	Rigidbody* rigid = Rigidbody::Create(false);
	rigid->SetFreezeRotationX(true);
	rigid->SetFreezeRotationY(true);
	rigid->SetFreezeRotationZ(true);
	rigid->SetFreezePositionX(true);
	rigid->SetFreezePositionZ(true);
	rigid->SetUseGravity(true);
	_child->AddComponent(rigid);

	collider = Collider::CreateBoxCollider(0.4f, 0.22f, 0.4f, XMVectorSet(0, 3.41f, 0, 0));
	_child->AddComponent(collider);

	gameObject->SetTag(GameObjectTag::MONSTER);

	_ChainDropEffectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_ChainDropEffectPlayer);
	_DestroyEffectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_DestroyEffectPlayer);

	_ChainDropAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\enviroment_Sound\\Play_World_Playroom_Castle_Dungeon_Interactable_CrusherBridgeSupport_ChainDrop.ogg");
	_DestroyAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\enviroment_Sound\\Play_World_Playroom_Castle_Dungeon_Interactable_CrusherBridgeSupport_Destroy.ogg");
}

void LockHint::Update(float fElapsedTime) noexcept
{
	if (_isDeath)
	{
		_deathTime += fElapsedTime;
		if (false == _endRigid)
		{
			if (_deathTime >= 0.5f)
			{
			_rigid->SetUseGravity(true);
				_endRigid = true;
			}
		}
	
		if (_deathTime >= 1.f)
		{
			gameObject->SetDestroy();
		}

	}

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
}

void LockHint::GetHit(float damage, int8 attacker) noexcept
{
	__super::GetHit(damage, attacker);

	if (_isDead)
	{
		Die();
		GameObject::FindObjectByName("ToyCrusher")->GetComponent<ToyCrusher>()->SetLockHint();
		return;
	}

	if (false == _visibleHp)
	{
		_visibleHp = true;
		_hpBar->Visible();
	}
	_visibleTime = 0;
}

void LockHint::Die(void) noexcept
{
	if (Actor::State::DIE == _state)
		return;

	_state = Actor::State::DIE;

	_hpBar->Invisible();

	_lock->Unlock();

	//부숴지는 애니메이션
	_child->GetTransform()->SetParent(nullptr);
	_child = nullptr;
	
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Crusher_Drawbridge_01_LockHintpoint_Breakable\\LockHintpoint_Break.fbx"));
	_renderer->SetOffsetMatrix(XMMatrixScaling(0.01f, 0.01f, 0.01f));

	Animator* animator = Animator::Create();
	gameObject->AddComponent(animator);

	animator->Play((uint16)0, false, std::bind(&LockHint::OnEndAnimation, this));
	_rigid->SetUseGravity(false);
	_rigid->SetIsKinematic(false);
	gameObject->GetComponent<Collider>()->SetIsTrigger(true);
}

void LockHint::OnEndAnimation(void) noexcept
{
	_isDeath = true;
	_deathTime = 0;


	

	_ChainDropEffectPlayer->Play(_ChainDropAsset, 0.5f);
	_DestroyEffectPlayer->Play(_DestroyAsset, 0.5f);

	gameObject->SetDestroy();

}

void LockHint::Idle(void) noexcept
{
}

void LockHint::Walk(void) noexcept
{
}

void LockHint::Attack(void) noexcept
{
}
