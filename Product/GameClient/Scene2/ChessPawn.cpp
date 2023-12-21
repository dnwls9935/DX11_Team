#include "pch.h"
#include "ChessPawn.h"
#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "BoneChildObject.h"
#include "HierarcyNode.h"
#include "ParticleSystem.h"
#include "ChessPawnFX.h"
#include "ChessBoardDataManager.h"
#include "Collider.h"
#include "AudioSource.h"
#include "SpriteRenderer.h"
#include "EffectPlayer.h"

ChessPawn::ChessPawn(void) noexcept
	: Summons(true)
	, _attackAsset(nullptr)
	, _vanishAsset(nullptr)
	, _spawnAsset(nullptr)

{
	_damage = 4;
}

void ChessPawn::Start(void) noexcept
{
	_renderer = ModelRenderer::Create(false);
	gameObject->AddComponent(_renderer);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessPawn\\ChessPawn.fbx"));
	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	_renderer->SetOffsetMatrix(offsetMat);

	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play("ChessPawn.ao|Enemy_PlayRoom_ChessPawn_Summon"
					, false
					, std::bind(&ChessPawn::OnEndAnimation, this));

	_animator->SetPlaySpeed(1.f);

	_dirtyState = Actor::State::JUMP;
	_state = _dirtyState;
	_isDirtyState = false;

	_pawnState = PAWN_STATE::PAWN_STATE_END;

	__super::Start();

	GameObject* particleObject = GameObject::Instantiate("PawnHeadParticle");
	particleObject->GetTransform()->SetParent(transform);

	HierarcyNode* bone = _renderer->GetModel()->GetHierarchyNode("Head_end");

	XMMATRIX localOffsetMat = XMMatrixTranslation(0, 1.5f, 0);

	BoneChildObject* bho = BoneChildObject::Create(bone, offsetMat, localOffsetMat);

	particleObject->AddComponent(bho);

	_particle = ParticleSystem::Create(ParticleSystem::Type::MESH, ParticleSystem::Shape::BOX);
	_particle->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Effect\\g_blob01.fbx"));
	_particle->SetStartSize(0.005f);
	_particle->SetEndSize(0.0f);
	_particle->SetLifeTime(1.f);
	_particle->SetRange(Vector3::one * 2.f);
	_particle->SetParticlePerSecond(0.1f);
	_particle->SetVelocity(2.f);
	_particle->SetColorLerpType(CETween::EaseType::easeLiner);
	_particle->SetStartColor(Vector4(1, 1, 1, 1));
	_particle->SetEndColor(Vector4(1, 1, 1, 1));
	_particle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\Noise34.dds"));
	_particle->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0, 0, 0, 1));
	_particle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Color_Ramp_07.dds"));
	_particle->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(0, 0, 0, 0));
	_particle->SetStartEmissiveColor(Vector4(0, 0, 0, 0));
	_particle->SetStartEmissiveColor(Vector4(0, 0, 0, 0));

	particleObject->AddComponent(_particle);
	particleObject->SetLayer(GameObjectLayer::TRANSLUCENT);
	particleObject->SetTag(GameObjectTag::EFFECT);

	GameObject* fxObject = GameObject::Instantiate("PawnFXObject");
	_fx = new ChessPawnFX(transform);
	fxObject->AddComponent(_fx);

	_triggerCollider = Collider::CreateBoxCollider(5, 5, 5);
	_triggerCollider->SetIsTrigger(true);
	gameObject->AddComponent(_triggerCollider);
	_triggerCollider->SetEnable(false);

	/* Break Object */
	GameObject* replace = GameObject::Instantiate("Bishop Replace");

	_replaceRenderer = ModelRenderer::Create();
	replace->AddComponent(_replaceRenderer);
	_replaceRenderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessPawn\\Pawn_Idle_Break.fbx"));
	offsetMat = XMMatrixScaling(0.01f,0.01f,0.01f);// *XMMatrixRotationY(CE_MATH::ToRadian(90));
	_replaceRenderer->SetOffsetMatrix(offsetMat);

	_replaceAnimator = Animator::Create();
	_replaceAnimator->Play((uint)0, false, std::bind(&ChessPawn::OnDead, this));
	_replaceAnimator->SetPlaySpeed(0.f);
	replace->AddComponent(_replaceAnimator);

	_replaceRenderer->SetEnable(false);

	_attackAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Pawn_Attack.ogg");
	_vanishAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Pawn_Vanish.ogg");
	_spawnAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Piece_Movement_Spawn_Pawn.ogg");


	GameObject* shockWave = GameObject::Instantiate("PawnbShockWave");
	shockWave->GetTransform()->SetParent(transform);
	shockWave->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(90.f), 0, 0);
	shockWave->GetTransform()->SetLocalPosition(0, 1.f, 0);

	_shockWave = SpriteRenderer::Create();
	shockWave->AddComponent(_shockWave);

	_shockWave->SetEnable(false);
	_shockWave->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\bokeh2.dds"));
	_shockWave->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_DISTORTION, ASSETMANAGER->GetTextureData("Asset\\Texture\\Normal_12.dds"));
	_shockWave->SetPassType(SpriteRenderer::RenderOption::DISTORTION);

	float distortionLV = 1;
	_shockWave->GetMaterial()->SetValue(ShaderSemantic::Type::FLOAT_DISTORTION, &distortionLV, sizeof(uint));
	_shockWave->SetSize(0, 0);
	shockWave->SetLayer(GameObjectLayer::TRANSLUCENT);


	GameObject* shockWavePilar = GameObject::Instantiate("PawnShockWavePilar");
	shockWavePilar->GetTransform()->SetParent(transform);
	shockWavePilar->GetTransform()->SetLocalPosition(0, 1.f, 0);

	_shockWavePilar = SpriteRenderer::Create();
	shockWavePilar->AddComponent(_shockWavePilar);

	_shockWavePilar->SetEnable(false);
	_shockWavePilar->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Particle_01.dds"));
	_shockWavePilar->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0, 0.2f, 1.f, 0.4f));
	_shockWavePilar->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Particle_01.dds"));
	_shockWavePilar->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(0, 0.2f, 1.f, 0.4f));
	_shockWavePilar->SetPassType(SpriteRenderer::RenderOption::EFFECTBLEND_MSAA);

	_shockWavePilar->SetSize(0, 0);

	shockWavePilar->SetLayer(GameObjectLayer::TRANSLUCENT);

	_isShockWave = false;
	_shockWaveTime = 0.f;

	_isShockWavePilar = false;
	_shockwavePilarTime = 0.f;

}

void ChessPawn::FixedUpdate(float fElapsedTime) noexcept
{
	__super::FixedUpdate(fElapsedTime);
}

void ChessPawn::Update(float fElapsedTime) noexcept
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

void ChessPawn::LateUpdate(float fElapsedTime) noexcept
{
	__super::LateUpdate(fElapsedTime);

	if (_isDirtyState)
	{
		_isDirtyState = false;
		_state = _dirtyState;

		switch (_dirtyState)
		{
		case Actor::State::ATTACK:
			switch (_pawnState)
			{
			case PAWN_STATE::ANTICIPATION_IDLE:
				_animator->Play("ChessPawn.ao|Enemy_PlayRoom_ChessPawn_Anticipation_mh"
								, false
								, std::bind(&ChessPawn::OnEndAnimation, this));
				_animator->SetPlaySpeed(1.f);

				_fx->StartFX();
				break;
			case PAWN_STATE::ATTACK:
				_animator->Play("ChessPawn.ao|Enemy_PlayRoom_ChessPawn_Attack_Start"
								, false
								, std::bind(&ChessPawn::OnEndAnimation, this)
								, 0.1f
								, std::bind(&ChessPawn::OnTriggerAnimation, this));
				_animator->SetPlaySpeed(1.f);

				_movementEffectPlayer->Play(_attackAsset, 1.f);

				break;
			}
			break;
		case Actor::State::JUMP:
			_animator->Play("ChessPawn.ao|Enemy_PlayRoom_ChessPawn_Summon"
							, false
							, std::bind(&ChessPawn::OnEndAnimation, this)
							, 0.8f
							, std::bind(&ChessPawn::OnTriggerAnimation, this));

			_animator->SetPlaySpeed(1.f);

			

			_dirtyState = Actor::State::JUMP;
			_state = _dirtyState;
			_isDirtyState = false;
			_particle->Play();
			break;
		}
	}
}

void ChessPawn::Attack(float fElapsedTime) noexcept
{
	switch (_pawnState)
	{
	case PAWN_STATE::ANTICIPATION_IDLE:
		_lerpTime += fElapsedTime * 0.5f;
		_lerpSize = CETween::Lerp(0.f, 0.4f, std::min(1.5f, _lerpTime), CETween::EaseType::easeLiner);
		transform->SetLocalScale(1, 1 - _lerpSize, 1);
		break;
	case PAWN_STATE::ATTACK:

		if (true == _isShockWave)
		{
			_shockWaveTime += fElapsedTime * 2.f;

			float size = CETween::Lerp(0, 20.f, std::min(0.4f, _shockWaveTime), CETween::EaseType::easeOutBack);

			_shockWave->SetSize(size * 2.f, size);

			if (_shockWaveTime >= 0.4f)
			{
				_shockWaveTime = 0.f;
				_isShockWave = false;
				_shockWave->SetSize(0, 0);
				_shockWave->SetEnable(false);
			}
		}

		_shockWavePilar->SetPivotY(0);

		if (true == _isShockWavePilar)
		{
			_shockwavePilarTime += fElapsedTime * 2.f;

			float size = CETween::Lerp(0, 35, std::min(1.f, _shockwavePilarTime), CETween::EaseType::easeLiner);

			_shockWavePilar->SetSize(1.5f, size);

			if (_shockwavePilarTime >= 1.f)
			{
				_shockwavePilarTime = 0.f;
				_isShockWavePilar = false;
				_shockWavePilar->SetSize(0, 0);
				_shockWavePilar->SetEnable(false);
			}

		}

		break;
	}
}

void ChessPawn::Die(void) noexcept
{
	if (_state == Actor::State::DIE)
		return;

	__super::Die();
	_movementEffectPlayer->Play(_vanishAsset, 1.f);
	_pawnState = PAWN_STATE::PAWN_STATE_END;
	_fx->StopFX();
}

void ChessPawn::SearchTile(void) noexcept
{
	_board->PlayFX(_index, ChessBoardDataManager::cardinalPoint::EIGHT);
}

void ChessPawn::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	//GameObject* obj = other->GetGameObject();

	//if (GameObjectTag::PLAYER == obj->GetTag())
	//{
	//	obj->GetComponent<Actor>()->GetHit(_damage);
	//}
}

void ChessPawn::OnEndAnimation(void) noexcept
{
	switch (_state)
	{
	case Actor::State::ATTACK:
		switch (_pawnState)
		{
		case PAWN_STATE::ANTICIPATION_IDLE:
			_isDirtyState = true;
			_dirtyState = Actor::State::ATTACK;
			_pawnState = PAWN_STATE::ATTACK;
			_lerpTime = 0.f;
			_lerpSize = 1.f;
			transform->SetLocalScale(1, 1, 1);
			_fx->StopFX();
			SearchTile();
			/* ¿Ö°î ÀÌÆÑÆ® Ãß°¡ */
			_isShockWave = true;
			_shockWave->SetEnable(true);
			_isShockWavePilar = true;
			_shockWavePilar->SetEnable(true);
			break;
		case PAWN_STATE::ATTACK:
			_pawnState = PAWN_STATE::PAWN_STATE_END;
			transform->SetLocalScale(0, 0, 0);
			Die();
			break;
		}
		break;
	case Actor::State::JUMP:
		_isDirtyState = true;
		_dirtyState = Actor::State::ATTACK;
		_pawnState = PAWN_STATE::ANTICIPATION_IDLE;

		_lerpTime = 0.f;
		_lerpSize = 1.f;
		transform->SetLocalScale(1, 1, 1);

		if (!_isSpawnSound)
		{
			_movementEffectPlayer->Play(_spawnAsset, 1.f);
			_isSpawnSound = true;
		}
		break;
	}
}

void ChessPawn::OnTriggerAnimation(void) noexcept
{
	switch (_state)
	{
	case Actor::State::ATTACK:
		switch (_pawnState)
		{
		case PAWN_STATE::ATTACK:
		//	_triggerCollider->SetEnable(false);
			break;
		}
		break;
	case Actor::State::JUMP:
		_particle->Stop();
		break;
	}
}
