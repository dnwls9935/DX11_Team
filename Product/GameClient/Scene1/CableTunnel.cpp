#include "pch.h"
#include "CableTunnel.h"

#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "ModelRenderer.h"

CableTunnel::CableTunnel(void) noexcept
{
}

CableTunnel::~CableTunnel(void) noexcept
{
}

void CableTunnel::Start(void) noexcept
{
	XMVECTOR rotation = CE_MATH::Deg2Rad * XMVectorSet(90.f, 180.f,0,1);
	transform->SetLocalEulerAngle(rotation);
	transform->SetLocalScale(2, 2, 2);

	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Utilty\\cabletunnel.fbx"));

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
							* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(-90)
																						, 0
																						, 0));
	renderer->SetOffsetMatrix(offsetmatrix);
	gameObject->AddComponent(renderer);

	_rigid = Rigidbody::Create(true);
	_rigid->SetFreezePositionX(true);
	_rigid->SetFreezePositionY(true);
	_rigid->SetFreezePositionZ(true);

	_rigid->SetFreezeRotationX(true);
	_rigid->SetFreezeRotationY(true);
	_rigid->SetFreezeRotationZ(true);

	gameObject->AddComponent(_rigid);

	_collider = Collider::CreateMeshCollider(2, renderer->GetModel());
	gameObject->AddComponent(_collider);
}

void CableTunnel::Update(float fElapsedTime) noexcept
{
}
