#include "pch.h"
#include "VacuumHub.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Animator.h"


VacuumHub::VacuumHub(void) noexcept
{

}

VacuumHub::~VacuumHub(void) noexcept
{

}

void VacuumHub::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
							* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(270)
																						, CE_MATH::ToRadian(90)
																						, CE_MATH::ToRadian(90)));

	if ("Vacuum_HubSmall" == gameObject->GetName() ||
		"Vacuum_HubSmall_Move" == gameObject->GetName())
	{
		renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Character\\VacuumHub\\VacuumHub_Small.fbx"));

		if ("Vacuum_HubSmall_Move" == gameObject->GetName())
		{
			offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

			Rigidbody* rigid = Rigidbody::Create(false);
			rigid->SetUseGravity(false);
			rigid->SetIsKinematic(true);
			rigid->SetDrag(0);
			rigid->SetAngularDrag(0);
			gameObject->AddComponent(rigid);
			collider = Collider::CreateBoxCollider(4.5f, 3, 4.5f, XMVectorSet(0, 5.3f, 0, 1));
			gameObject->AddComponent(collider);

			GameObject* counterWeight = GameObject::Instantiate("Counterweight");
			counterWeight->SetTag(GameObjectTag::GROUND);

			ModelRenderer* counterWeightRenderer = ModelRenderer::Create();
			counterWeightRenderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\VacuumPlatform\\VacuumPlatform.fbx"));
			counterWeightRenderer->SetOffsetMatrix(offsetmatrix);
			counterWeight->AddComponent(counterWeightRenderer);

			counterWeight->GetTransform()->SetParent(transform);
			counterWeight->GetTransform()->SetLocalPosition(0, 3.0f, 0);
			XMVECTOR rotation = CE_MATH::Deg2Rad * XMVectorSet(0, 0, 0, 1);
			counterWeight->GetTransform()->Rotate(rotation);
			counterWeight->GetTransform()->SetLocalScale(1.5f, 1, 1.5f);
		}

		renderer->SetOffsetMatrix(offsetmatrix);
		gameObject->AddComponent(renderer);

		_animator = Animator::Create();
		gameObject->AddComponent(_animator);
		_animator->Play("VacuumHubSmall.ao|VacuumHubSmall_mh"
			, true);
		_animator->SetPlaySpeed(1.f);
	}
	else if ("Vacuum_HubBig" == gameObject->GetName())
	{
		renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Character\\VacuumHub\\VacuumHub_Big.fbx"));

		renderer->SetOffsetMatrix(offsetmatrix);
		gameObject->AddComponent(renderer);

		_animator = Animator::Create();
		gameObject->AddComponent(_animator);
		_animator->Play("VacuumHubBig.ao|VacuumHubBig_ON_MH"
			, true);
		_animator->SetPlaySpeed(1.f);
	}
	else
	{
		renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Character\\VacuumHub\\VacuumHub.fbx"));

		renderer->SetOffsetMatrix(offsetmatrix);
		gameObject->AddComponent(renderer);

		_animator = Animator::Create();
		gameObject->AddComponent(_animator);
		_animator->Play("VacuumHub.ao|VacuumHub_MH"
			, true);
		_animator->SetPlaySpeed(1.f);
	}
}

void VacuumHub::FixedUpdate(float fElapsedTime) noexcept
{
}

void VacuumHub::Update(float fElapsedTime) noexcept
{
	if ("Vacuum_HubSmall_Move" == gameObject->GetName())
	{
		if (_direction == true)
		{
			_range += fElapsedTime * 4.f;
			transform->Translate(XMVectorSet(0, 0, 1, 0) * fElapsedTime * 4.f);
		}
		else
		{
			_range += fElapsedTime * 4.f;
			transform->Translate(XMVectorSet(0, 0, 1, 0) * fElapsedTime * -4.f);
		}

	}
}

void VacuumHub::LateUpdate(float fElapsedTime) noexcept
{
	if (20 <= _range)
	{
		_direction = !_direction;
		_range = 0;
	}
}
