#include "pch.h"
#include "VacuumFan.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Animator.h"


VacuumFan::VacuumFan(bool up) noexcept
	: _up(up)
{

}

VacuumFan::~VacuumFan(void) noexcept
{

}

void VacuumFan::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\VacuumFan\\Vacuum_Fan.fbx"));
	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
							* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(270)
																						, CE_MATH::ToRadian(90)
																						, CE_MATH::ToRadian(90)));

	renderer->SetOffsetMatrix(offsetmatrix);

	XMVECTOR rotation = CE_MATH::Deg2Rad * XMVectorSet(-180, 0, 0, 1);
	transform->Rotate(rotation);

	_on = true;
}

void VacuumFan::Update(float fElapsedTime) noexcept
{
	if (_on == false)
		return;

	if (false == _up)
		transform->Rotate(XMVectorSet(0, 1, 0, 0) * fElapsedTime * 10.f);
	else
		transform->Rotate(XMVectorSet(0, 1, 0, 0) * fElapsedTime * -10.f);
}

void VacuumFan::FixedUpdate(float fElapsedTime) noexcept
{
}

void VacuumFan::LateUpdate(float fElapsedTime) noexcept
{
}
