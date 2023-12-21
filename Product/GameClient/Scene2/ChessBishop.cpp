#include "pch.h"
#include "ChessBishop.h"
#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ChessBishopFX.h"
#include "ChessBoardDataManager.h"
#include "BoneChildObject.h"
#include "ParticleSystem.h"
#include "AudioSource.h"
#include "SpriteRenderer.h"
#include "AtlasAnimation.h"
#include "EffectPlayer.h"

ChessBishop::ChessBishop(void) noexcept
	: Summons(true)
	, _attackAsset(nullptr)
	, _vanishAsset(nullptr)
	, _spawnAsset(nullptr)
{
}

void ChessBishop::Start(void) noexcept
{
	transform->SetLocalEulerAngle(0, XMConvertToRadians(-135.f), 0);

	_renderer = ModelRenderer::Create(false);
	gameObject->AddComponent(_renderer);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessBishop\\ChessBishop.fbx"));
	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-135.f));
	_renderer->SetOffsetMatrix(offsetMat);

	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play("ChessBishop.ao|Enemy_PlayRoom_ChessBishop_Summon"
					, false
					, std::bind(&ChessBishop::OnEndAnimation, this));

	_animator->SetPlaySpeed(1.f);

	_dirtyState = Actor::State::JUMP;
	_state = _dirtyState;
	_isDirtyState = false;

	_bishopState = BISHOP_STAT::BISHOP_STATE_END;

	GameObject* fxObject = GameObject::Instantiate("ChessBishopFX");
	fxObject->GetTransform()->SetParent(transform);
	_fx = new ChessBishopFX(transform);
	fxObject->AddComponent(_fx);

	__super::Start();

	GameObject* particleObject = GameObject::Instantiate("BishopHeadParticle");
	particleObject->GetTransform()->SetParent(transform);

	HierarcyNode* bone = _renderer->GetModel()->GetHierarchyNode("Head_end");

	XMMATRIX localOffsetMat = XMMatrixTranslation(0, 1.5f, 0);

	BoneChildObject* bho = BoneChildObject::Create(bone, offsetMat, localOffsetMat);

	particleObject->AddComponent(bho);

	_particle = ParticleSystem::Create(ParticleSystem::Type::MESH, ParticleSystem::Shape::BOX);
	_particle->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Effect\\g_blob01.fbx"));
	_particle->SetStartSize(0.005f);
	_particle->SetEndSize(0.0f);
	_particle->SetLifeTime(0.6f);
	_particle->SetRange(Vector3::one);
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

	/* Break Object */
	GameObject* replace = GameObject::Instantiate("Bishop Replace");

	_replaceRenderer = ModelRenderer::Create();
	replace->AddComponent(_replaceRenderer);
	_replaceRenderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessBishop\\Bishop_Attak_Break.fbx"));
	offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f);// *XMMatrixRotationY(CE_MATH::ToRadian(90));
	_replaceRenderer->SetOffsetMatrix(offsetMat);

	_replaceAnimator = Animator::Create();
	_replaceAnimator->Play((uint)0, false, std::bind(&ChessBishop::OnDead, this));
	_replaceAnimator->SetPlaySpeed(0.f);
	replace->AddComponent(_replaceAnimator);

	_replaceRenderer->SetEnable(false);

	_attackAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Bishop_Attack.ogg");
	_vanishAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Bishop_Vanish.ogg");
	_spawnAsset  = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Piece_Movement_Spawn_Bishop.ogg");


	GameObject* shockWave = GameObject::Instantiate("BishopbShockWave");
	shockWave->GetTransform()->SetParent(transform);
	shockWave->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(90.f), 0, 0);
	shockWave->GetTransform()->SetLocalPosition(0, 1.f, 0);

	_shockWave = SpriteRenderer::Create();
	shockWave->AddComponent(_shockWave);

	_shockWave->SetEnable(false);
	_shockWave->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\mask_8.dds"));
	_shockWave->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_DISTORTION, ASSETMANAGER->GetTextureData("Asset\\Texture\\Normal_12.dds"));
	_shockWave->SetPassType(SpriteRenderer::RenderOption::DISTORTION);

	float distortionLV = 1;
	_shockWave->GetMaterial()->SetValue(ShaderSemantic::Type::FLOAT_DISTORTION, &distortionLV, sizeof(uint));
	_shockWave->SetSize(0, 0);
	shockWave->SetLayer(GameObjectLayer::TRANSLUCENT);



	_shockAtalsObject = GameObject::Instantiate("BishopbShockWave");
	_shockAtalsObject->GetTransform()->SetParent(transform);
	_shockAtalsObject->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(90.f), 0, 0);
	_shockAtalsObject->GetTransform()->SetLocalPosition(0, 0.1f, 0);

	AtlasAnimation::AtlasDesc desc;
	desc._fullPath = "Asset\\Texture\\ShockWave_5.dds";
	desc._emissivePath = "Asset\\Texture\\ShockWave_5.dds";
	desc._billboard = false;
	desc._amount = XMFLOAT2(10, 10);
	desc._color = Vector4(1, 1, 1, 1);
	desc._emissiveColor = Vector4(1, 1, 1, 1);
	desc._lerpSize = XMFLOAT2(15, 15);
	desc._size = XMFLOAT2(15, 15);
	desc._lerpType = CETween::EaseType::easeLiner;
	desc._offsetVector = XMVectorSet(XMConvertToRadians(90.f), 0, 0, 0);
	desc._renderPass = SpriteRenderer::RenderOption::ATLASANIM_ONE;
	desc._duration = 30.f;
	desc._speed = 82.f;
	desc._fadeOut = false;
	
	_shockAtlas = new AtlasAnimation(desc, false);
	_shockAtalsObject->AddComponent(_shockAtlas);
	_shockAtlas->SetEnable(true);
	_shockAtalsObject->SetActive(false);
}

void ChessBishop::FixedUpdate(float fElapsedTime) noexcept
{
	__super::FixedUpdate(fElapsedTime);
}

void ChessBishop::Update(float fElapsedTime) noexcept
{
	__super::Update(fElapsedTime);

	if (_isDead)
		return;

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

void ChessBishop::LateUpdate(float fElapsedTime) noexcept
{
	__super::LateUpdate(fElapsedTime);

	if (_isDirtyState)
	{
		_isDirtyState = false;
		_state = _dirtyState;

		switch (_dirtyState)
		{
		case Actor::State::ATTACK:
			switch (_bishopState)
			{
			case BISHOP_STAT::ANTICIPATION_IDLE:
				_animator->Play("ChessBishop.ao|Enemy_PlayRoom_ChessBishop_Anticipation_mh"
								, false
								, std::bind(&ChessBishop::OnEndAnimation, this));
				_animator->SetPlaySpeed(1.f);
				break;
			case BISHOP_STAT::ATTACK:
				_animator->Play("ChessBishop.ao|Enemy_PlayRoom_ChessBishop_Attack_Start"
								, false
								, std::bind(&ChessBishop::OnEndAnimation, this)
								, 0.2f
								, std::bind(&ChessBishopFX::Explosion, _fx));
				_animator->SetPlaySpeed(1.f);

				_movementEffectPlayer->Play(_attackAsset, 0.45f);
				_shockAtlas->SetEnable(true);
				_shockAtlas->ResetUV();
				break;
			}
			break;
		case Actor::State::JUMP:
			_animator->Play("ChessBishop.ao|Enemy_PlayRoom_ChessBishop_Summon"
							, false
							, std::bind(&ChessBishop::OnEndAnimation, this)
							, 0.45f
							, std::bind(&ChessBishop::OnTriggerAnimation, this));

			_animator->SetPlaySpeed(1.f);

			_dirtyState = Actor::State::JUMP;
			_state = _dirtyState;
			_isDirtyState = false;

			_particle->Play();
			break;
		}
	}
}

void ChessBishop::Die(void) noexcept
{
	if (_state == Actor::State::DIE)
		return;

	__super::Die();
	_fx->StopFX();
	_movementEffectPlayer->Play(_vanishAsset, 0.45f);
	_shockAtlas->SetEnable(false);
	_shockAtalsObject->SetActive(false);
	//_fx->DestroyFX();
}

void ChessBishop::Attack(float fElapsedTime) noexcept
{
	switch (_bishopState)
	{
	case BISHOP_STAT::ANTICIPATION_IDLE:
		_lerpTime += fElapsedTime;
		_lerpSize = CETween::Lerp(0.f, 0.4f, std::min(1.5f, _lerpTime), CETween::EaseType::easeLiner);
		transform->SetLocalScale(1, 1 - _lerpSize, 1);
		break;
	case BISHOP_STAT::ATTACK:
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
		break;
	}
}

void ChessBishop::OnEndAnimation(void) noexcept
{
	switch (_state)
	{
	case Actor::State::ATTACK:
		switch (_bishopState)
		{
		case BISHOP_STAT::ANTICIPATION_IDLE:
			_isDirtyState = true;
			_dirtyState = Actor::State::ATTACK;
			_bishopState = BISHOP_STAT::ATTACK;
			_lerpTime = 0.f;
			_lerpSize = 1.f;
			transform->SetLocalScale(1, 1, 1);
			_isShockWave = true;
			_shockWave->SetEnable(true);
			_shockAtalsObject->SetActive(true);
			break;
		case BISHOP_STAT::ATTACK:
			_isDirtyState = true;
			_dirtyState = Actor::State::JUMP;
			_bishopState = BISHOP_STAT::BISHOP_STATE_END;
			transform->SetLocalScale(0, 0, 0);
			_shockAtlas->ResetUV();
			_shockAtlas->SetEnable(false);
			_shockAtalsObject->SetActive(false);
			__super::Die();
			break;
		}
		break;
	case Actor::State::JUMP:
		_isDirtyState = true;
		_dirtyState = Actor::State::ATTACK;
		_bishopState = BISHOP_STAT::ANTICIPATION_IDLE;

		_lerpTime = 0.f;
		_lerpSize = 1.f;
		transform->SetLocalScale(1, 1, 1);

		if (!_isSpawnSound)
		{
			_movementEffectPlayer->Play(_spawnAsset, 0.45f);
			_isSpawnSound = true;
		}

		_fx->StartFX();
		break;
	}
}

void ChessBishop::OnTriggerAnimation(void) noexcept
{
	switch (_state)
	{
	case Actor::State::ATTACK:
		break;
	case Actor::State::JUMP:
		_particle->Stop();
		break;
	}
}
