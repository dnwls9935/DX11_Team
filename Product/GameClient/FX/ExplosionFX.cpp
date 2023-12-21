#include "pch.h"
#include "ExplosionFX.h"
#include "GameObject.h"
#include "Transform.h"
#include "BigExplosion.h"
#include "AdditionalSmoke.h"
#include "ExplosionShockWave.h"
#include "Debris.h"

ExplosionFX::ExplosionFX(void) noexcept
{
}

void ExplosionFX::Start(void) noexcept
{
	GameObject* obj = GameObject::Instantiate("BigExplosion");
	obj->GetTransform()->SetParent(transform);
	obj->GetTransform()->SetLocalPosition(0, 0, 0);
	obj->AddComponent(_bigExplosion = new BigExplosion());

	obj = GameObject::Instantiate("AdditionalSmoke");
	obj->GetTransform()->SetParent(transform);
	obj->GetTransform()->SetLocalPosition(0, 0, 0);
	obj->AddComponent(_additionalSmoke = new AdditionalSmoke());

	obj = GameObject::Instantiate("ExplosionShockWave");
	obj->GetTransform()->SetParent(transform);
	obj->GetTransform()->SetLocalPosition(0, 0, 0);
	obj->AddComponent(_explosionShockWave = new ExplosionShockWave());

	obj = GameObject::Instantiate("ExplosionDebris");
	obj->GetTransform()->SetParent(transform);
	obj->GetTransform()->SetLocalPosition(0, 0, 0);
	obj->AddComponent(_debris = new Debris());
} 

void ExplosionFX::Update(float fElapsedTime) noexcept
{
}

void ExplosionFX::StartFX(XMVECTOR position) noexcept
{
	transform->SetWorldPosition(position);

	_bigExplosion->StartFX();
	//_additionalSmoke->StartFX();
	_explosionShockWave->StartFX();
	_debris->StartFX();
}
