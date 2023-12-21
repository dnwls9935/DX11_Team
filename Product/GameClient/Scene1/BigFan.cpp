#include "pch.h"
#include "BigFan.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"

BigFan::BigFan(bool direction) noexcept
	: _direction(direction)
{
}

BigFan::~BigFan(void) noexcept
{

}

void BigFan::Start(void) noexcept
{
	_rotate = true;

	transform->SetWorldEulerAngle(CE_MATH::ToRadian(-90), 0, 0);
	
	auto rigidbody = gameObject->GetComponent<Rigidbody>();

	if (rigidbody == nullptr)
		return;

	rigidbody->SetFreezePositionY(true);
	rigidbody->SetFreezeRotationX(true);
	rigidbody->SetFreezePositionY(true);
}

void BigFan::Update(float fElapsedTime) noexcept
{

}

void BigFan::FixedUpdate(float fElapsedTime) noexcept
{
	if (true == _rotate)
	{
		if (true == _direction)
			transform->Rotate(XMVectorSet(0, 0, 30.1f, 0) * fElapsedTime * CE_MATH::Deg2Rad, true);
		else
			transform->Rotate(XMVectorSet(0, 0, 30.f, 0) * -fElapsedTime * CE_MATH::Deg2Rad, true);
	}
}

void BigFan::LateUpdate(float fElapsedTime) noexcept
{
}
