#include "pch.h"
#include "WallBridge.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "AudioSource.h"	
#include "EffectPlayer.h"

WallBridge::WallBridge(bool isShort) noexcept
	: _isShort(isShort)
	, _isUp(false)
	, _isDown(false)
	, _time(0)
	, _isPushButton(false)
{
}

void WallBridge::Start(void) noexcept
{
	/* Rigidbody */
	Rigidbody* rigidbody = Rigidbody::Create(false);
	rigidbody->SetIsKinematic(true);
	gameObject->AddComponent(rigidbody);

	/* Model / Collider*/
	ModelRenderer* renderer = ModelRenderer::Create();
	Collider* collider;

	if (false == _isShort)
	{
		renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Wall_Bridge\\Wall_Bridge_long.fbx"));
		collider = Collider::CreateBoxCollider(2.2f, 2, 3.5f, XMVectorSet(-1.8f, -2, 3.5f, 0));
	}
	else
	{
		renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Wall_Bridge\\Wall_Bridge_short.fbx"));
		collider = Collider::CreateBoxCollider(3.2f, 2, 2.2f, XMVectorSet(3, -2, 1.8f,0));
	}

	gameObject->AddComponent(renderer);
	gameObject->AddComponent(collider);

	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);

	_upAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\enviroment_Sound\\Play_World_Clockwork_Outside_Platform_DrawBridge_Raise.ogg");
	_downAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\enviroment_Sound\\Play_World_Clockwork_Outside_Platform_DrawBridge_Raise_Complete.ogg");
}

void WallBridge::Update(float fElapsedTime) noexcept
{
	if (_isEnd)
		return;

	if (_isUp)
	{
		_time += fElapsedTime;
		if (_time >= 1)
		{
			_time = 1;
			_isEnd = true;
			if (!_isPushButton)
			{
				_effectPlayer->Play(_upAsset, 0.5f);
			}
			_isPushButton = true;
		}

		float y = CETween::Lerp(_pos.y, 0.f, _time);
		transform->SetWorldPosition(_pos.x, y, _pos.z);
	}
	if (_isDown)
	{
		_time += fElapsedTime;
		if (_time >= 1)
		{
			_time = 1;
			_isEnd = true;
			if (_isPushButton)
			{
				_effectPlayer->Play(_downAsset, 0.5f);
			}
			_isPushButton = false;

		}

		float y = CETween::Lerp(_pos.y,-4.f, _time);
		transform->SetWorldPosition(_pos.x, y, _pos.z);
	}

}

void WallBridge::UpBridge(void) noexcept
{
	_isUp = true;
	_isDown = false;
	_isEnd = false;
	_time = 0;
	XMStoreFloat4(&_pos,transform->GetWorldPosition());
}

void WallBridge::DownBridge(void) noexcept
{
	_isUp = false;
	_isDown = true;
	_isEnd = false;
	_time = 0;
	XMStoreFloat4(&_pos, transform->GetWorldPosition());
}
