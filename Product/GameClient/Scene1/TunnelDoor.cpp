#include "pch.h"
#include "TunnelDoor.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "NarrationManager.h"

bool TunnelDoor::_closeNarration = false;

TunnelDoor::TunnelDoor(GameObject* target, int8 cardinalNESW) noexcept
	: _target(target)
	, _isClosed(false)
	, _isClose(false)
	, _closeTime(0.f)
	, _cardinalDir(cardinalNESW)
{

}

void TunnelDoor::Start(void) noexcept
{
	if (!_target)
	{
		CE_ASSERT("rldnr", "target이 없습니다.");
		return;
	}
}

void TunnelDoor::FixedUpdate(float fElapsedTime) noexcept
{
	if (!_isClose && !_isClosed && (XMVectorGetX(XMVector3Length(transform->GetWorldPosition() - _target->GetTransform()->GetWorldPosition())) < 300.f))
	{
		_isClose = true;

		if (!_closeNarration && 
			(_cardinalDir == 2 || _cardinalDir == 4))
		{
			_closeNarration = true;
			if (!NarrationManager::This().SetCsvData("Asset\\Narrative\\Tree\\FirstGatesClose.csv"))
			{
				CE_ASSERT("rldnr", "NarrationData가 없습니다.");
			}
			NarrationManager::This().SetNarrationVolume(2.f);
			NarrationManager::This().Play();
		}
	}
}

void TunnelDoor::Update(float fElapsedTime) noexcept
{

	if (_isClose)
	{
		_closeTime += fElapsedTime;

		if (_closeTime < 8.3f)
		{
			if (_cardinalDir == 1)
				transform->Translate(0.f, fElapsedTime * 5.4f * -1.f, 0.f);
			else if (_cardinalDir == 2)
				transform->Translate(0.f, 0.f, fElapsedTime * 5.4f * -1.f);
			else if (_cardinalDir == 3)
				transform->Translate(0.f, fElapsedTime * 5.4f, 0.f);
			else if (_cardinalDir == 4)
				transform->Translate(0.f, 0.f, fElapsedTime * 5.4f);
			else /* up vector*/
				transform->Translate(-XMVector3Normalize(transform->GetWorldMatrix().r[2]) * fElapsedTime * 5.5f);
		}
		else
		{
			_isClosed = true;
			_isClose = false;
		}
	}
}

void TunnelDoor::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	int a = 0;
}

