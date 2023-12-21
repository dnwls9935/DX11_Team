#include "pch.h"
#include "SpinningTrap.h"

#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ModelRenderer.h"
#include "Animator.h"

#include "Actor.h"
#include "Light.h"

SpinningTrap::SpinningTrap(void) noexcept
	: MonsterActor(30, true)
	, _start{}
	, _end{}
	, _to{}
	, _damage(1.f)
	, _rotate(false)
	, _smashed(false)
{
}

SpinningTrap::~SpinningTrap(void) noexcept
{
}

void SpinningTrap::Start(void) noexcept
{
	_renderer = ModelRenderer::Create();
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Spinning_Trap_01\\Spinning_Trap_01.fbx"));
	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	_renderer->SetOffsetMatrix(offsetMat);
	gameObject->AddComponent(_renderer);

	_rigid = Rigidbody::Create(false);
	_rigid->SetIsKinematic(true);
	gameObject->AddComponent(_rigid);

	_triggerCollider = Collider::CreateCapsuleCollider(1.5f, 2.5f);
	_triggerCollider->SetIsTrigger(true);
	gameObject->AddComponent(_triggerCollider);

	_collider = Collider::CreateCapsuleCollider(1.5f, 2.5f);
	gameObject->AddComponent(_collider);

	_die = false;

}

void SpinningTrap::Update(float fElapsedTime) noexcept
{
	if (_smashed)
	{
		//transform->Translate(0.f, -fElapsedTime * 2.f, 0.f, false);
		//if (XMVectorGetY(transform->GetWorldPosition()) <= -10.f)
		//{
		//	gameObject->SetActive(false);
		//}
		return;
	}

	transform->Rotate(0, fElapsedTime * 5.f, 0, 0);
		
	XMVECTOR to{XMLoadFloat3(&_to)};
	if (_rotate)
	{
		to *= -1;
	}

	to *= fElapsedTime * 10.f;
		
	transform->Translate(to, false);
	if (XMVectorGetX(XMVector3Length(transform->GetWorldPosition() - XMLoadFloat3(&_start))) > _length
		|| XMVectorGetX(XMVector3Length(transform->GetWorldPosition() - XMLoadFloat3(&_end))) > _length)
	{
		_rotate = !_rotate;
	}
				
}
void SpinningTrap::Idle(void) noexcept
{
	gameObject->SetActive(false);
}
void SpinningTrap::Walk(void) noexcept
{
	_smashed = true;
}
void SpinningTrap::Die(void) noexcept
{
	if (true == _die)
		return;
	_die = true;
	_smashed = true;
	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Spinning_Trap_01\\BreakTrap\\SpinningTrap3.fbx"));
	_renderer->SetOffsetMatrix(offsetMat);
	
	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play((uint16)0, false, std::bind(&SpinningTrap::Idle, this), 0.8f, std::bind(&SpinningTrap::Walk, this));
	_animator->SetPlaySpeed(1.f);

	_collider->SetEnable(false);
	_triggerCollider->SetEnable(false);
}

void SpinningTrap::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	GameObject* obj = other->GetGameObject();
	
	if (GameObjectTag::PLAYER == obj->GetTag())
	{
		obj->GetComponent<Actor>()->GetHit(_damage);
	}
	else if (obj->GetName() == "ToyCrusher")
	{
		//_smashed = true;
		Die();
	}
	

}

void SpinningTrap::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
}

void SpinningTrap::SetStartEnd(FXMVECTOR start, FXMVECTOR end) noexcept
{
	XMStoreFloat3(&_start, start);
	XMStoreFloat3(&_end, end);

	XMStoreFloat3(&_to, XMVector3Normalize(end - start));
	_length = XMVectorGetX(XMVector3Length(end - start));
}
