#include "pch.h"
#include "ChessRook.h"
#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "RookFX.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

ChessRook::ChessRook(void) noexcept
	: Summons(true)
{
}

ChessRook::~ChessRook(void) noexcept
{
}

void ChessRook::Start(void) noexcept
{
	_renderer = ModelRenderer::Create(false);
	gameObject->AddComponent(_renderer);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessRook\\ChessRook.fbx"));
	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	_renderer->SetOffsetMatrix(offsetMat);

	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play("ChessRook.ao|Enemy_PlayRoom_ChessRook_Summon"
					, false
					, std::bind(&ChessRook::OnEndAnimation, this));
	_animator->SetPlaySpeed(1.f);

	_dirtyState = Actor::State::JUMP;
	_state = _dirtyState;
	_isDirtyState = false;

	_rookState = ROOK_STATE::ROOK_STATE_END;

	GameObject* fxObject = GameObject::Instantiate("ChessRookFX");
	fxObject->GetTransform()->SetParent(transform);
	_fx = new ChessRookFX();
	fxObject->AddComponent(_fx);
	_fx->SetBoardManager(_board);

	__super::Start();

	/* Break Object */
	GameObject* replace = GameObject::Instantiate("Rook Replace");

	_replaceRenderer = ModelRenderer::Create();
	replace->AddComponent(_replaceRenderer);
	_replaceRenderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessRook\\Rook_Attack_Break.fbx"));
	offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	_replaceRenderer->SetOffsetMatrix(offsetMat);

	_replaceAnimator = Animator::Create();
	_replaceAnimator->Play((uint)0, false, std::bind(&ChessRook::OnDead, this));
	_replaceAnimator->SetPlaySpeed(0.f);
	replace->AddComponent(_replaceAnimator);

	_replaceRenderer->SetEnable(false);

	_attackAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Rook_Attack.ogg");
	_vanishAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Rook_Vanish.ogg");
	_spawnAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Piece_Movement_Spawn_Rook.ogg");
}

void ChessRook::FixedUpdate(float fElapsedTime) noexcept
{
	__super::FixedUpdate(fElapsedTime);
}

void ChessRook::Update(float fElapsedTime) noexcept
{
	__super::Update(fElapsedTime);

	switch (_state)
	{
	case Actor::State::ATTACK:
		Attack(fElapsedTime);
		break;
	case Actor::State::JUMP:
		_lerpTime += fElapsedTime;
		_lerpSize = CETween::Lerp(0.f, 1.f, std::min(1.f, _lerpTime), CETween::EaseType::easeLiner);
		transform->SetLocalScale(_lerpSize, _lerpSize, _lerpSize);
		break;
	}
}

void ChessRook::LateUpdate(float fElapsedTime) noexcept
{
	__super::LateUpdate(fElapsedTime);

	if (_isDirtyState)
	{
		_isDirtyState = false;
		_state = _dirtyState;

		switch (_dirtyState)
		{
		case Actor::State::ATTACK:
			switch (_rookState)
			{
			case ROOK_STATE::ANTICIPATION_IDLE:
				_animator->Play("ChessRook.ao|Enemy_PlayRoom_ChessRook_Anticipation_mh"
					, false
					, std::bind(&ChessRook::OnEndAnimation, this));
				_animator->SetPlaySpeed(1.f);
				break;
			case ROOK_STATE::ATTACK:
				_animator->Play("ChessRook.ao|Enemy_PlayRoom_ChessRook_Slam_Attack_Start"
					, false
					, std::bind(&ChessRook::OnEndAnimation, this)
					, 0.2f
					, std::bind(&ChessRookFX::Explosion, _fx));
				_animator->SetPlaySpeed(1.f);

				_movementEffectPlayer->Play(_attackAsset, 0.45f);

				break;
			}
			break;
		case Actor::State::JUMP:
			_animator->Play("ChessRook.ao|Enemy_PlayRoom_ChessRook_Summon"
				, false
				, std::bind(&ChessRook::OnEndAnimation, this));

			_animator->SetPlaySpeed(1.f);

			_dirtyState = Actor::State::JUMP;
			_state = _dirtyState;
			_isDirtyState = false;
			break;
		}
	}
}

void ChessRook::Die(void) noexcept
{
	if (_state == Actor::State::DIE)
		return;

	__super::Die();
	_fx->DieReset();
	_movementEffectPlayer->Play(_vanishAsset, 0.45f);

	_rookState = ROOK_STATE::ROOK_STATE_END;
}

void ChessRook::Attack(float fElapsedTime) noexcept
{
	switch (_rookState)
	{
	case ROOK_STATE::ANTICIPATION_IDLE:
		_lerpTime += fElapsedTime * 0.5f;
		_lerpSize = CETween::Lerp(0.f, 0.4f, std::min(1.5f, _lerpTime), CETween::EaseType::easeLiner);
		transform->SetLocalScale(1, 1 - _lerpSize, 1);
		break;
	case ROOK_STATE::ATTACK:
		break;
	}
}

void ChessRook::OnEndAnimation(void) noexcept
{
	switch (_state)
	{
	case Actor::State::ATTACK:
		switch (_rookState)
		{
		case ROOK_STATE::ANTICIPATION_IDLE:
			_isDirtyState = true;
			_dirtyState = Actor::State::ATTACK;
			_rookState = ROOK_STATE::ATTACK;

			_lerpTime = 0.f;
			_lerpSize = 1.f;
			transform->SetLocalScale(1, 1, 1);
			break;
		case ROOK_STATE::ATTACK:
			_animator->Pause();
			/*_isDirtyState = true;
			_dirtyState = Actor::State::JUMP;
			_rookState = ROOK_STATE::ANTICIPATION_IDLE;*/
			break;
		}
		break;
	case Actor::State::JUMP:
		_isDirtyState = true;
		_dirtyState = Actor::State::ATTACK;
		_rookState = ROOK_STATE::ANTICIPATION_IDLE;

		_lerpTime = 0.f;
		_lerpSize = 1.f;
		transform->SetLocalScale(1, 1, 1);
		if (!_isSpawnSound)
		{
			_movementEffectPlayer->Play(_spawnAsset, 0.45f);
			_isSpawnSound = true;
		}
		break;
	}
}
