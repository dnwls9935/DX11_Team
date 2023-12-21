#include "pch.h"
#include "RookFX.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Transform.h"
#include "FlyParticle.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Spike.h"
#include "ChessBoardDataManager.h"
#include "Actor.h"
#include "RookSpikeCollider.h"
#include "ChessRook.h"


ChessRookFX::ChessRookFX(void) noexcept
	: _lerpTime(0.f)
	, _start(false)
	, _explosion(false)
	, _oldpos(0.f)
	, _count(0)
	, _colliderWidth(false)
	, _rookFX{}
{
}

void ChessRookFX::Start(void) noexcept
{
	for (int i = 0; i < 4; i++)
	{
		_rookFX[i]._moveObj = GameObject::Instantiate("ChessRookFX");
		_rookFX[i]._moveObj->GetTransform()->SetLocalEulerAngle(0, XMConvertToRadians((i * 90.f) - CHESS_SCENE_ANGLE), 0);
		_rookFX[i]._moveObj->GetTransform()->SetLocalPosition(XMVectorSet(0, 0, 0, 1));
		_rookFX[i]._moveObj->SetActive(false);

		_rookFX[i]._moveObj->AddComponent(new RookSpikeCollider());

		/* ==================================================== */
		_rookFX[i]._ColliderObj = _rookFX[i]._moveObj->GetComponent<Collider>();

		_rookFX[i]._moveObj->SetLayer(GameObjectLayer::OBJECT);

	}

	for (int i = 0; i < numspike; ++i)
	{
		GameObject* obj = GameObject::Instantiate(("Spike" + std::to_string(i)).c_str());
		_spike[i] = new Spike();
		obj->AddComponent(_spike[i]);
		obj->SetActive(false);
	}

}

void ChessRookFX::FixedUpdate(float fElapsedTime) noexcept
{
	if (!_colliderWidth)
	{
		for (int i = 0; i < 4; ++i)
		{
			XMVECTOR dir = XMVector3Normalize(_rookFX[i]._moveObj->GetTransform()->GetForward());
			if (i < 2)
			{
				_rookFX[i]._ColliderObj->SetOffsetPos(XMVectorSet(0.f, 2.f, XMVectorGetZ(-dir) * 5.f, 1.f));
			}
			else
			{
				_rookFX[i]._ColliderObj->SetOffsetPos(XMVectorSet(0.f, 2.f, XMVectorGetZ(dir) * 5.f, 1.f));
			}
		}
		_colliderWidth = true;
	}

	

}

void ChessRookFX::Update(float fElapsedTime) noexcept
{
	if (true == _explosion)
	{
		XMVECTOR dir{};
		XMVECTOR right{};
		XMVECTOR objpos{};
		_oldpos += fElapsedTime ;

		for (int i = 0; i < 4; ++i)
		{
			if (!_rookFX[i]._moveObj->GetActive())
				continue;

			dir = XMVector3Normalize(_rookFX[i]._moveObj->GetTransform()->GetForward());
			right = XMVector3Normalize(_rookFX[i]._moveObj->GetTransform()->GetRight());
			
			_rookFX[i]._moveObj->GetTransform()->Translate(dir * fElapsedTime * 20.f);
			objpos = _rookFX[i]._moveObj->GetTransform()->GetWorldPosition();
			
			if (_boardmanager->isOut(objpos))
			{
				_rookFX[i]._moveObj->SetActive(false);
			}

			if (_oldpos > 0.05f)
			{
				_spike[_count]->GetGameObject()->SetActive(true);
				_spike[_count++]->SetDirection(objpos,right);
					
				if (_count >= numspike -1)
					_count = 0;
			}

		}
		if ( _oldpos > 0.05f)
		{
			_oldpos = 0.f;
		}
	}
}

void ChessRookFX::LateUpdate(float fElapsedTime) noexcept
{
	bool die = true;
	for (int i = 0; i < 4; ++i)
	{
		if (_rookFX[i]._moveObj->GetActive())
			die = false;
	}
	if (die && _explosion)
	{
		_explosion = false;
		transform->GetParent()->GetGameObject()->GetComponent<ChessRook>()->Die();
	}
}

void ChessRookFX::DieReset(void) noexcept
{
	_explosion = false;
}

void ChessRookFX::StartFX(void) noexcept
{
}

void ChessRookFX::Explosion(void) noexcept
{
	_start = false;
	for (int i = 0; i < 4; i++)
	{
		_rookFX[i]._moveObj->SetActive(true);
		XMVECTOR position = transform->GetParent()->GetWorldMatrix().r[3];
		_rookFX[i]._moveObj->GetTransform()->SetWorldPosition(XMVectorGetX(position), 0, XMVectorGetZ(position));
		_rookFX[i]._ColliderObj->SetEnable(true);
	}
	_lerpTime = 0.f;
	_explosion = true;
}
