#include "pch.h"
#include "AcornShooter.h"
#include "AcornBullet.h"
#include "GameObject.h"
#include "Transform.h"

AcornShooter::AcornShooter(GameObjectTag tag) noexcept
	: _enemyTag(tag)
{
}

void AcornShooter::Awake(void) noexcept
{

}

void AcornShooter::Start(void) noexcept
{

}

void AcornShooter::FixedUpdate(float fElapsedTime) noexcept
{
	
}

void AcornShooter::Update(float fElapsedTime) noexcept
{
	if (_isPlaying)
	{
		_deltaTime += fElapsedTime;

		if (_deltaTime >= _shootingTime)
		{
			Fire(XMVector3Normalize(transform->GetForward()));
			_deltaTime = 0;
		}
	}
}

void AcornShooter::LateUpdate(float fElapsedTime) noexcept
{

}

void AcornShooter::OnEnable(void) noexcept
{

}

void AcornShooter::OnDisable(void) noexcept
{

}

void AcornShooter::Play( const float damage
						, const float speed
						, const bool loop
						, const float reFireTime) noexcept
{
	_bulletDamage	= damage;
	_bulletSpeed	= speed;
	_shootingTime	= reFireTime;

	_isPlaying		= loop;
	_deltaTime		= 0;

	Fire(XMVector3Normalize(transform->GetForward()));
}

void AcornShooter::Play( const float damage
						, const float speed
						, const FXMVECTOR dir) noexcept
{
	_bulletDamage	= damage;
	_bulletSpeed	= speed;

	Fire(dir);
}

void AcornShooter::Stop(void) noexcept
{
	_isPlaying		= false;
}

void AcornShooter::Fire(FXMVECTOR direction) noexcept
{
	GameObject* obj = GameObject::Instantiate("Bullet", true);
	obj->GetTransform()->SetWorldPosition(transform->GetWorldMatrix().r[3]);
	obj->AddComponent(new AcornBullet(direction, _bulletSpeed, _bulletDamage, _enemyTag));
}
