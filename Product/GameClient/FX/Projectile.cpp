#include "pch.h"
#include "Projectile.h"
#include "GameObject.h"
#include "Transform.h"
#include "Collider.h"

Projectile::Projectile(Vector3 direction, float speed, GameObjectTag enemyTag, float damage, float deletetime, bool rotate) noexcept
	: _direction(direction)
	, _speed(speed)
	, _enemyTag(enemyTag)
	, _damage(damage)
	, _rotate(rotate)
	, _deleteTime(deletetime)
{

}

void Projectile::Start(void) noexcept
{
	/* Rotate */
	XMVECTOR forward = transform->GetForward();
	forward = XMVectorSetY(forward, 0);

	float dot = XMVectorGetX(XMVector3Dot((XMVECTOR)_direction, forward));
	float angle = acosf(dot);
	XMVECTOR cross = XMVector3Cross(transform->GetForward(),(XMVECTOR) _direction);
	if (XMVectorGetY(cross) < 0)
		angle *= -1;

	float dot2 = XMVectorGetX(XMVector3Dot((XMVECTOR)_direction, XMVectorSet(0, 1, 0, 0)));
	float angle2 = acosf(dot2);
	XMVECTOR cross2 = XMVector3Cross(XMVectorSet(0, 1, 0, 0), (XMVECTOR)_direction);
	if (XMVectorGetY(cross2) < 0)
		angle2 *= -1;

	if(_rotate)
		transform->Rotate(0, angle, 0, false);
	else
	{
		transform->Rotate(angle2 - CE_MATH::ToRadian(90), angle ,0 ,false);
	}
}

void Projectile::Update(float fElapsedTime) noexcept
{
	XMVECTOR dir =(XMVECTOR)_direction * _speed * fElapsedTime;
	transform->Translate(dir, false);

	_time += fElapsedTime;
	if (_time >= _deleteTime)
	{
		DestroyObject();
	}
}

void Projectile::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	if (other->GetGameObject()->GetTag() == _enemyTag)
	{
		other->GetGameObject()->GetComponent<Actor>()->GetHit(_damage);
		DestroyObject();
	}
	if (other->GetGameObject()->GetTag() == GameObjectTag::DEFAULT)
	{
		DestroyObject();
	}
}

void Projectile::DestroyObject()
{
	const std::vector<Transform*>& childs = transform->GetChilds();
	if (childs.size() > 0)
	{
		for (int i = 0; i < childs.size(); ++i)
		{
			GameObject::Destroy(childs[i]->GetGameObject());
		}
	}

	gameObject->SetDestroy();
}
