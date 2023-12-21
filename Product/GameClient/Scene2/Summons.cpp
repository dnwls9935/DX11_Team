#include "pch.h"
#include "Summons.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Transform.h"
#include "WorldHP.h"
#include "ChessBoardDataManager.h"
#include "SlashHitFX.h"
#include "ParticleSystem.h"
#include "Animator.h"
#include "ModelRenderer.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

Summons::Summons(bool isPreInstance) noexcept
	: MonsterActor(50, isPreInstance)
	, _replaceAnimator(nullptr)
	, _isSpawnSound(false)
{
	_maxHp = 50;
	_damage = 4;
}

void Summons::Start(void) noexcept
{
	gameObject->SetActive(false);

	_rigidbody = Rigidbody::Create(false);
	_rigidbody->SetFreezeRotationX(true);
	_rigidbody->SetFreezeRotationY(true);
	_rigidbody->SetFreezeRotationZ(true);
	_rigidbody->SetAngularDrag(100);
	_rigidbody->SetDrag(100);
	_rigidbody->SetMass(100);
	gameObject->AddComponent(_rigidbody);

	_collider = Collider::CreateCapsuleCollider(1.0f, 1.0f, XMVectorSet(0, 2.f, 0, 0));
	gameObject->AddComponent(_collider);

	_movementEffectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_movementEffectPlayer);

	__super::Start();
}

void Summons::FixedUpdate(float fElapsedTime) noexcept
{
	__super::FixedUpdate(fElapsedTime);
}

void Summons::Update(float fElapsedTime) noexcept
{
	if (_isDeath)
	{
		_deathTime += fElapsedTime;
		if (_deathTime >= 1.f)
		{
			_replaceRenderer->SetEnable(false);
			_isDeath = false;
			gameObject->SetActive(false);
		}
		_replaceRenderer->GetGameObject()->GetTransform()->Translate(0, (-5 * fElapsedTime), 0);

		return;
	}

	if (_isDead)
	{
		Die();
		return;
	}

	__super::Update(fElapsedTime);
}

void Summons::LateUpdate(float fElapsedTime) noexcept
{
	__super::LateUpdate(fElapsedTime);

}

void Summons::Die(void) noexcept
{
	if (_state == Actor::State::DIE)
		return;

	_dirtyState = Actor::State::DIE;
	_state = Actor::State::DIE;

	_isSpawn = false;
	_isSpawnSound = false;
	_board->SetCheckIn(_index, false);
	_slashHit->StopFX();

	if(_collider != nullptr)
		_collider->SetEnable(false);

	if (_particle != nullptr)
		_particle->Stop();

	if (_replaceAnimator != nullptr)
	{
		_hpBar->Invisible();

		GameObject* obj = _replaceAnimator->GetGameObject();

		XMVECTOR pos = transform->GetWorldPosition();
		obj->GetTransform()->SetWorldPosition(pos);
		XMVECTOR rot = transform->GetWorldEulerAngle();
		obj->GetTransform()->SetWorldEulerAngle(rot);

		obj->GetTransform()->ImmediateUpdateForcely();

		_replaceRenderer->SetEnable(true);
		_replaceAnimator->SetPlaySpeed(1.f);

		//±âº» ¸ðµ¨¸µ ²ô±â 
		_renderer->SetEnable(false);
	}
	else
	{
		gameObject->SetActive(false);
	}
	
}

void Summons::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	__super::OnTriggerEnter(mine, other);
}

void Summons::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
	__super::OnTriggerExit(mine, other);
}

void Summons::Spawn(int index)
{
	if (_isSpawn)
		return;

	//Full hp
	_hp = _maxHp;
	_hpBar->Visible();

	_isDead = false;
	_isDeath = false;

	_isSpawn = true;
	_index = index;
	_board->SetCheckIn(_index, true);

	_dirtyState = Actor::State::JUMP;
	_isDirtyState = true;

	transform->SetWorldPosition(_board->GetPosVec(_index));
	transform->SetLocalScale(0, 0, 0);

	_collider->SetEnable(true);

	_renderer->SetEnable(true);
	gameObject->SetActive(true);


	if (_replaceAnimator != nullptr)
	{
		_replaceRenderer->SetEnable(false);

		_replaceAnimator->Play((uint)0, false, std::bind(&Summons::OnDead, this));
		_replaceAnimator->SetPlaySpeed(0.f);
	}
}

void Summons::OnDead(void) noexcept
{
	_isDeath = true;
	_deathTime = 0;
}
