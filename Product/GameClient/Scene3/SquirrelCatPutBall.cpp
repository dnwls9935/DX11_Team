#include "pch.h"
#include "SquirrelCatPutBall.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "GameObject.h"
#include "Transform.h"

void SquirrelCatPutBall::Start(void) noexcept
{
	_rigidbody	= Rigidbody::Create(false);
	gameObject->AddComponent(_rigidbody);

	_collider	= Collider::CreateSphereCollider(6);
	gameObject->AddComponent(_collider);

	_collider->SetEnable(false);
	_rigidbody->SetEnable(false);
}

void SquirrelCatPutBall::FixedUpdate(float fElapsedTime) noexcept
{
}

void SquirrelCatPutBall::Update(float fElapsedTime) noexcept
{
}

void SquirrelCatPutBall::LateUpdate(float fElapsedTime) noexcept
{
}

void SquirrelCatPutBall::OnEnable(void) noexcept
{
}

void SquirrelCatPutBall::OnDisable(void) noexcept
{
}

void SquirrelCatPutBall::Fire(FXMVECTOR force) noexcept
{
	transform->SetParent(nullptr);
	_rigidbody->SetEnable(true);
	_collider->SetEnable(true);

	_rigidbody->AddForce(force);
}
