#include "pch.h"
#include "ChessKing.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Animator.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ChessBoss.h"
#include "Actor.h"
#include "ChessBoardDataManager.h"
#include "SpriteRenderer.h"
#include "AudioSource.h"
#include "DungeonCamera.h"
#include "EffectPlayer.h"

ChessKing::ChessKing(ChessBoss* chessBoss, ChessBoardDataManager* boardManager) noexcept
	: MonsterActor(0)
	, _bossMgr(chessBoss)
	, _boardMgr(boardManager)
{
}

void ChessKing::Start(void) noexcept
{
	//시작 위치 지정 
	_curIndex = 4;
	XMVECTOR pos = _boardMgr->GetPosVec(_curIndex);
	transform->SetWorldPosition(pos);
	transform->SetWorldEulerAngle(0, CE_MATH::ToRadian(140.f), 0);

	_curDir = Direction::BACK;
	_boardMgr->SetCheckIn(_curIndex,true);

	/* Model Renderer */
	_renderer = ModelRenderer::Create();
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessKing\\ChessKing.fbx"));
	gameObject->AddComponent(_renderer);

	XMMATRIX offsetMat = XMMatrixScaling(0.015f, 0.015f, 0.015f) * XMMatrixRotationY(CE_MATH::ToRadian(-90.f));
	_renderer->SetOffsetMatrix(offsetMat);

	/* Animator */
	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play("ChessKing.ao|Enemy_PlayRoom_ChessKing_mh", true);

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
	_collider = Collider::CreateCapsuleCollider(1.5f, 2, XMVectorSet(0, 3.7f, 0, 0));//Collider::CreateBoxCollider(1.5f, 2, 1.5f, XMVectorSet(0, 2, 0, 0));
	gameObject->AddComponent(_collider);

	gameObject->SetTag(GameObjectTag::MONSTER);

	_state = Actor::State::END;

	/* Effect */
	GameObject* obj = GameObject::Instantiate("king effect");
	_effectTrans = obj->GetTransform();
	_effectTrans->SetParent(transform);
	_effectTrans->SetLocalPosition(0, 0.1f, 0);
	_effectTrans->SetLocalEulerAngle(CE_MATH::ToRadian(90), 0, 0);
	_effect = SpriteRenderer::Create();
	_effect->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\Orange.dds"));
	_effect->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1.f, 0.6f, 0.3f, 1.f));

	_effect->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_ALPHA, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\T_NoiseRing_01.dds"));
	_effect->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE ,ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\T_NoiseRing_01.dds"));
	_effect->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1.f, 0.6f, 0.3f, 1.f));

	_effect->SetPassType(SpriteRenderer::RenderOption::USEALPHATEXTURE);
	_effect->SetSize(10.f, 10.f);
	obj->AddComponent(_effect);
	obj->SetLayer(GameObjectLayer::TRANSLUCENT);
	
	_effect->SetEnable(false);


	obj = GameObject::Instantiate("King Shock Wave");
	_shockWaveTrans = obj->GetTransform();
	_shockWaveTrans->SetParent(transform);
	_effectTrans->SetLocalEulerAngle(CE_MATH::ToRadian(90), 0, 0);
	_shockWave = SpriteRenderer::Create(true);
	_shockWave->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\bokeh2.dds")); 
	_shockWave->SetPassType(SpriteRenderer::RenderOption::DISTORTION);

	float distortionLV = 0;
	_shockWave->GetMaterial()->SetValue(ShaderSemantic::Type::FLOAT_DISTORTION, &distortionLV, sizeof(float));
	_shockWave->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_DISTORTION, ASSETMANAGER->GetTextureData("Asset\\Texture\\Normal_12.dds"));
	_shockWave->SetSize(0, 0);
	obj->AddComponent(_shockWave);
	obj->SetLayer(GameObjectLayer::TRANSLUCENT);
	_shockWave->SetEnable(true);
	_shockWaveTrans->SetLocalPosition(0, 3.f , 0);


	_dieObject = GameObject::Instantiate("KingDie");
	_dieObject->GetTransform()->SetParent(transform);
	_dieObject->GetTransform()->SetLocalPosition(0, 0, 0);
	_dieObject->GetTransform()->SetLocalEulerAngle(0, XMConvertToRadians(-90.f), 0);
	_dieTransform = GetTransform();
	_dieRenderer = ModelRenderer::Create();
	_dieObject->AddComponent(_dieRenderer);
	_dieRenderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessKing\\King_Dead.fbx"));
	_dieRenderer->SetOffsetMatrix(XMMatrixScaling(0.015f, 0.015f, 0.015f));
	_dieRenderer->SetEnable(false);

	_dieAnimator = Animator::Create();
	_dieAnimator->Play((uint16)0, false);
	_dieAnimator->SetPlaySpeed(0.f);
	_dieObject->AddComponent(_dieAnimator);

	_audioasset[static_cast<uint>(eAudioType::INTROVO)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_VO_DB_Chessboard_BossFight_Effort_Intro_King_020.ogg");
	_audioasset[static_cast<uint>(eAudioType::JUMPVO)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_VO_DB_Chessboard_BossFight_Effort_Jump_King_010.ogg");
	_audioasset[static_cast<uint>(eAudioType::SUMMONVO)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_VO_DB_Chessboard_BossFight_Effort_Summon_King_040.ogg");
	_audioasset[static_cast<uint>(eAudioType::DEADVO)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_VO_DB_Chessboard_BossFight_Effort_Death_King_010.ogg");
	_audioasset[static_cast<uint>(eAudioType::MOVE)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_King_Move.ogg");
	_audioasset[static_cast<uint>(eAudioType::LAND)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_King_Move_Land.ogg");

	_voiceEffectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_voiceEffectPlayer);

	_movementEffectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_movementEffectPlayer);

	_dungeonCam = Camera::GetMainCamera()->GetGameObject()->GetComponent<DungeonCamera>();
}

void ChessKing::Update(float fElapsedTime) noexcept
{
	_shockWaveTrans->SetLocalPosition(0, 3.f, 0);
	/*if (_isDead)
	{
		if (_isFlying)
		{
			transform->Translate(0, -2*fElapsedTime, 0);
			if (XMVectorGetY(transform->GetWorldPosition()) <= 0.05f)
			{
				_isFlying = false;
			}
		}
		return;
	}*/
	__super::Update(fElapsedTime);


	if (_visibleEffect)
	{
		_effectTime += fElapsedTime;
		
		float size = CETween::Lerp(0, 14, _effectTime * 4);
		_effect->SetSize(size, size);
		size = CETween::Lerp(0, 20, _effectTime * 4);
		_shockWave->SetSize(size, size);

		float alpha = CETween::Lerp(0.4f, 0, std::min(1.f, _effectTime * 4.f), CETween::EaseType::easeLiner);
		_effect->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1.f, 0.6f, 0.3f, alpha));
		_effect->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1.f, 0.6f, 0.3f, alpha));


		float y = CETween::Lerp(0.1f, 1.f, _effectTime * 4);
		_effectTrans->SetLocalPosition(0, y, 0);

		if (_effectTime >= 0.25f)
		{
			_effectTime = 0.25f;
			_visibleEffect = false;
			_effect->SetEnable(false);
			_shockWave->SetEnable(false);
			_shockWave->SetSize(0, 0);
		}
	}

	switch (_state)
	{
	case State::IDLE:
		if (_target == nullptr)
			break;

		_rotTime += fElapsedTime;
		if (_rotTime >= 1.f)
		{
			_rotTime = 1.f;

			Jump();
		}

		if (_curRadian != _rotationAngle)
		{
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
		}
		break;
	case State::JUMP:

		_time += fElapsedTime;
		if (_time >= 1.85f)
		{
			_time = 1.85f;
			_curIndex = _nextIndex;
			EndJump();
		}

		if (!_same)
		{
			float y;
			if (_time <= 1.05f)
			{
				float time = std::min(1.f, _time);
				y = CETween::Lerp(0, 3, time);
			}
			else
				y = CETween::Lerp(3, 0, (_time - 1.05f) * 1.25f);

			float x = CETween::Lerp(_curPosition.x, _nextPosition.x, _time *0.54f);
			float z = CETween::Lerp(_curPosition.z, _nextPosition.z, _time * 0.54f);

			transform->SetWorldPosition(x, y, z);
		}
		else
		{
			float y;
			if (_time <= 1.05f)
			{
				float time = std::min(1.f, _time);
				y = CETween::Lerp(0, 3, time);
			}
			else
				y = CETween::Lerp(3, 0, (_time - 1.05f) * 1.25f);

			transform->SetWorldPosition(_curPosition.x, y, _curPosition.z);
		}
		break;
	case State::END:
		Idle();
		break;
	case State::DIE:
		break;
	}
}

void ChessKing::LateUpdate(float fElapsedTime) noexcept
{
	if (_isDirtyState)
	{
		_isDirtyState = false;
		_state = _dirtyState;

		if (_dirtyAnimState != _animState)
			_animState = _dirtyAnimState;
		else
			return;

		switch (_animState)
		{
		case State::IDLE:
			_animator->Play("ChessKing.ao|Enemy_PlayRoom_ChessKing_mh", true);
			break;
		case State::ATTACK:
			_animator->Play("ChessKing.ao|Enemy_PlayRoom_ChessKing_Summon"
				, false
				, std::bind(&ChessKing::EndAttack, this));

			_voiceEffectPlayer->Play(_audioasset[static_cast<uint>(eAudioType::SUMMONVO)],0.025f);


			break;
		case State::JUMP:
			_animator->Play("ChessKing.ao|Enemy_PlayRoom_ChessKing_Jump"
							, false);

			_voiceEffectPlayer->Play(_audioasset[static_cast<uint>(eAudioType::JUMPVO)], 1.f);

			_movementEffectPlayer->Play(_audioasset[static_cast<uint>(eAudioType::MOVE)], 1.f);


			break;
		case State::DIE:
			_animator->Play("ChessKing.ao|Enemy_PlayRoom_ChessKing_Death"
							, false
							, std::bind(&ChessKing::DieEndAnimation, this));

			_voiceEffectPlayer->Play(_audioasset[static_cast<uint>(eAudioType::DEADVO)], 1.f); 
	
			break;
		}
	}
}

void ChessKing::Idle(void) noexcept
{
	if (State::IDLE == _state)
		return;

	_isDirtyState = true;
	_dirtyState = State::IDLE;

	if(_dirtyAnimState != State::ATTACK)
		_dirtyAnimState = State::IDLE;

	if (_target == nullptr)
	{
		_dirtyState = State::END;
		return;
	}

	_rotTime = 0;

	//각도 구하기
	LookAt();
	
	//제자리
	if (_nextIndex == -1)
		return;

	//방향 정하기
	int dirIndex;
	
	if (_directionAngle == 0)
		dirIndex = 0;
	else 
	{
		//가까운 8방향으로 
		dirIndex = static_cast<int>(fabs(_directionAngle) / 45.f);

		if (fabs(fmodf(_directionAngle, 45.f)) >= 22.5f)
			++dirIndex;

		if (_directionAngle < 0)
			dirIndex *= -1;
	}

	_isReverseAngle = false;
	if (dirIndex != 0 && dirIndex != 8)
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
	//현재 보고있는 방향으로 이동할 거임
	else
	{
		if (dirIndex == 8)
			dirIndex = 0;

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
}

void ChessKing::Walk(void) noexcept
{
}

void ChessKing::Jump(void) noexcept
{
	if (State::JUMP == _state)
		return;

	_isDirtyState = true;
	_dirtyState = State::JUMP;

	if (_dirtyAnimState != State::ATTACK)
		_dirtyAnimState = State::JUMP;

	//다음칸 
	SetNextPos();
	_time = 0;

	if (_nextIndex != _curIndex)
	{
		_boardMgr->SetCheckIn(_curIndex, false);
		_boardMgr->SetCheckIn(_nextIndex, true);
	}
}

void ChessKing::Attack(void) noexcept
{
	if (_isAttack)
		return;

	_isAttack = true;
	_isDirtyState = true;
	_dirtyAnimState = State::ATTACK;
}

void ChessKing::GetHit(float damage, int8 attacker) noexcept
{
	if (_isDead) return;

	bool result = _bossMgr->GetHit(damage, attacker);

	//죽음 
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
	//안죽음 
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

void ChessKing::Die(void) noexcept
{
	if (_isDead
		|| State::DIE == _state)
		return;

	_isDirtyState = true;
	_state = State::DIE;
	_dirtyState = State::DIE;
	_dirtyAnimState = State::DIE;

	float y = XMVectorGetY(transform->GetWorldPosition());
	if (y != 0.f)
		_isFlying = true;

}

void ChessKing::LookAt(void) noexcept
{
	XMVECTOR vDir = (_target->GetTransform()->GetWorldPosition() - transform->GetWorldPosition());

	//거리가 가까운지 체크 
	float length = XMVectorGetX(XMVector3Length(vDir));
	if (length <= 4)
	{
		_nextIndex = -1;
		_directionAngle = 0;
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

	//if (_directionAngle >= 360.f)
	//	_directionAngle -= 360.f;
}

void ChessKing::SetNextPos(void) noexcept
{
	_same = false;

	_curPosition = transform->GetWorldPosition();
	if (_nextIndex == -1)
	{
		_same = true;
		_nextIndex = _curIndex;
		_nextPosition = _curPosition;
	}

	int lineX = _curIndex / 8;
	int lineY = _curIndex % 8;

	switch (_curDir)
	{
	case Direction::BACK_LEFT:
		if (lineX == 7 || lineY == 0)
		{
			_nextIndex = _curIndex;
				_same = true;
				break;
		}
		_nextIndex = _curIndex + 7;
		break;
	case Direction::BACK:
		if (lineX == 7)
		{
			_nextIndex = _curIndex;
			_same = true;
			break;
		}
		_nextIndex = _curIndex + 8;
		break;
	case Direction::BACK_RIGHT:
		if (lineX == 7 || lineY == 7)
		{
			_nextIndex = _curIndex;
			_same = true;
			break;
		}
		_nextIndex = _curIndex + 9;
		break;
	case Direction::LEFT:
		if (lineY == 0)
		{
			_nextIndex = _curIndex;
			_same = true;
			break;
		}
		_nextIndex = _curIndex - 1;
		break;
	case Direction::RIGHT:
		if (lineY == 7)
		{
			_nextIndex = _curIndex;
			_same = true;
			break;
		}
		else 
		{
			_nextIndex = _curIndex + 1;
			int line = lineX + 1;
			if ((line * 8) - 1 == _nextIndex)
				_nextIndex = _curIndex;
		}
		break;
	case Direction::FORWARD_LEFT:
		if (lineX == 0 || lineY == 0)
		{
			_nextIndex = _curIndex;
			_same = true;
			break;
		}
		_nextIndex = _curIndex - 9;
		break;
	case Direction::FORWARD:
		if (lineX == 0)
		{
			_nextIndex = _curIndex;
			_same = true;
			break;
		}
		_nextIndex = _curIndex - 8;
		break;
	case Direction::FORWARD_RIGHT:
		if (lineX == 0 || lineY == 7)
		{
			_nextIndex = _curIndex;
			_same = true;
			break;
		}
		_nextIndex = _curIndex - 7;
		break;
	}
	if (_nextIndex < 0
		|| _nextIndex > 63)
	{
		_same = true;
		_nextIndex = _curIndex;
	}
//	_curPosition = transform->GetWorldPosition();

	//갈곳에 무언가 있음
	if (_boardMgr->GetCheckIn(_nextIndex))
	{
		_same = true;
		_nextIndex = _curIndex;
		_nextPosition = _curPosition;
	}
	else
	{
		_nextPosition = _boardMgr->GetPosVec(_nextIndex);
	}
}

void ChessKing::SetTarget(GameObject* target, bool isHit) noexcept
{
	if (_target == target || isHit)
		return;

	_target = target;
}

void ChessKing::EndAttack(void) noexcept
{
	_isAttack = false;
	_dirtyAnimState = State::END;
}

void ChessKing::EndJump(void) noexcept
{
	_visibleEffect = true;
	_effectTime = 0;
	_effect->SetSize(0,0);
	_effect->SetEnable(true);
	_shockWave->SetEnable(true);

	_bossMgr->DistanceDamage(6.f, 3, true);


	_movementEffectPlayer->Play(_audioasset[static_cast<uint>(eAudioType::LAND)],  1.f);
	_dungeonCam->ShakeCamera(0.3f, 0.5f);

	Idle();
}

void ChessKing::DieEndAnimation(void) noexcept
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
