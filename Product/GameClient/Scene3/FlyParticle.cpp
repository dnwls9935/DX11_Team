#include "pch.h"
#include "FlyParticle.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Actor.h"
#include "ChessBoardData.h"
#include "Transform.h"
#include "ChessBoardDataManager.h"
#include "ChessBoardData.h"

void FlyParticle::Start(void) noexcept
{
	_rigid = Rigidbody::Create(false);
	_rigid->SetIsKinematic(true);
	_rigid->SetUseGravity(false);
	_rigid->SetDrag(0);
	_rigid->SetAngularDrag(0);
	_rigid->SetMass(0);
	gameObject->AddComponent(_rigid);

	Collider* collider = Collider::CreateSphereCollider(0.001f, XMVectorSet(0, 0.5f, 0, 0));
	collider->SetIsTrigger(true);
	gameObject->AddComponent(collider);

	_board = GameObject::FindLoadingObjectByName("ChessBoardMgr")->GetComponent<ChessBoardDataManager>();
}

void FlyParticle::Update(float fElapsedTime) noexcept
{
		if (_board->isOut(transform->GetWorldPosition()))
		{
			gameObject->SetActive(false);
		}

		const auto& board = _board->GetBoard();
		bool play = false;

		const auto& iterindex = std::find_if(board.begin(), board.end(), [&](const ChessBoardDataManager::CHESSBOARD& chessboard) {
			return 2 > XMVectorGetX(XMVector3Length(transform->GetWorldPosition() - chessboard._boarddata->GetTransform()->GetWorldPosition()));
		});
		if (iterindex != board.end())
		{
			play = true;
		}

		if (play)
		{
			size_t index = std::distance(board.begin(), iterindex);
			if (!_board->GetBoardData(static_cast<uint>(index))->isPlaying())
			{
				_board->GetBoardData(static_cast<uint>(index))->Play();
			}
		}
}

void FlyParticle::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	GameObject* obj = other->GetGameObject();

	if (GameObjectTag::PLAYER == obj->GetTag())
	{
		obj->GetComponent<Actor>()->GetHit(_damage);
		gameObject->SetActive(false);
	}
	else if (GameObjectTag::OBJECT == obj->GetTag())
	{
		obj->GetComponent<ChessBoardData>()->Play();
	}
}

void FlyParticle::StartFX(void) noexcept
{
	gameObject->SetActive(true);
	
}

void FlyParticle::ClearForce(void) noexcept
{
	_rigid->ClearForce();
	_rigid->ClearTorque();
}

void FlyParticle::DestroyFX(void) noexcept
{
	gameObject->SetDestroy();
}
