#include "pch.h"
#include "CageBottom.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "BoneChildObject.h"
#include "HierarcyNode.h"
#include "ModelRenderer.h"
#include "Actor.h"

CageBottom::CageBottom(GameObject* flyingMachineObject, XMMATRIX modelOffsetMatrix) noexcept
	: _flyingMachineObject(flyingMachineObject)
	, _offsetMat(modelOffsetMatrix)
{
}

void CageBottom::Start(void) noexcept
{
	HierarcyNode* turretBaseBone = _flyingMachineObject->GetComponent<ModelRenderer>()->GetModel()->GetHierarchyNode("TurretBase");
	transform->SetParent(_flyingMachineObject->GetTransform());
	gameObject->AddComponent(new BoneChildObject(turretBaseBone, _offsetMat));

	_rigid = Rigidbody::Create(true);
	_rigid->SetUseGravity(false);
	gameObject->AddComponent(_rigid);

	_collider = Collider::CreateBoxCollider(5, 0.1f, 7.5f, XMVectorSet(0, -0.25f, 0, 0));
	_collider->SetIsTrigger(false);
	gameObject->AddComponent(_collider);

	_trigger = Collider::CreateBoxCollider(5, 0.1f, 7.5f, XMVectorSet(0, -0.25f, 0, 0));
	_trigger->SetIsTrigger(true);
	gameObject->AddComponent(_trigger);


	gameObject->SetTag(GameObjectTag::GROUND);
}

void CageBottom::Update(float fElapsedTime) noexcept
{
}

void CageBottom::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	GameObject* obj = other->GetGameObject();

	if (obj->GetTag() == GameObjectTag::WALL)
	{
		_flyingMachineObject->GetComponent<Actor>()->Die();
	}
}

void CageBottom::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
}
