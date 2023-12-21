#include "pch.h"
#include "PressBtn.h"
#include "Actor.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "MachineGearFrame.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

PressBtn::PressBtn(void) noexcept
	: _collider(nullptr)
	, _rigidbody(nullptr)
{

}

PressBtn::~PressBtn(void) noexcept
{

}

void PressBtn::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	//renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Shed2\\Machine\\Machine_Power_Button.fbx"));
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Shed2\\Machine\\Machine_Power_Button.FBX"));
	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
							* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(270)
																						, CE_MATH::ToRadian(90)
																						, CE_MATH::ToRadian(90)));
	renderer->SetOffsetMatrix(offsetmatrix);

	transform->SetLocalScale(2, 2, 2);
	XMVECTOR angle = CE_MATH::Deg2Rad* XMVectorSet(0, 0, -90.f, 1);
	transform->SetLocalEulerAngle(angle);

	_rigidbody = Rigidbody::Create(false);
	_rigidbody->SetIsKinematic(true);

	_rigidbody->SetFreezePositionY(true);
	_rigidbody->SetFreezePositionZ(true);

	_rigidbody->SetFreezeRotationX(true);
	_rigidbody->SetFreezeRotationY(true);
	_rigidbody->SetFreezeRotationZ(true);

	_rigidbody->SetUseGravity(false);
	gameObject->AddComponent(_rigidbody);

	_collider = Collider::CreateBoxCollider(0.2f ,1.5f,1.5f, XMVectorSet(-1.7f, 0, 0, 1));
	gameObject->AddComponent(_collider);

	/* 눌린 상태 */
	_pressed = false;
	_up = false;
	_deltaTime = 0.5f;

	_press = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\Button\\Play_Inter_Shared_GroundPoundButton_Press.ogg");
	_reset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\Button\\Play_Inter_Shared_GroundPoundButton_Reset.ogg");

	_audioSource = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_audioSource);
}

void PressBtn::FixedUpdate(float fElapsedTime) noexcept
{
}

void PressBtn::Update(float fElapsedTime) noexcept
{
	if (true == _up)
	{
		float x = XMVectorGetX(transform->GetWorldPosition());
		float y = XMVectorGetY(transform->GetWorldPosition());
		float z = XMVectorGetZ(transform->GetWorldPosition());

		y = CETween::Lerp(y, _originY, _deltaTime, CETween::EaseType::easeInQuad);

		transform->SetWorldPosition(XMVectorSet(x, y, z, 1));
		_rigidbody->SetFreezeRotationX(true);

		if (y >= _originY - 0.01f)
		{
			transform->SetWorldPosition(XMVectorSet(x, _originY, z, 1));
			_up = false;
			_pressed = false;
		}

		return;
	}

	if (true == _pressed)
	{
		float x = XMVectorGetX(transform->GetWorldPosition());
		float y = XMVectorGetY(transform->GetWorldPosition());
		float z = XMVectorGetZ(transform->GetWorldPosition());

		y = CETween::Lerp(y, _y, _deltaTime, CETween::EaseType::easeInQuad);

		transform->SetWorldPosition(XMVectorSet(x, y, z, 1));

		_rigidbody->SetFreezeRotationX(true);
	}
}

void PressBtn::LateUpdate(float fElapsedTime) noexcept
{
}

void PressBtn::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	GameObject* obj = other->GetGameObject();

	if (GameObjectTag::PLAYER == obj->GetTag())
	{
		Actor* actor = obj->GetComponent<Actor>();

		if (actor->GetPlayerState() != Actor::State::GROUNDPOUNDFALLING)
			return;

		_rigidbody->SetFreezePositionX(false);
		_pressed = true;
		_up = false;
		_originY = XMVectorGetY(transform->GetWorldPosition());
		_y = _originY - 0.6f;
		_audioSource->Play(_press,0.05f);
	}
}

void PressBtn::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
	if (false == _pressed)
		return;


	GameObject* obj = other->GetGameObject();

	if (GameObjectTag::PLAYER == obj->GetTag())
	{
		Actor* actor = obj->GetComponent<Actor>();

		if (actor->GetPlayerState() == Actor::State::IDLE ||
			actor->GetPlayerState() == Actor::State::WALK ||
			actor->GetPlayerState() == Actor::State::WALKEND ||
			actor->GetPlayerState() == Actor::State::WALKSTART || 
			actor->GetPlayerState() == Actor::State::JUMP ||
			actor->GetPlayerState() == Actor::State::JUMPFWD || 
			actor->GetPlayerState() == Actor::State::JUMPLAND)
		{
			_up = true;
			_audioSource->Play(_reset,0.05f);
		}
	}
}
