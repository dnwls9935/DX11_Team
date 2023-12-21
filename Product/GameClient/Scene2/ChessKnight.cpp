#include "pch.h"
#include "ChessKnight.h"
#include "GameObject.h"
#include "GameObject.h"
#include "Animator.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "BoneChildObject.h"
#include "ParticleSystem.h"
#include "ChessBoardDataManager.h"
#include "ChessBoardData.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

ChessKnight::ChessKnight(void) noexcept
	: Summons(true)
	, _attackAsset(nullptr)
	, _vanishAsset(nullptr)
	, _spawnAsset(nullptr)
{
	_damage = 4;
}

void ChessKnight::Start(void) noexcept
{
	transform->SetLocalEulerAngle(0, XMConvertToRadians(-90.f - CHESS_SCENE_ANGLE), 0);

	_renderer = ModelRenderer::Create(false);
	gameObject->AddComponent(_renderer);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessKnight\\ChessKnight.fbx"));
	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	_renderer->SetOffsetMatrix(offsetMat);

	_renderer->SetEnable(false);

	_animator = Animator::Create();
	gameObject->AddComponent(_animator);

	_state = Actor::State::END;
	_go = false;

	__super::Start();
	GameObject* particleObject = GameObject::Instantiate("ChessKnightParticle");
	particleObject->GetTransform()->SetParent(transform);

	HierarcyNode* bone = _renderer->GetModel()->GetHierarchyNode("Foot_end");
	BoneChildObject* bho = BoneChildObject::Create(bone, offsetMat);
	particleObject->AddComponent(bho);

	_particle = ParticleSystem::Create(ParticleSystem::Type::MESH, ParticleSystem::Shape::BOX);
	_particle->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Effect\\g_blob01.fbx"));
	_particle->SetStartSize(0.005f);
	_particle->SetEndSize(0.0f);
	_particle->SetLifeTime(0.6f);
	_particle->SetRange(Vector3::one);
	_particle->SetParticlePerSecond(0.05f);
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

	Collider* triggerCollider = Collider::CreateCapsuleCollider(1.0f, 1.0f, XMVectorSet(0, 2.f, 0, 0));
	triggerCollider->SetIsTrigger(true);
	gameObject->AddComponent(triggerCollider);

	/* Break Object */
	GameObject* replace = GameObject::Instantiate("Knight Replace");

	_replaceRenderer = ModelRenderer::Create();
	replace->AddComponent(_replaceRenderer);
	_replaceRenderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Monster\\ChessKnight\\Knight_Attack_Break.fbx"));
	offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(CE_MATH::ToRadian(-90));
	_replaceRenderer->SetOffsetMatrix(offsetMat);

	_replaceAnimator = Animator::Create();
	_replaceAnimator->Play((uint)0, false, std::bind(&ChessKnight::OnDead, this));
	_replaceAnimator->SetPlaySpeed(0.f);
	replace->AddComponent(_replaceAnimator);

	_replaceRenderer->SetEnable(false);

	_attackAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Knight_Attack.ogg");
	_vanishAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Knight_Vanish.ogg");
	_spawnAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\Chessboard\\Play_Characters_Enemies_Chessboard_Piece_Movement_Spawn_Knight.ogg");
}

void ChessKnight::FixedUpdate(float fElapsedTime) noexcept
{
	__super::FixedUpdate(fElapsedTime);
}

void ChessKnight::Update(float fElapsedTime) noexcept
{
	__super::Update(fElapsedTime);
	
	switch (_state)
	{
	case Actor::State::ATTACK:
		if (false == _go)
			return;
		transform->Translate(transform->GetForward() * fElapsedTime * 5.5, false);
		break;
	case Actor::State::JUMP:
		_lerpTime += fElapsedTime;
		_lerpSize = CETween::Lerp(0.f, 1.f, std::min(1.f, _lerpTime), CETween::EaseType::easeLiner);
		transform->SetLocalScale(_lerpSize, _lerpSize, _lerpSize);
		break;
	}
}

void ChessKnight::LateUpdate(float fElapsedTime) noexcept
{
	__super::LateUpdate(fElapsedTime);

	if (true == _isDirtyState)
	{
		_isDirtyState = false;
		_state = _dirtyState;

		int random = 0; 
		std::string str = "";

		switch (_dirtyState)
		{
		case Actor::State::ATTACK:
			random = CE_MATH::Random(4);
			str = "ChessKnight.ao|Enemy_PlayRoom_ChessKnight_AttackMove_var";
			str += std::to_string(++random);

			_animator->Play(str.c_str()
							, false
							, std::bind(&ChessKnight::OnEndAnimation, this)
							, 0.4f
							, std::bind(&ChessKnight::Attack, this));
			_animator->SetPlaySpeed(1.f);

			_movementEffectPlayer->Play(_attackAsset, 0.05f);	
			break;
		case Actor::State::JUMP:
			_animator->Play("ChessKnight.ao|Enemy_PlayRoom_ChessKnight_Summon"
							, false
							, std::bind(&ChessKnight::OnEndAnimation, this));
			_animator->SetPlaySpeed(1.f);
			_go = false;

		/*	_dirtyState = Actor::State::JUMP;
			_state = _dirtyState;
			_isDirtyState = false;*/
			break;
		}
	}
}

void ChessKnight::Attack(void) noexcept
{
	_go = true;
}

void ChessKnight::Die(void) noexcept
{
	if (_state == Actor::State::DIE)
		return;

	__super::Die();
	_go = false;
	_movementEffectPlayer->Play(_vanishAsset, 0.05f);
	/*_isDirtyState = true;
	_dirtyState = Actor::State::IDLE;*/
}

void ChessKnight::Spawn(int index) noexcept
{
	__super::Spawn(index);

	_lerpTime = 0.f;
	_lerpSize = 0.f;
	_finalPointIndex = index - 7;
}

void ChessKnight::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	//이동 중에만 체크할 수 있도록
	if (false == _go)
		return;

	GameObject* obj = other->GetGameObject();

	if (GameObjectTag::OBSTACLE == obj->GetTag())
	{
		_isDead = true;
		_go = false;
		return;
	}
	else if (GameObjectTag::PLAYER == obj->GetTag())
	{
		obj->GetComponent<Actor>()->GetHit(static_cast<float>(_damage), 0);
	}
}

void ChessKnight::OnEndAnimation(void) noexcept
{
	switch (_state)
	{
	case Actor::State::ATTACK:
		_go = false;
		break;
	case Actor::State::JUMP:
		_isDirtyState = true;
		_dirtyState = Actor::State::ATTACK;
		_lerpTime = 0.f;
		_lerpSize = 1.f;
		transform->SetLocalScale(1, 1, 1);
		_particle->Play();
		if (!_isSpawnSound)
		{
			_movementEffectPlayer->Play(_spawnAsset, 0.05f);
			_isSpawnSound = true;
		}
		break;
	}
}