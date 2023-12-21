#include "pch.h"
#include "VacuumBoss.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Animator.h"

VacuumBoss::VacuumBoss(void) noexcept
{
}

VacuumBoss::~VacuumBoss(void) noexcept
{

}

void VacuumBoss::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Character\\VacuumBoss\\VacuumBoss.fbx"));


	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
								* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(0
																							, CE_MATH::ToRadian(180.f)
																							, 0));

	renderer->SetOffsetMatrix(offsetmatrix);
	gameObject->AddComponent(renderer);

	transform->SetLocalPosition(-62.28f, -24.69f, -143.3678f);
	transform->SetLocalScale(2, 2, 2);

	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play("VacuumBossHead.ao|VacuumBoss_Mh", true);
	_animator->SetPlaySpeed(1.f);
}

void VacuumBoss::Update(float fElapsedTime) noexcept
{
}
