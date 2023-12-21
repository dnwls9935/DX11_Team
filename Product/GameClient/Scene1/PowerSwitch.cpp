#include "pch.h"
#include "SpriteRenderer.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "FuseCrab.h"
#include "TargetPoint.h"
#include "PowerSwitch.h"

#include "AudioSource.h"
#include "NarrationManager.h"
#include "EffectPlayer.h"

PowerSwitch::PowerSwitch(void) noexcept
	: _oneGrab(false)
{
}

PowerSwitch::~PowerSwitch(void) noexcept
{
}

void PowerSwitch::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Utilty\\PowerSwitch.fbx"));
	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
								* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(270)
																							, CE_MATH::ToRadian(90)
																							, CE_MATH::ToRadian(90)));
	renderer->SetOffsetMatrix(offsetmatrix);

	_defaultPosition =XMVectorSet(-32.69f, -1.61f, 35.55f, 1);
	transform->SetWorldPosition(_defaultPosition);


	_defaultRotation = CE_MATH::Deg2Rad * XMVectorSet(-43.775f, 0, 90.f, 1);
	transform->SetWorldEulerAngle(_defaultRotation);

	_rigid = Rigidbody::Create(true);
	gameObject->AddComponent(_rigid);

	_collider = Collider::CreateSphereCollider(7.f);
	_collider->SetIsTrigger(true);
	gameObject->AddComponent(_collider);

	{
		_mayGrab = GameObject::Instantiate("PowerSwitchMayGrab");
		_mayGrab->GetTransform()->SetParent(gameObject->GetTransform());
		_mayGrab->GetTransform()->SetLocalPosition(XMVectorSet(2.735f, 1.53f, 0.064f, 1.f));
		_mayGrab->GetTransform()->SetWorldEulerAngle(_defaultRotation);

		GameObject* targetpoint = GameObject::Instantiate();

		TargetPoint* point = new TargetPoint(XMVectorSet(-34.25f,0.5f,34.f,1.f),TargetPoint::targetType::MAYPOINT);
		targetpoint->AddComponent(point);


		_codyGrab = GameObject::Instantiate("PowerSwitchCodyGrab");
		_codyGrab->GetTransform()->SetParent(gameObject->GetTransform());
		_codyGrab->GetTransform()->SetLocalPosition(XMVectorSet(2.735f, -1.53f, 0.064f, 1.f));
		_codyGrab->GetTransform()->SetWorldEulerAngle(_defaultRotation);

		targetpoint = GameObject::Instantiate();
		point = new TargetPoint(XMVectorSet(-31.f, 0.5f, 34.f, 1.f), TargetPoint::targetType::CODYPOINT);
		targetpoint->AddComponent(point);

		

	}

	/* 레버 상태 */
	_lower = false;
	_lowerAngle = 100.f;
	_check[0] = false;
	_check[1] = false;


	{
		GameObject* obj = GameObject::Instantiate("FusePutIn0");
		//obj->GetTransform()->SetParent(gameObject->GetTransform());
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-27.467f, -3.128879f, 35.7324f, 1.f));
		obj->SetTag(GameObjectTag::OBJECT);
		Rigidbody* rigid = Rigidbody::Create(true);
		//rigid->SetIsKinematic(true);
		rigid->SetUseGravity(true);
		obj->AddComponent(rigid);
		Collider* collider = Collider::CreateBoxCollider(0.5f, 0.5f, 0.5f);
		collider->SetIsTrigger(true);
		obj->AddComponent(collider);

		obj = GameObject::Instantiate("FusePutIn1");
		//obj->GetTransform()->SetParent(gameObject->GetTransform());
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-25.817f, -3.128879f, 35.7324f, 1.f));
		obj->SetTag(GameObjectTag::OBJECT);
		rigid = Rigidbody::Create(true);
		//rigid->SetIsKinematic(true);
		rigid->SetUseGravity(true);
		obj->AddComponent(rigid);
		collider = Collider::CreateBoxCollider(0.5f, 0.5f, 0.5f);
		collider->SetIsTrigger(true);
		obj->AddComponent(collider);

		obj = GameObject::Instantiate("FusePutIn2");
		//obj->GetTransform()->SetParent(gameObject->GetTransform());
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-24.247f, -3.128879f, 35.7324f, 1.f));
		obj->SetTag(GameObjectTag::OBJECT);
		rigid = Rigidbody::Create(true);
		//rigid->SetIsKinematic(true);
		rigid->SetUseGravity(true);
		obj->AddComponent(rigid);
		collider = Collider::CreateBoxCollider(0.5f, 0.5f, 0.5f);
		collider->SetIsTrigger(true);
		obj->AddComponent(collider);
	}

	_oneinteract = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\PowerSwitch\\Play_World_Shared_Interactable_PowerSwitch_OnePlayerOnSwitch.ogg");
	_doubleinteract = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\PowerSwitch\\Play_World_Shared_Interactable_PowerSwitch_DoubleInteract.ogg");
	_activated = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\PowerSwitch\\Play_World_Shared_Interactable_PowerSwitch_Activated.ogg");

	_audioSource = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_audioSource);



}

void PowerSwitch::FixedUpdate(float fElapsedTime) noexcept
{

	if (_check[0] && _check[1] && _lower == false)
	{
		_lower = true;
		_collider->SetEnable(false);
		_rigid->SetEnable(false);

		_audioSource->Play(_doubleinteract, 0.1f);

	
	}

	if ((_check[0] || _check[1]) && !_oneGrab)
	{
		_oneGrab = true;

		_audioSource->Play(_oneinteract , 0.1f);
	}

}

void PowerSwitch::Update(float fElapsedTime) noexcept
{
	if (true == _lower)
	{
		transform->Rotate(CE_MATH::Deg2Rad * fElapsedTime * -100.f ,0,0);
	}
}

void PowerSwitch::LateUpdate(float fElapsedTime) noexcept
{
	float rotationX = CE_MATH::Rad2Deg * XMVectorGetX(transform->GetWorldEulerAngle());
	float defaultX = CE_MATH::Rad2Deg * XMVectorGetX(_defaultRotation);

	_currentAngle = fabs(defaultX - rotationX);

	if (true == _lower &&
		_currentAngle >= _lowerAngle)
	{
		_lower = false;
		GameObject::FindObjectByName("FuseCrab0")->GetComponent<FuseCrab>()->StartAnimation();
		GameObject::FindObjectByName("FuseCrab1")->GetComponent<FuseCrab>()->StartAnimation();
		GameObject::FindObjectByName("FuseCrab2")->GetComponent<FuseCrab>()->StartAnimation();

		_check[0] = false;
		_check[1] = false;

		_audioSource->Play(_activated , 0.01f);


		GameObject* obj = GameObject::FindObjectByName("mayTargetUI");
		if (nullptr != obj)
			obj->SetActive(false);
		obj = GameObject::FindObjectByName("mayTargetOutLineUI");
		if (nullptr != obj)
			obj->SetActive(false);
		obj = GameObject::FindObjectByName("codyTargetUI");
		if (nullptr != obj)
			obj->SetActive(false);
		obj = GameObject::FindObjectByName("codyTargetOutLineUI");
		if (nullptr != obj)
			obj->SetActive(false);
	}
}

void PowerSwitch::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
}

void PowerSwitch::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
}

XMMATRIX PowerSwitch::GetCodyGrabMatrix()
{
	return _codyGrab->GetTransform()->GetWorldMatrix();
}

XMMATRIX PowerSwitch::GetMayGrabMatrix()
{
	return _mayGrab->GetTransform()->GetWorldMatrix();
}
