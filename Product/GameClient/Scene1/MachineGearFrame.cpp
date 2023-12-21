#include "pch.h"
#include "MachineGearFrame.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"

#include "PressBtn.h"

MachineGearFrame::MachineGearFrame(POSITION posType) noexcept
	: _posType(posType)
{

}

MachineGearFrame::~MachineGearFrame(void) noexcept
{

}

void MachineGearFrame::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Shed2\\Machine\\MachineCogs_Large.FBX"));
	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
							* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(270)
																						, CE_MATH::ToRadian(90)
																						, CE_MATH::ToRadian(90)));
	renderer->SetOffsetMatrix(offsetmatrix);

	XMVECTOR angle = XMVectorSet(0, 0, 0, 0);
	if (POSITION::UP == _posType)
	{
		angle = CE_MATH::Deg2Rad * XMVectorSet(-3.06f, 90.23f, 0.078f, 1);
		transform->Rotate(angle);
		transform->SetLocalScale(0.8f, 0.8f, 0.8f);
	}
	else if (POSITION::DOWN == _posType)
	{
		angle = CE_MATH::Deg2Rad * XMVectorSet(0, 90.f, 0, 1);
		transform->Rotate(angle);
		transform->SetLocalScale(1, 1, 1);
	}

	_rotate = false;
}

void MachineGearFrame::FixedUpdate(float fElapsedTime) noexcept
{
	if (true == _btnA->GetComponent<PressBtn>()->GetPress() &&
		true == _btnB->GetComponent<PressBtn>()->GetPress())
		_rotate = true;
}

void MachineGearFrame::Update(float fElapsedTime) noexcept
{
	if (true == _rotate)
	{
		if (POSITION::UP == _posType)
			transform->Rotate(XMVectorSet(1,0,0,0) * fElapsedTime);
		else if (POSITION::DOWN == _posType)
			transform->Rotate(XMVectorSet(-1, 0, 0, 0) * fElapsedTime);
	}
}

void MachineGearFrame::LateUpdate(float fElapsedTime) noexcept
{
}
