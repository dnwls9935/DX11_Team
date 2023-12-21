#include "pch.h"
#include "BigFan_Module.h"
#include "Transform.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "PxPhysicsAPI.h"
#include "PhysicsSystem.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

BigFan_Module::BigFan_Module(void) noexcept
	: _playSound(false)
{
}

BigFan_Module::~BigFan_Module(void) noexcept
{
}

void BigFan_Module::Start(void) noexcept
{
	XMMATRIX mat = XMMatrixScaling(0.01f, 0.01f, 0.01f)
			* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(0)
																		, CE_MATH::ToRadian(0)
																		, CE_MATH::ToRadian(0)));
	gameObject->GetComponent<ModelRenderer>()->SetOffsetMatrix(mat);
	
	Rigidbody* rigidbody = Rigidbody::Create(true);
	rigidbody->SetUseGravity(false);
	gameObject->AddComponent(rigidbody);


	Collider* collider = Collider::CreateBoxCollider(3.f, 5.f, 3.f, XMVectorSet(0.f, 1.4f,0.f,1.f));
	gameObject->AddComponent(collider);
	gameObject->SetTag(GameObjectTag::ROT_OBSTACLE2);


	XMVECTOR position = gameObject->GetTransform()->GetWorldMatrix().r[3];
	_myX = XMVectorGetX(position);

	if (-221.9f >= _myX)
	{
		transform->SetLocalEulerAngle(0, 0, 0);
		_reverse = true;
	}
	else
	{
		transform->SetLocalEulerAngle(0, CE_MATH::Deg2Rad * 180.f, 0);
		_reverse = false;
	}

	_rotate = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\WallSlideFan\\Play_World_Shed_Vacuum_Platform_WallSlideFan_WallRotate.ogg");

	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);
}

void BigFan_Module::FixedUpdate(float fElapsedTime) noexcept
{
}

void BigFan_Module::Update(float fElapsedTime) noexcept
{

	if (true == _lerp && false == _playSound)
	{

		_effectPlayer->Play(_rotate,0.05f,false,nullptr,false,true);
		_playSound = true;
	}

	float t = 0;
	if (false == _reverse)
	{
		if (-221.9f <= _myX)
		{
			if (true == _lerp)
			{
				_lerpTime -= fElapsedTime * 1.5f;

				t = CETween::Lerp(180.f, 0, std::max(0.f, std::min(1.f, _lerpTime)), CETween::EaseType::easeInOutQuart);
				transform->SetLocalEulerAngle(0, CE_MATH::Deg2Rad * t, 0);

				if (_lerpTime <= -0.5f)
				{
					_lerp = false;
				}
			}
		}
		else
		{
			_reverse = !_reverse;
			_lerp = true;
			_playSound = false;
		}
	}
	else
	{
		if (-221.9f >= _myX)
		{
			if (true == _lerp)
			{
				_lerpTime += fElapsedTime * 1.5f;

				t = CETween::Lerp(180.f, 0, std::max(0.f, std::min(1.f, _lerpTime)), CETween::EaseType::easeInOutQuart);
				transform->SetLocalEulerAngle(0, CE_MATH::Deg2Rad * t, 0);

				if (_lerpTime >= 1.5f)
				{
					_lerp = false;
				}
			}
		}
		else
		{
			_reverse = !_reverse;
			_lerp = true;
			_playSound = false;
		}
	}

	
	transform->GetParent()->SetLocalEulerAngle(0, 0, -1 * XMVectorGetZ(transform->GetParent()->GetParent()->GetLocalEulerAngle()));
}

void BigFan_Module::LateUpdate(float fElapsedTime) noexcept
{
	XMVECTOR position = gameObject->GetTransform()->GetWorldMatrix().r[3];
	_myX = XMVectorGetX(position);

}
