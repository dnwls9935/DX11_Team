#include "pch.h"
#include "BulletFlare.h"
#include "GameObject.h"
#include "Transform.h"
#include "TinyExplosion.h"

BulletFlare::BulletFlare(void) noexcept
{
}

void BulletFlare::Start(void) noexcept
{

	GameObject* obj = GameObject::Instantiate("BulletFlare");
	obj->AddComponent(_tinyExplosion = new TinyExplosion());
	obj->GetTransform()->SetParent(transform);
}

void BulletFlare::Update(float fElapsedTime) noexcept
{

	transform->SetLocalPosition(0, 0, 0);
}

void BulletFlare::StartFX(void) noexcept
{
	_start = true;
	_tinyExplosion->StartFX();
}
