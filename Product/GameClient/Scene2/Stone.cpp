#include "pch.h"
#include "Stone.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ModelRenderer.h"
#include "SpriteRenderer.h"
#include "Actor.h"
#include "ParticleSystem.h"
#include "Animator.h"
#include "Camera.h"
#include "DungeonCamera.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

Stone::Stone(int turn) noexcept
	: _stoneTurn(turn)
	, _collision(false)
	, _fallTime (0)
{
}

void Stone::Start(void) noexcept
{
	_renderer = ModelRenderer::Create(false);
	//모델링 랜덤
	std::string path;
	int num = rand() % 3;
	switch (num)
	{
	case 0:
		path = "Asset\\Models\\Castle_Dungeon\\Cave_Rock_01_Single_01\\Cave_Rock_01_Single_01.fbx";
		_breakPath = "Asset\\Models\\Castle_Dungeon\\Cave_Rock_01_Single_01\\Cave_Rocks_01_Single_01_Break.fbx";
		break;
	case 1:
		path = "Asset\\Models\\Castle_Dungeon\\Cave_Rock_01_Single_02\\Cave_Rock_01_Single_02.fbx";
		_breakPath = "Asset\\Models\\Castle_Dungeon\\Cave_Rock_01_Single_02\\Cave_Rocks_01_Single_02_Break.fbx";
		break;
	case 2:
		path = "Asset\\Models\\Castle_Dungeon\\Cave_Rock_01_Single_03\\Cave_Rock_01_Single_03.fbx";
		_breakPath= "Asset\\Models\\Castle_Dungeon\\Cave_Rock_01_Single_03\\Cave_Rocks_01_Single_03_Break.fbx";
		break;
	}
	_renderer->SetModel(ASSETMANAGER->GetModelData(path.c_str()));
	XMMATRIX offsetMat = XMMatrixScaling(0.05f, 0.05f, 0.05f);
	_renderer->SetOffsetMatrix(offsetMat);
	gameObject->AddComponent(_renderer);

	Rigidbody* rigid = Rigidbody::Create(false);
	rigid->SetUseGravity(false);
	rigid->SetFreezePositionX(true);
	rigid->SetFreezePositionZ(true);
	gameObject->AddComponent(rigid);

	Collider* collider = Collider::CreateBoxCollider(3.0f, 2.5f, 2.0f);
	collider->SetIsTrigger(true);
	gameObject->AddComponent(collider);

	/* Particle */
	/*GameObject* obj = GameObject::Instantiate();
	_particle = ParticleSystem::Create(ParticleSystem::Type::SPRITE, ParticleSystem::Shape::SPHERE);
	_particle->SetStartSize(5);
	_particle->SetEndSize(1);
	_particle->SetLifeTime(1);
	_particle->SetRange(Vector3::one * 5);
	_particle->SetParticlePerSecond(0.001f);
	_particle->SetVelocity(1.5f);
	_particle->SetStartColor(Vector4(0.5f, 0.3f, 0.3f, 1));
	_particle->SetEndColor(Vector4(0, 0, 0, 0));
	_particle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
		, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\SoftCloud\\SoftCloud_0.dds"));

	obj->AddComponent(_particle);
	obj->SetLayer(GameObjectLayer::TRANSLUCENT);*/

	/* Sprite */
	GameObject* obj = GameObject::Instantiate();
	_shadow = SpriteRenderer::Create(false);
	_shadow->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\black.dds"));
	_shadow->SetPassType(SpriteRenderer::RenderOption::NORMAL_MSAA);
	_shadow->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1,1,1,0.8f));
	_shadow->SetSize(1, 1);
	obj->AddComponent(_shadow);
	obj->SetLayer(GameObjectLayer::TRANSLUCENT);

	XMVECTOR pos = transform->GetWorldPosition();
	obj->GetTransform()->Rotate(CE_MATH::ToRadian(90), 0, 0);
	obj->GetTransform()->SetWorldPosition(XMVectorSetY(pos, 0.1f + (0.1f * _stoneTurn)));

	_startPos = pos;

	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);

	_audioAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\ToyOgre\\Play_World_Playroom_Castle_Events_ToyCharger_Destructible_Stone_Large.ogg");
}

void Stone::Update(float fElapsedTime) noexcept
{
	if (false == _fall)
	{
		_fallTime += fElapsedTime;
		if (_fallTime >= 1.f)
		{
			//바닥에 충돌함 
			_fallTime = 1.f;
			_fall = true;
			Collision();
			_effectPlayer->Play(_audioAsset, 0.5f);
		}
		
		//돌 낙하
		float y = CETween::Lerp(_startPos.y, 0, _fallTime);
		transform->SetWorldPosition(_startPos.x, y, _startPos.z);
		
		////그림자 알파값 조절 
		float size = CETween::Lerp(2.5f, 7, std::max(std::min(1.0f, _fallTime), 0.0f), CETween::EaseType::easeInExpo);
		_shadow->SetSize(size, size);
	}
	else if (_explosion)
	{
		//잔재 낙하 
		transform->Translate(0, -3* fElapsedTime, 0);
		
		_time += fElapsedTime;
		if (_time >= 1.f)
		{
			_explosion = false;
			gameObject->SetDestroy();
		}
	}
}

void Stone::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	if (_collision)
		return;

	GameObject* obj = other->GetGameObject();

	if (obj->GetTag() == GameObjectTag::PLAYER)
	{
		obj->GetComponent<Actor>()->GetHit(3);
	}
}

void Stone::Collision(void) noexcept
{
	_collision = true;

	// 모델링 변경
	XMMATRIX offset = XMMatrixScaling(0.025f, 0.025f, 0.025f);
	_renderer->SetModel(ASSETMANAGER->GetModelData(_breakPath.c_str()));
	_renderer->SetOffsetMatrix(offset);

	Animator* animator = Animator::Create();
	gameObject->AddComponent(animator);

	//깨지는 애니메이션
	animator->Play( (uint16)0
					, false
					, std::bind(&Stone::BrokenStone, this));

	//그림자 오브젝트 삭제 
	GameObject::Destroy(_shadow->GetGameObject());

	Camera::GetMainCamera()->GetGameObject()->GetComponent<DungeonCamera>()->ShakeCamera(1.5f, 0.7f);
}

void Stone::BrokenStone(void) noexcept
{	//깨지는 애니메이션 끝나면 호출 
	_explosion = true;
	_time = 0;
}
