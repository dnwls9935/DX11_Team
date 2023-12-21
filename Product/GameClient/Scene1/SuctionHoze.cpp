#include "pch.h"
#include "SuctionHoze.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Animator.h"

#include "FuseLight.h"

SuctionHoze::SuctionHoze(void) noexcept
{

}

SuctionHoze::~SuctionHoze(void) noexcept
{

}

void SuctionHoze::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create(false);
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Character\\SucktionHoze\\SuctionHozeAnimate.fbx"));
	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
							* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(270)
																						, CE_MATH::ToRadian(90)
																						, CE_MATH::ToRadian(90)));

	

	if ("SuctionHoze_Swamp" == gameObject->GetName())
	{
		_suction = true;

		_animator = Animator::Create();
		gameObject->AddComponent(_animator);
		_animator->Play("VacuumHoseHead.ao|VacuumHoseHead_Suck_MH"
			, true);
		_animator->SetPlaySpeed(1.f); 
		renderer->SetOffsetMatrix(offsetmatrix);
	}
	else if ("SuctionHoze_Swamp_Up" == gameObject->GetName())
	{
		offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
		_suction = true;

		_animator = Animator::Create();
		gameObject->AddComponent(_animator);
		_animator->Play("VacuumHoseHead.ao|VacuumHoseHead_Suck_MH"
			, true);
		_animator->SetPlaySpeed(1.f);
		renderer->SetOffsetMatrix(offsetmatrix);
	}
	else
	{
		_suction = false;

		_animator = Animator::Create();
		gameObject->AddComponent(_animator);
		_animator->Play("VacuumHoseHead.ao|VacuumHoseHead_Stunned_MH"
			, true);
		_animator->SetPlaySpeed(1.f); 
		renderer->SetOffsetMatrix(offsetmatrix);

	}
}

void SuctionHoze::FixedUpdate(float fElapsedTime) noexcept
{
}

void SuctionHoze::Update(float fElapsedTime) noexcept
{
	if ("SuctionHoze_PowerSwitch" == gameObject->GetName())
	{
		if (true == _fuseLights[0]->GetComponent<FuseLight>()->GetLight() &&
			true == _fuseLights[1]->GetComponent<FuseLight>()->GetLight() &&
			true == _fuseLights[2]->GetComponent<FuseLight>()->GetLight() &&
			(uint16)ANIMATION::VacuumHoseHead_Stunned_MH == _animator->GetCurrentAnimationIndex())
		{
			SetSuction(true);
		}
	}

}

void SuctionHoze::LateUpdate(float fElapsedTime) noexcept
{
}

void SuctionHoze::SetSuction(bool b)
{
	_suction = b;
	_animator->Play("VacuumHoseHead.ao|VacuumHoseHead_Suck_MH"
		, true);
}
