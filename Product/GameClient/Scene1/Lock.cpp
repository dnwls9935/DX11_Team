#include "pch.h"
#include "Lock.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "CrusherDrawBridge.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

Lock::Lock(CrusherDrawBridge* bridge) noexcept
	: _bridge (bridge)
	, _time(0)
{
	_bridge->AddLock(this);
}

void Lock::Start(void) noexcept
{
	GameObject* lock = GameObject::Instantiate();
	_lockTrans = lock->GetTransform();
	_lockTrans->SetParent(transform);
	_lockTrans->SetLocalPosition(0, 0, 0);
	_lockTrans->SetLocalEulerAngle(0, 0, CE_MATH::ToRadian(-15));

	XMMATRIX offset = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Crusher_Drawbridge_01_Lock\\Lock.fbx"));
	renderer->SetOffsetMatrix(offset);
	lock->AddComponent(renderer);

	_OpenEffectPlayer = new EffectPlayer(AudioSource::Create());
	lock->AddComponent(_OpenEffectPlayer);
	_OpenAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\enviroment_Sound\\Play_World_Playroom_Castle_Dungeon_Interactable_CrusherBridgeSupport_LockOpen.ogg");

}

void Lock::Update(float fElapsedTime) noexcept
{
	if (_isEnd)
		return;

	if (_isUnlock)
	{
		_time += fElapsedTime;
		if (_time >= 0.5f)
		{
			_time = 0.5f;
			_isEnd = true;
			_isUnlock = false;
			_bridge->AddUnlock();
		}
		float angle = CETween::Lerp(-15, -180.f, _time *2, CETween::EaseType::easeInQuad);
		_lockTrans->SetLocalEulerAngle(0, 0, CE_MATH::ToRadian(angle));
	}
	else if (_isFollow)
	{
		_time += fElapsedTime;
		if (_time >= 1)
		{
			_time = 1;
			_isEnd = true;
			_isFollow = false;
		}
		float angle = CETween::Lerp(-180, -225, _time, CETween::EaseType::easeInQuad);
		_lockTrans->SetLocalEulerAngle(0, 0, CE_MATH::ToRadian(angle));
	}
}

void Lock::Unlock(void) noexcept
{
	_isUnlock = true;
	_time = 0;

	_OpenEffectPlayer->Play(_OpenAsset, 0.5f);
}

void Lock::FollowBridge(void) noexcept
{
	_time = 0;
	_isFollow = true;
	_isEnd = false;
}
