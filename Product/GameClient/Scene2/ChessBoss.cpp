#include "pch.h"
#include "ChessBoss.h"
#include "GameObject.h"
#include "Transform.h"
#include "ChessKing.h"
#include "ChessQueen.h"
#include "BossHP.h"
#include "DungeonActor.h"
#include "ChessBoardDataManager.h"
#include "ChessPawn.h"
#include "ChessBishop.h"
#include "ChessKnight.h"
#include "ChessRook.h"
#include "Summons.h"
#include "Damaged.h"
#include "DungeonCinematic.h"

ChessBoss::ChessBoss(ChessBoardDataManager* chessBord) noexcept
	:_boardMgr(chessBord)
{
}

void ChessBoss::Start(void) noexcept
{
	_maxHp = 200;
	_hp = _maxHp;

	GameObject* chessKing = GameObject::Instantiate("Chess King");
	_king = new ChessKing(this, _boardMgr);
	chessKing->AddComponent(_king);

	GameObject* chessQueen = GameObject::Instantiate("Chess Queen");
	_queen = new ChessQueen(this, _boardMgr);
	chessQueen->AddComponent(_queen);

	gameObject->AddComponent(new BossHP(this));

	_prevSummon = SummonType::END;
	/* Summons */
	for (int i = 0; i < 3; i++)
	{
		std::string name = "Pawn_" + std::to_string(i);
		GameObject* obj = GameObject::Instantiate(name.c_str());
		_pawns[i] = new ChessPawn();
		_pawns[i]->SetBoard(_boardMgr);
		obj->AddComponent(_pawns[i]);

		obj->SetActive(false);
	}
	for (int i = 0; i < 2; i++)
	{
		std::string name = "Bishop_" + std::to_string(i);
		GameObject* obj = GameObject::Instantiate(name.c_str());
		_bishops[i] = new ChessBishop();
		_bishops[i]->SetBoard(_boardMgr);
		obj->AddComponent(_bishops[i]);

		obj->SetActive(false);
	}
	for (int i = 0; i < 7; i++)
	{
		std::string name = "Knight_" + std::to_string(i);
		GameObject* obj = GameObject::Instantiate(name.c_str());
		_knights[i] = new ChessKnight();
		_knights[i]->SetBoard(_boardMgr);
		obj->AddComponent(_knights[i]);

		obj->SetActive(false);
	}
	GameObject* obj = GameObject::Instantiate("Rook");
	_rook = new ChessRook();
	_rook->SetBoard(_boardMgr);
	obj->AddComponent(_rook);
	obj->SetActive(false);
}

void ChessBoss::FixedUpdate(float fElapsedTime) noexcept
{
	if (false == _playerSet)
	{
		std::vector<GameObject*> players = GameObject::FindObjectsByTag(GameObjectTag::PLAYER);
		if (players.size() == 0)
			return;

		for (int i = 0; i < players.size(); i++)
		{
			_players.push_back(players[i]->GetComponent<DungeonActor>());
		}
		_playerSet = true;
	}
}

void ChessBoss::Update(float fElapsedTime) noexcept
{
	if (_isDead)
		return;

	if (false == _playerSet)
		return;

	int kingTarget = -1;
	int queenTarget = -1;

	bool playerDead = false;
	
	float kingDist = 100;
	float queenDist = 0;

	for (int i = 0; i < _players.size(); ++i)
	{
		if (_players[i]->IsDead())
		{
			playerDead = true;
			continue;
		}
		float kingLength = XMVectorGetX(XMVector3Length(
								_players[i]->GetTransform()->GetWorldPosition() - _king->GetTransform()->GetWorldPosition()));
		float queenLength = XMVectorGetX(XMVector3Length(
								_players[i]->GetTransform()->GetWorldPosition() - _queen->GetTransform()->GetWorldPosition()));
		
		if (kingDist > kingLength)
		{
			kingDist = kingLength;
			kingTarget = i;
		}
		if (queenDist < queenLength)
		{
			queenDist = queenLength;
			queenTarget = i;
		}
	}

	if(kingTarget != -1)
		_king->SetTarget(_players[kingTarget]->GetGameObject(), false);
	else
		_king->SetTarget(nullptr, false);

	if(queenTarget != -1)
		_queen->SetTarget(_players[queenTarget]->GetGameObject(), false);
	else
		_queen->SetTarget(nullptr, false);
}

bool ChessBoss::GetHit(float damage, int8 attacker) noexcept
{
	if (_isDead)
		return true;

	_hp = _hp - damage;
	if (_hp < 0)
	{
		_hp = 0;
		_isDead = true;
		
		_king->GetTransform()->SetWorldPosition(2.31f, 0.f, -5.98f);
		_king->GetTransform()->SetWorldEulerAngle(0.f , XMConvertToRadians(260.381f), 0.f);
		_king->Die();

		_queen->GetTransform()->SetWorldPosition(-6.35f, 0.f, -7.91f);
		_queen->GetTransform()->SetWorldEulerAngle(0.f, XMConvertToRadians(-8.299f), 0.f);
		_queen->Die();

		/*	
			Chess Boss 죽을때 Cinematic 재생 
			몬스터 죽이기
			다음 Scene loading
		*/
		GameObject::FindObjectByName("Cinematic0")->GetComponent<DungeonCinematic>()->PlayChessEnd();

		const auto& monsters = GameObject::FindObjectsByTag(GameObjectTag::MONSTER);
		for (auto& monster : monsters)
		{
			MonsterActor* monsteractor = monster->GetComponent<MonsterActor>();
			if (monsteractor != nullptr)
			{
				monsteractor->GetHit(9999);
			}
		}

		if (SCENEMANAGER->IsWhileLoading() == false && SCENEMANAGER->GetPreLoadingScene() == nullptr)
		{
			SCENEMANAGER->LoadSceneAsync("Tree", false);
		}

		return true;
	}


	GameObject* obj = GameObject::Instantiate();
	obj->AddComponent(new Damaged(transform, damage, attacker));

	return false;
}

void ChessBoss::SetSummonPattern(void) noexcept
{
	_king->Attack();
}

void ChessBoss::Summon(void) noexcept
{
	//종류 4가지 중 하나 선택 (바로 직전에 나온 종류는 X)
	int summonType = CE_MATH::Random(4);
	while (summonType == (int)_prevSummon)
	{
		summonType = CE_MATH::Random(4);
	}

	_prevSummon = (SummonType)summonType;

	int summonCount = 0;
	//나이트 아니면 랜덤 위치 
	switch ((SummonType)summonType)
	{
	case SummonType::PAWN:
	{
		summonCount = CE_MATH::Random(2, 3);
		for (int i = 0; i < summonCount; ++i)
		{
			while (true)
			{
				int index = CE_MATH::Random(64);
				if (false == _boardMgr->GetCheckIn(index))
				{
					_pawns[i]->Spawn(index);
					break;
				}
			}
		}
		break;
	}
	case SummonType::BISHOP:
	{
		summonCount = CE_MATH::Random(1, 2);
		for (int i = 0; i < summonCount; ++i)
		{
			while (true)
			{
				int index = CE_MATH::Random(64);
				if (false == _boardMgr->GetCheckIn(index))
				{
					_bishops[i]->Spawn(index);
					break;
				}
			}
		}
		break;
	}
	case SummonType::KNIGHT:
	{
		summonCount = CE_MATH::Random(5, 7);
		for (int i = 0; i < summonCount; ++i)
		{
			while (true)
			{
				int index = (CE_MATH::Random(1, 8) * 8) - 1;
				if (false == _boardMgr->GetCheckIn(index))
				{
					_knights[i]->Spawn(index);
					break;
				}
			}
		}
		break;
	}
	case SummonType::ROOK:
	{
		summonCount = 1;
		while (true)
		{
			int index = CE_MATH::Random(64);
			if (false == _boardMgr->GetCheckIn(index))
			{
				_rook->Spawn(index);
				break;
			}

		}
		break;
	}
	}
}

void ChessBoss::visible(bool bvisible) noexcept
{
	_king->GetGameObject()->SetActive(bvisible);
	_queen->GetGameObject()->SetActive(bvisible);
	gameObject->GetComponent<BossHP>()->SetVisible(bvisible);
}

void ChessBoss::DistanceDamage(float dist, float damage, bool isKing) noexcept
{
	if (isKing)
	{
		for (int i = 0; i < 2; ++i)
		{
			if(_players[i]->IsDead())
				continue;
			
			float length = XMVectorGetX(XMVector3Length(_players[i]->GetTransform()->GetWorldPosition() - _king->GetTransform()->GetWorldPosition()));
			if (length <= dist)
				_players[i]->GetHit(damage);
		}
	}
}
