#include "pch.h"
#include "SquirrelCatput.h"
#include "GameObject.h"
#include "Transform.h"
#include "SquirrelCatPutBall.h"
#include "NarrationManager.h"

SquirrelCatput::SquirrelCatput(GameObject* target) noexcept
	: _target(target)
	, _isFire(false)
	, _isFired(false)
	, _deltaTime(0.0f)
{
}

void SquirrelCatput::Start(void) noexcept
{
	if (!_target)
	{
		CE_ASSERT("rldnr", "target이 없습니다.");
		return;
	}
}

void SquirrelCatput::FixedUpdate(float fElapsedTime) noexcept
{
	float length =  XMVectorGetX(XMVector3Length(transform->GetWorldPosition() - _target->GetTransform()->GetWorldPosition()));
	static bool isnarration = false;

	if (length < 300.f && _isFired == false)
	{
		_isFire		= true;
		_isFired	= true;
		if (!isnarration)
		{
			isnarration = true;
			if (!NarrationManager::This().SetCsvData("Asset\\Narrative\\Tree\\CatapultApproach.csv"))
			{
				CE_ASSERT("rldnr", "csvdata 가 없습니다.");
			}
			NarrationManager::This().SetNarrationVolume(2.f);
			NarrationManager::This().Play();
		}
	}
}

void SquirrelCatput::Update(float fElapsedTime) noexcept
{
	if (_isFire && _isFired)
	{
		transform->Rotate(0.f, -fElapsedTime * 4, 0.f);
		_deltaTime += (fElapsedTime * 4);

		if (_deltaTime > 0.85f && _isShoot == false)
		{
			_isShoot = true;

			Transform* child = transform->GetChilds()[0];

			SquirrelCatPutBall* ball = child->GetGameObject()->GetComponent<SquirrelCatPutBall>();
			ball->Fire(XMVector3Normalize(transform->GetForward()) * 8000);
		}

		if (_deltaTime > 1.0f)
		{
			_deltaTime	= 0;
			_isFire		= false;
		}
	}
}

