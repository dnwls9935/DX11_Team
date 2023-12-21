#include "pch.h"
#include "TargetDummy.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Transform.h"

TargetDummy::TargetDummy(void) noexcept
	:MonsterActor(0)
{
}

void TargetDummy::Start(void) noexcept
{
	_renderer = ModelRenderer::Create(false);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\TargetDummy\\Target_Dummy.fbx"));
	gameObject->AddComponent(_renderer);

	_rigidbody = Rigidbody::Create(false);
	_rigidbody->SetFreezePositionY(true);
	_rigidbody->SetFreezeRotationY(true);
	_rigidbody->SetDrag(5);
	_rigidbody->SetAngularDrag(0);
	_rigidbody->SetMass(1);
	_rigidbody->SetUseGravity(true);
	gameObject->AddComponent(_rigidbody);

	Collider* collider = Collider::CreateBoxCollider(0.5f, 1, 0.5f, XMVectorSet(0, 1, 0, 0));
	gameObject->AddComponent(collider);
	gameObject->SetTag(GameObjectTag::MONSTER);
	gameObject->SetLayer(GameObjectLayer::OBJECT);
}

void TargetDummy::Update(float fElapsedTime) noexcept
{
	if (_colorChanged)
	{
		_colorChangeDelta += fElapsedTime;
		if (_colorChangeDelta > 0.1f)
		{
			_colorChanged = false;
			_colorChangeDelta = 0;
		}
		Material::TList& mats = _renderer->GetMaterials();
		for (Material& mat : mats)
			mat.SetValue(ShaderSemantic::Type::BOOLEAN_USE_FORCECOLOR, (void*)&_colorChanged, sizeof(bool));
	}
}

void TargetDummy::GetHit(float damage, int8 attacker) noexcept
{
	XMVECTOR norVec = XMVector3Normalize( transform->GetForward());
 //	_rigidbody->AddForce(norVec * -300);

	Material::TList& mats = _renderer->GetMaterials();
	bool useForceColor = true;

	for (Material& mat : mats)
		mat.SetValue(ShaderSemantic::Type::BOOLEAN_USE_FORCECOLOR, (void*)&useForceColor, sizeof(bool));

	_colorChanged = true;
	_colorChangeDelta = 0;
}

void TargetDummy::Idle(void) noexcept
{
}

void TargetDummy::Walk(void) noexcept
{
}

void TargetDummy::Attack(void) noexcept
{
}

void TargetDummy::Die(void) noexcept
{
}
