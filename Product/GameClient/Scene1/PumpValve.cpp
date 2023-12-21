#include "pch.h"
#include "PumpValve.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"

#include "BigFan.h"
#include "PressBtn.h"

#include "BigFan_Vertical.h"

#include "MeshRenderer.h"

PumpValve::PumpValve(void) noexcept
{

}

PumpValve::~PumpValve(void) noexcept
{

}

void PumpValve::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\PumpValve\\PumpValve.fbx"));
	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	renderer->SetOffsetMatrix(offsetmatrix);

	_rotate = false;
	_direction = false;

	{
		_grabPosition = GameObject::Instantiate("PumpValveGrabPosition");
		_grabPosition->GetTransform()->SetWorldPosition(-279.039f, -20.874f, -139.8f);
		_grabPosition->SetTag(GameObjectTag::OBJECT);

		Rigidbody* rigid = Rigidbody::Create(true);
		_grabPosition->AddComponent(rigid);

		Collider* collider = Collider::CreateSphereCollider(3.f);
		collider->SetIsTrigger(true);
		_grabPosition->AddComponent(collider);

	}
}

void PumpValve::FixedUpdate(float fElapsedTime) noexcept
{
}

void PumpValve::Update(float fElapsedTime) noexcept
{
	if (true == _rotate)
	{
		if (true == _direction)
			transform->Rotate(XMVectorSet(1, 0, 0, 0) * -fElapsedTime * 4.f);
		else
			transform->Rotate(XMVectorSet(1, 0, 0, 0) * fElapsedTime * 4.f);
	}
}

void PumpValve::LateUpdate(float fElapsedTime) noexcept
{
	if (nullptr != _btn)
	{
		_rotate = _btn->GetComponent<PressBtn>()->GetPress();
		if (nullptr != _bigFan[0])
			_bigFan[0]->GetComponent<BigFan>()->SetRotate(_rotate);
		if (nullptr != _bigFan[1])
			_bigFan[1]->GetComponent<BigFan>()->SetRotate(_rotate);
	}
}

void PumpValve::SetDirection(bool b)
{
	_direction = b;
}

void PumpValve::SetRotate(bool b)
{
	_rotate = b;
	if (nullptr != _bigFan[0])
		_bigFan[0]->GetComponent<BigFan_Vertical>()->SetRotate(_rotate);
	if (nullptr != _bigFan[1])
		_bigFan[1]->GetComponent<BigFan_Vertical>()->SetRotate(_rotate);
}

void PumpValve::SetController(GameObject* obj1, GameObject* obj2)
{
	if(nullptr != obj1)
		_bigFan[0] = obj1;
	if(nullptr != obj2)
		_bigFan[1] = obj2;
};

void PumpValve::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
}

void PumpValve::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
}

void PumpValve::SetBtn(GameObject* btn)
{
	if (nullptr != btn)
		_btn = btn;
}
