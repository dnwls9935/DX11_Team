#include "pch.h"
#include "Destruction.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

Destruction::Destruction(float maxHp) noexcept
	:MonsterActor(maxHp, true)
	, _breakEffectPlayer(nullptr)
	, _breakAsset(nullptr)
{
}

void Destruction::Start(void) noexcept
{
	gameObject->SetTag(GameObjectTag::MONSTER);
	_smashed = false;

	_breakEffectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_breakEffectPlayer);
}

void Destruction::Update(float fElapsedTime) noexcept
{
	if (false == _smashed)
		return;

	if (_smashed)
	{
		transform->Translate(0.f, -fElapsedTime * 2.f, 0.f, false);
		if (XMVectorGetY(transform->GetWorldPosition()) <= -10.f)
		{
			gameObject->SetActive(false);
		}
		return;
	}
}

void Destruction::GetHit(float damage, int8 attacker) noexcept
{
	if (_isDead)
		return;

	_hp = _hp - damage;
	if (_hp < 0)
	{
		_hp = 0;
		_isDead = true;
		Die();
	}

}

void Destruction::Die(void) noexcept
{
	gameObject->SetDestroy();
}

void Destruction::Idle(void) noexcept
{
	_smashed = true;
}

void Destruction::Walk(void) noexcept
{
}

void Destruction::Attack(void) noexcept
{
}

void Destruction::OnEndAnimation(void) noexcept
{
	gameObject->SetDestroy();
}
