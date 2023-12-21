#include "pch.h"
#include "ExplosionSphere.h"

#include "GameObject.h"
#include "Transform.h"
#include "LightningSphere.h"

ExplosionSphere::ExplosionSphere(void) noexcept
{
}

void ExplosionSphere::Start(void) noexcept
{
	_lightningSphere = GameObject::Instantiate("LightningSphere");
	LightningSphere* lightningSphere = new LightningSphere();
	_lightningSphere->AddComponent(lightningSphere);

}

void ExplosionSphere::Update(float fElapsedTime) noexcept
{
}

void ExplosionSphere::StartFX(void) noexcept
{
}
