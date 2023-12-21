#include "pch.h"
#include "Dumbbell.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"

#include "May.h"
#include "Cody.h"

Dumbbell::Dumbbell(void) noexcept
{

}

Dumbbell::~Dumbbell(void) noexcept
{

}

void Dumbbell::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Dumbbell\\Dumbbell.fbx"));
	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(CE_MATH::Deg2Rad * 90.f);
							/** XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(270)
																						, CE_MATH::ToRadian(90)
																						, CE_MATH::ToRadian(90)))*/;
	renderer->SetOffsetMatrix(offsetmatrix);

	gameObject->SetTag(GameObjectTag::GROUND);

	XMVECTOR position = XMVectorSet(-102.88f, -19.32f, -142.54f, 1.f);
	transform->SetWorldPosition(position);
	XMVECTOR rotation = CE_MATH::Deg2Rad * XMVectorSet(0, 90.f, 0, 0);
	transform->SetLocalEulerAngle(rotation);

	_rigid = Rigidbody::Create(false);
	_rigid->SetIsKinematic(true);
	_rigid->SetFreezeRotationX(true);
	_rigid->SetFreezeRotationY(true);
	_rigid->SetFreezeRotationZ(true);
	_rigid->SetDrag(0);
	_rigid->SetAngularDrag(0);
	_rigid->SetUseGravity(false);

	gameObject->AddComponent(_rigid);

	_collider = Collider::CreateBoxCollider(10, 2, 2, XMVectorSet(0, 0, 2, 1));
	gameObject->AddComponent(_collider);

	_may = GameObject::FindLoadingObjectByName("May");
	_cody = GameObject::FindLoadingObjectByName("Cody");

	{
		_dumbbellMayGrab = GameObject::Instantiate("DumbbellMayGrab");
		_dumbbellMayGrab->GetTransform()->SetWorldPosition(-103.718f, -21.127f, -138.43f);
		_dumbbellMayGrab->SetTag(GameObjectTag::OBJECT);

		Rigidbody* rigid = Rigidbody::Create(false);
		rigid->SetIsKinematic(true);
		rigid->SetFreezeRotationX(true);
		rigid->SetFreezeRotationY(true);
		rigid->SetFreezeRotationZ(true);
		rigid->SetDrag(0);
		rigid->SetAngularDrag(0);
		rigid->SetUseGravity(false);
		_dumbbellMayGrab->AddComponent(rigid);

		Collider* collider = Collider::CreateSphereCollider(3.f);
		collider->SetIsTrigger(true);
		_dumbbellMayGrab->AddComponent(collider);


		_dumbbellCodyGrab = GameObject::Instantiate("DumbbellCodyGrab");
		_dumbbellCodyGrab->GetTransform()->SetWorldPosition(-103.718f, -21.127f, -146.76f);
		_dumbbellCodyGrab->SetTag(GameObjectTag::OBJECT);

		rigid = Rigidbody::Create(false);
		rigid->SetIsKinematic(true);
		rigid->SetFreezeRotationX(true);
		rigid->SetFreezeRotationY(true);
		rigid->SetFreezeRotationZ(true);
		rigid->SetDrag(0);
		rigid->SetAngularDrag(0);
		rigid->SetUseGravity(false);
		_dumbbellCodyGrab->AddComponent(rigid);

		collider = Collider::CreateSphereCollider(3.f);
		collider->SetIsTrigger(true);
		_dumbbellCodyGrab->AddComponent(collider);
	}
}

void Dumbbell::Update(float fElapsedTime) noexcept
{
	if (nullptr != _may && nullptr != _cody)
	{
		Actor::State mayState = _may->GetComponent<May>()->GetState();
		Actor::State codyState = _cody->GetComponent<Cody>()->GetState();

		if (
			(Actor::State::PUSH == mayState &&
			Actor::State::PUSH == codyState)
			)
		{			
			float x = XMVectorGetX(transform->GetWorldPosition());
			if (-88.2 >= x)
			{
				transform->Translate(XMVectorSet(1, 0, 0, 0) * fElapsedTime * 10.f);
				_may->GetTransform()->Translate(XMVector3Normalize(_may->GetTransform()->GetForward()) * fElapsedTime * 10.f);
				_cody->GetTransform()->Translate(XMVector3Normalize(_may->GetTransform()->GetForward()) * fElapsedTime * 10.f);

				_dumbbellMayGrab->GetTransform()->Translate(XMVectorSet(1, 0, 0, 0) * fElapsedTime * 10.f);
				_dumbbellCodyGrab->GetTransform()->Translate(XMVectorSet(1, 0, 0, 0) * fElapsedTime * 10.f);
			}
		}
	}
}

void Dumbbell::FixedUpdate(float fElapsedTime) noexcept
{
}

void Dumbbell::LateUpdate(float fElapsedTime) noexcept
{
}
