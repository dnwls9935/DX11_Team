#include "pch.h"
#include "FlyingSquirrel.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "PathFollower.h"
#include "GameObject.h"
#include "BoneChildObject.h"
#include "AcornShooter.h"
#include "Damaged.h"
#include "SpriteHP.h"
#include "ExplosionFX.h"

FlyingSquirrel::FlyingSquirrel(PathData* data, float speed) noexcept
	: MonsterActor(10, true)
	, _data(data)
	, _player(nullptr)
	, _speed(speed)
{
}

void FlyingSquirrel::Awake(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create(false);
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Tree\\Enemy\\FlyingSquirrel_Hanglider.fbx"));
	gameObject->AddComponent(renderer);
	renderer->SetOffsetMatrix(XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationRollPitchYaw(0, CE_MATH::ToRadian(90), 0));
	GameObject* body = GameObject::Instantiate();
	ModelRenderer * renderer2 = ModelRenderer::Create();
	renderer2->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Tree\\Enemy\\FlyingSquirrel_Body.fbx"));
	renderer2->SetOffsetMatrix(XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationRollPitchYaw(0, CE_MATH::ToRadian(90), 0));
	body->AddComponent(renderer2);
	body->GetTransform()->SetParent(transform);

	_follower = PathFollower::Create(_data);
	gameObject->AddComponent(_follower);

	GameObject* obj = GameObject::Instantiate("Gun_R");
	obj->GetTransform()->SetParent(transform);
	obj->AddComponent(new BoneChildObject(renderer->GetModel()->GetHierarchyNode("Gun_Right")
										, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationRollPitchYaw(0, CE_MATH::ToRadian(90), 0)));
										
	obj->AddComponent(_right = new AcornShooter(GameObjectTag::PLAYER));
	_right->Play(10, 300, true, 0.6f);

	obj = GameObject::Instantiate("Gun_L");
	obj->GetTransform()->SetParent(transform);
	obj->AddComponent(new BoneChildObject(renderer->GetModel()->GetHierarchyNode("Gun_Left")
										, XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationRollPitchYaw(0, CE_MATH::ToRadian(90), 0)));
										
	obj->AddComponent(_left = new AcornShooter(GameObjectTag::PLAYER));
	_left->Play(10, 300, true, 0.6f);

	
	_rigid = Rigidbody::Create(false);
	//_rigid->SetIsKinematic(true);
	_rigid->SetUseGravity(true);
	gameObject->AddComponent(_rigid);

	_collider = Collider::CreateBoxCollider(5, 3, 5);
	gameObject->AddComponent(_collider);

	Collider* trigger  = Collider::CreateBoxCollider(5, 3, 5);
	gameObject->AddComponent(trigger);
}

void FlyingSquirrel::Start(void) noexcept
{
	/* HP Bar */
	_spriteHpBar = new SpriteHP(this,Vector4(1,0,0,1));
	gameObject->AddComponent(_spriteHpBar);
	_spriteHpBar->Invisible();

	/* Dead Fx */
	GameObject* obj = GameObject::Instantiate("SquirrelTurrentDieFX");
	obj->AddComponent(_fx = new ExplosionFX());

	gameObject->SetTag(GameObjectTag::MONSTER);
}

void FlyingSquirrel::FixedUpdate(float fElapsedTime) noexcept
{

}

void FlyingSquirrel::Update(float fElapsedTime) noexcept
{
	if (_player == nullptr)
	{
		_player = GameObject::FindObjectByTag(GameObjectTag::PLAYER)->GetTransform();

	}

	float dist = XMVectorGetX(XMVector3Length(_player->GetWorldMatrix().r[3] - transform->GetWorldMatrix().r[3]));
	if (dist < 300)
	{
		if (!_visibleHp)
		{
			_visibleHp = true;
			_spriteHpBar->Visible();
		}
	}
	else
	{
		if (_visibleHp)
		{
			_visibleHp = false;
			_spriteHpBar->Invisible();
		}
	}
}

void FlyingSquirrel::LateUpdate(float fElapsedTime) noexcept
{
}

void FlyingSquirrel::OnEnable(void) noexcept
{
	_follower->Play(_speed, false, std::bind(&FlyingSquirrel::OnEndPath, this));
}

void FlyingSquirrel::OnDisable(void) noexcept
{
}

void FlyingSquirrel::OnDestroy(void) noexcept
{
	_spriteHpBar->Invisible();

	Transform::TList& childs = const_cast<Transform::TList&>(transform->GetChilds());

	for (int i = 0; i < childs.size(); ++i)
	{
		GameObject::Destroy(childs[i]->GetGameObject());
	}
}

void FlyingSquirrel::OnEndPath(void) noexcept
{
	_left->Stop();
	_right->Stop();

	GameObject::Destroy(gameObject);
}

void FlyingSquirrel::Die(void) noexcept
{
	if (Actor::State::DIE == _state)
		return;

	_state = Actor::State::DIE;
	// Dead Explosion
	_fx->StartFX(transform->GetWorldMatrix().r[3]);


	OnEndPath();
}

void FlyingSquirrel::GetHit(float damage, int8 attacker) noexcept
{
	if (_isDead)
		return;

	_hp = _hp - damage;
	if (_hp <= 0)
	{
		_hp = 0;
		_isDead = true;
		Die();
	}
	//else
	//{
	//	GameObject* obj = GameObject::Instantiate();
	//	obj->AddComponent(new Damaged(transform, damage, attacker));
	//}
}

void FlyingSquirrel::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	if (_isDead)
		return;

	if (other->GetGameObject()->GetTag() == GameObjectTag::PLAYER)
	{
		GetHit(3);
		other->GetGameObject()->GetComponent<Actor>()->GetHit(15);
	}
}

