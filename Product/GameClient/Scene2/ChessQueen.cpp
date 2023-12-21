#include "pch.h"
#include "ChessQueen.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Animator.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ChessBoss.h"
#include "ChessBoardDataManager.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

ChessQueen::ChessQueen(ChessBoss* chessBoss, ChessBoardDataManager* boardManager) noexcept
	: MonsterActor(0)
	, _bossMgr(chessBoss)
	, _boardMgr(boardManager)
{
}

void ChessQueen::Start(void) noexcept
{
	//시작 위치 지정 
	_curIndex = 3;
//	XMVECTOR pos = (XMVECTOR)_bossMgr->GetBoardPos(_curIndex);
	XMVECTOR pos = _boardMgr->GetPosVec(_curIndex);
//	transform->SetParent(nullptr);
	transform->SetWorldPosition(pos);
	transform->SetWorldEulerAngle(0, CE_MATH::ToRadian(140), 0);

	_curDir = Direction::BACK;
	_boardMgr->SetCheckIn(_curIndex,true);

	/* Model Renderer */
	_renderer = ModelRenderer::Create();
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessQueen\\ChessQueen.fbx"));
	gameObject->AddComponent(_renderer);

	XMMATRIX offsetMat = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(CE_MATH::ToRadian(-90.f));
	_renderer->SetOffsetMatrix(offsetMat);

	/* Animator */
	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play("ChessQueen.ao|Enemy_PlayRoom_ChessQueen_mh", true);

	/* Rigidbody */
	_rigidbody = Rigidbody::Create(false);
	_rigidbody->SetFreezeRotationX(true);
	_rigidbody->SetFreezeRotationZ(true);
	_rigidbody->SetAngularDrag(100);
	_rigidbody->SetDrag(100);
	_rigidbody->SetMass(150);
	_rigidbody->SetUseGravity(true);
	gameObject->AddComponent(_rigidbody);

	/* Collider */
	_collider = Collider::CreateCapsuleCollider(1.5f, 2, XMVectorSet(0, 3.7f, 0, 0));
	gameObject->AddComponent(_collider);

	_trigger = Collider::CreateBoxCollider(1.f, 2, 1.f, XMVectorSet(0, 2, 1, 0));
	_trigger->SetIsTrigger(true);
	gameObject->AddComponent(_trigger);

	_trigger->SetEnable(false);

	gameObject->SetTag(GameObjectTag::MONSTER);

	_state = State::END;

	_wait = true;

	_dieObject = GameObject::Instantiate("QueenDie");
	_dieObject->GetTransform()->SetParent(transform);
	_dieObject->GetTransform()->SetLocalPosition(0, 0, 0);
	_dieObject->GetTransform()->SetLocalEulerAngle(0, XMConvertToRadians(-90.f), 0);
	_dieTransform = GetTransform();
	_dieRenderer = ModelRenderer::Create();
	_dieObject->AddComponent(_dieRenderer);
	_dieRenderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessQueen\\Queen_Dead.fbx"));
	_dieRenderer->SetOffsetMatrix(XMMatrixScaling(0.015f, 0.015f, 0.015f));
	_dieRenderer->SetEnable(false);

	_dieAnimator = Animator::Create();
	_dieAnimator->Play((uint16)0, false);
	_dieAnimator->SetPlaySpeed(0.f);
	_dieObject->AddComponent(_dieAnimator);

	_audioasset[static_cast<uint>(eAudioType::ATTACK)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_VO_DB_Chessboard_BossFight_Effort_Attack_Heavy_Queen_020.ogg");
	_audioasset[static_cast<uint>(eAudioType::CHARGE)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_VO_DB_Chessboard_BossFight_Effort_Charge_Queen_030.ogg");
	_audioasset[static_cast<uint>(eAudioType::INTRO)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_VO_DB_Chessboard_BossFight_Effort_Intro_Queen_030.ogg");
	_audioasset[static_cast<uint>(eAudioType::SUMMON)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_VO_DB_Chessboard_BossFight_Effort_Summon_Queen_050.ogg");


	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);

}

void ChessQueen::Update(float fElapsedTime) noexcept
{
	if (_isDead)
		return;

	__super::Update(fElapsedTime);

	switch (_state)
	{
	case State::IDLE:
		if (_target == nullptr)
			break;
		if (_wait)
		{
			_waitTime += fElapsedTime;
			if(_waitTime >= 0.5f)
			{
				_wait = false;
			}
			break;
		}

		_rotTime += fElapsedTime;
		if (_rotTime >= 1.f)
		{
			_rotTime = 1.f;

			Dash();
		}

		if (_isReverseAngle)
		{
			float value = _curRadian - _rotationAngle;
			float angle = CETween::Lerp(0, value, _rotTime);
			transform->SetWorldEulerAngle(0, _curRadian - angle, 0);
		}
		else
		{
			float angle = CETween::Lerp(_curRadian, _rotationAngle, _rotTime);
			transform->SetWorldEulerAngle(0, angle, 0);
		}
		break;

	case State::DASH:
		if (false == _isDash)
			break;
		_time += fElapsedTime;
		if (_time >= 1.f)
		{
			_time = 1.f;
			_boardMgr->SetCheckIn(_nextIndex, true);
			_curIndex = _nextIndex;
			DashEndAnimation();
		}
		if (_curIndex != _nextIndex)
		{
			float x = CETween::Lerp(_curPosition.x, _nextPosition.x, _time);
			float z = CETween::Lerp(_curPosition.z, _nextPosition.z, _time);

			transform->SetWorldPosition(x, 0, z);
		}
		break;
	case State::END:
		Idle();
		break;
	case State::DIE:
		break;
	}
}

void ChessQueen::LateUpdate(float fElapsedTime) noexcept
{

	if (_isDirtyState)
	{
		_isDirtyState = false;
		_state = _dirtyState;

		switch (_state)
		{
		case State::IDLE:
			_animator->Play("ChessQueen.ao|Enemy_PlayRoom_ChessQueen_mh", true);
			break;
		case State::ATTACK:
			_animator->Play("ChessQueen.ao|Enemy_PlayRoom_ChessQueen_Summon"
							, false	
							, std::bind(&ChessQueen::Summon,this));

			_effectPlayer->Play(_audioasset[static_cast<uint>(eAudioType::SUMMON)],1.f);
			break;
		case State::DASH:
			_animator->Play("ChessQueen.ao|Enemy_PlayRoom_ChessQueen_Move_Start"
							, false
							, std::bind(&ChessQueen::DashStart,this));

			_effectPlayer->Play(_audioasset[static_cast<uint>(eAudioType::ATTACK)], 1.f);

			break;
		case State::DIE:
			_animator->Play("ChessQueen.ao|Enemy_PlayRoom_ChessQueen_Death"
							, false
							, std::bind(&ChessQueen::DieEndAnimation, this));
			break;
		}
	}

}

void ChessQueen::Idle(void) noexcept
{
	if (State::IDLE == _state)
		return;

	_waitTime = 0;

	_isDirtyState = true;
	_dirtyState = State::IDLE;

	if (_target == nullptr)
	{
		_dirtyState = State::END;
		return;
	}
	//각도 구하기
	LookAt();

	//방향 정하기
	int dirIndex;

	if (_nextIndex == -1)
		dirIndex = 0;
	else
	{
		dirIndex = static_cast<int>( fabs(_directionAngle) / 45.f);

		if (fabs(fmodf(_directionAngle, 45.f)) >= 22.5f)
			++dirIndex;

		if (_directionAngle < 0)
			dirIndex *= -1;
	}

	_isReverseAngle = false;
	if (dirIndex != 0)
	{
		_curRadian = XMVectorGetY(transform->GetWorldEulerAngle());
		float curAngle = CE_MATH::ToDegree(_curRadian);
		_curRadian = CE_MATH::ToRadian(curAngle);

		_rotationAngle = (dirIndex * 45.f) + curAngle;

		if (_rotationAngle >= 360.f)
		{
			_isReverseAngle = true;
			_rotationAngle -= 360.f;
		}
		else if (_rotationAngle < 0)
		{
			_rotationAngle += 360.f;
		}
		_rotationAngle = CE_MATH::ToRadian(_rotationAngle);
	}
	else
	{
		_curRadian = XMVectorGetY(transform->GetWorldEulerAngle());
		float curAngle = CE_MATH::ToDegree(_curRadian);
		_curRadian = CE_MATH::ToRadian(curAngle);
		_rotationAngle = _curRadian;
	}

	dirIndex += (int)_curDir;
	if (dirIndex >= 8)
		dirIndex -= 8;
	else if (dirIndex < 0)
		dirIndex += 8;

	_curDir = (Direction)dirIndex;
	_rotTime = 0;

}

void ChessQueen::Walk(void) noexcept
{
}

void ChessQueen::Dash(void) noexcept
{
	if (State::DASH== _state)
		return;

	_isDirtyState = true;
	_dirtyState = State::DASH;

	_trigger->SetEnable(true);

	//다음칸 
	SetNextPos();
	
	if (_curIndex == _nextIndex)
		return;

	_time = 0;
}

void ChessQueen::Attack(void) noexcept
{
	if (State::ATTACK == _state)
		return;

	_isDirtyState = true;
	_dirtyState = State::ATTACK;
}

void ChessQueen::GetHit(float damage, int8 attacker) noexcept
{
	if (_isDead) return;

	bool result = _bossMgr->GetHit(damage, attacker);

	if (result)
	{
		if (_colorChanged)
		{
			_colorChanged = false;
			Material::TList& mats = _renderer->GetMaterials();
			for (Material& mat : mats)
				mat.SetValue(ShaderSemantic::Type::BOOLEAN_USE_FORCECOLOR, (void*)&_colorChanged, sizeof(bool));
		}
	}
	else 
	{
		Material::TList& mats = _renderer->GetMaterials();
		bool useForceColor = true;

		for (Material& mat : mats)
			mat.SetValue(ShaderSemantic::Type::BOOLEAN_USE_FORCECOLOR, (void*)&useForceColor, sizeof(bool));

		_colorChanged = true;
		_colorChangeDelta = 0;
	}
}

void ChessQueen::Die(void) noexcept
{
	if (_isDead
		|| State::DIE == _state)
		return;

	_isDirtyState = true;
	_dirtyState = State::DIE;
	_state = State::DIE;

	_trigger->SetEnable(false);
}

void ChessQueen::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	if (false == _isDash)
		return;

	GameObject* obj = other->GetGameObject();
	if (obj->GetTag() == GameObjectTag::PLAYER)
	{
		obj->GetComponent<Actor>()->GetHit(3, 0);
	}
}

void ChessQueen::SetPattern(void) noexcept
{
	int patternNum = CE_MATH::Random(5) + CE_MATH::Random(1, 6);//0~9
	// 기본공격(대쉬) 0 1 2 3 
	if (patternNum <= 3)
	{
		_wait = true;
		Idle();
	}
	// 소환 4 5 6 7 8 9
	else
	{
		_bossMgr->SetSummonPattern();
		Attack();
	}

}

void ChessQueen::LookAt(void) noexcept
{
	XMVECTOR vDir = (_target->GetTransform()->GetWorldPosition() - transform->GetWorldPosition());
	float length = XMVectorGetX(XMVector3Length(vDir));
	if (length < 4)
	{
		_nextIndex = -1;
		return;
	}

	vDir = XMVectorSetY(XMVector3Normalize(vDir), 0);

	XMVECTOR forward = XMVector3Normalize(transform->GetForward());
	forward = XMVectorSetY(forward, 0);

	float dot = XMVectorGetX(XMVector3Dot(vDir, forward));
	float angle = acosf(dot);

	XMVECTOR cross = XMVector3Cross(transform->GetForward(), vDir);
	if (XMVectorGetY(cross) < 0)
		angle *= -1;

	_directionAngle = CE_MATH::ToDegree(angle);
}

void ChessQueen::SetNextPos(void) noexcept
{
	if (_nextIndex == -1)
	{
		_nextIndex = _curIndex;

		_curPosition = transform->GetWorldPosition();
		_nextPosition = _curPosition;
		return;
	}

	int lineX = _curIndex / 8;
	int lineY = _curIndex % 8;

	int index = _curIndex;
	bool end = false;
	switch (_curDir)
	{
	case Direction::BACK_LEFT:
		if (lineY == 0 || lineX == 7)
		{
			_nextIndex = _curIndex;
			break;
		}
		for (int i = lineX + 1; i < 8; ++i)
		{
			index += 7;
			if (index % 8 == 0)
				end = true;
			else if (i == 7)
				end = true;
			
			if (true == _boardMgr->GetCheckIn(index))
			{
				index -= 7;
				break;
			}
			else if (end)
				break;
		}
		_nextIndex = index;
		break;
	case Direction::BACK:
		if (lineX == 7)
		{
			_nextIndex = _curIndex;
			break;
		}
		for (int i = lineX + 1; i < 8; i++)
		{
			index += 8;
			if (i == 7)
				end = true;
			if (true == _boardMgr->GetCheckIn(index))
			{
				index -= 8;
				break;
			}
			else if (end)
				break;
		}
		_nextIndex = index;//56 + lineY;
		break;
	case Direction::BACK_RIGHT:
	{
		//가장 우측 인경우 
		if (lineY == 7 || lineX == 7)
		{
			_nextIndex = _curIndex;
			break;
		}
		int line = lineX + 1;
		for (int i = lineX + 1; i < 8; ++i)
		{
			++line;
			index += 9;
			if ((line * 8) - 1 == index)
				end = true;
			else if (line == 8)
				end = true;
			if (true == _boardMgr->GetCheckIn(index))
			{
				index -= 9;
				break;
			}
			else if (end)
				break;
		}
		_nextIndex = index;
	}
		break;
	case Direction::LEFT:
		if (lineY == 0)
		{
			_nextIndex = _curIndex;
			break;
		}
		for (int i = lineY-1; i >= 0; --i)
		{
			--index;
			if (true == _boardMgr->GetCheckIn(index))
			{
				++index;
				break;
			}
		}
		_nextIndex = index;//lineX * 8;
		break;
	case Direction::RIGHT:
		if (lineY == 7)
		{
			_nextIndex = _curIndex;
			break;
		}
		for (int i = lineY+1; i < 8; ++i)
		{
			++index;
			if (true == _boardMgr->GetCheckIn(index))
			{
				--index;
				break;
			}
		}
		_nextIndex = index;// ((lineX + 1) * 8) - 1;
		break;
	case Direction::FORWARD_LEFT:
		if (lineY == 0 || lineX == 0)
		{
			_nextIndex = _curIndex;
			break;
		}
		for (int i = lineX - 1; i >= 0; --i)
		{
			index -= 9;
			if (index % 8 == 0)
				end = true;
			else if (index / 8 == 0)
				end = true;
		
			if (true == _boardMgr->GetCheckIn(index))
			{
				index += 9;
				break;
			}
			else if (true == end)
				break;
			
		}
		_nextIndex = index;
		break;
	case Direction::FORWARD:
		if(lineX == 0)
		{
			_nextIndex = _curIndex;
			break;
		}
		for (int i = lineX - 1; i >= 0; --i)
		{
			index -= 8;
			if (true == _boardMgr->GetCheckIn(index))
			{
				index += 8;
				break;
			}
		}
		_nextIndex = index;
		break;
	case Direction::FORWARD_RIGHT:
	{
		if (lineY == 7 || lineX == 0)
		{
			_nextIndex = _curIndex;
			break;
		}
		int line = lineX + 1;
		for (int i = lineX - 1; i >= 0; --i)
		{
			--line;
			index -= 7;

			if ((line * 8) - 1 == index)
				end = true;
			else if (line == 1)
				end = true;

			if (true == _boardMgr->GetCheckIn(index))
			{
				index += 7;
				break;
			}
			else if (true == end)
				break;

		}
		_nextIndex = index;
	}
		break;
	}
	if (_nextIndex < 0
		|| _nextIndex > 63)
		_nextIndex = _curIndex;

	_curPosition = transform->GetWorldPosition();
	_nextPosition = _boardMgr->GetPosVec(_nextIndex);

}

void ChessQueen::SetTarget(GameObject* target, bool isHit) noexcept
{
	if (_target == target || isHit)
		return;

	_target = target;
}

void ChessQueen::Summon(void) noexcept
{
	//소환 
	_bossMgr->Summon();
	//
	Idle();
}

void ChessQueen::DashStart(void) noexcept
{
	_boardMgr->SetCheckIn(_curIndex, false);
	_isDash = true;
	_animator->Play("ChessQueen.ao|Enemy_PlayRoom_ChessQueen_Move_mh", true);
}

void ChessQueen::DashEndAnimation(void) noexcept
{
	if (_dashAni)
		return;

	_isDash = false;
	_dashAni = true;
	_animator->Play("ChessQueen.ao|Enemy_PlayRoom_ChessQueen_Move_Exit"
		, false
		, std::bind(&ChessQueen::DashEnd, this));
}

void ChessQueen::DashEnd(void) noexcept
{
	_dashAni = false;
	_trigger->SetEnable(false);
	SetPattern();
}

void ChessQueen::DieEndAnimation(void) noexcept
{
	_renderer->SetEnable(false);
	_rigidbody->SetFreezePositionX(true);
	_rigidbody->SetFreezePositionY(true);
	_rigidbody->SetFreezePositionZ(true);
	_rigidbody->SetFreezeRotationY(true);

	_dieObject->SetActive(true);

	XMVECTOR pos = transform->GetWorldMatrix().r[3];
	_dieRenderer->SetEnable(true);
	_dieTransform->SetWorldPosition(XMVectorGetX(pos), 0, XMVectorGetZ(pos));

	_dieAnimator->Play((uint16)0, false);
	_dieAnimator->SetPlaySpeed(1.f);
}
