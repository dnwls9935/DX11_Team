#include "pch.h"
#include "ChessBishopFX.h"

#include "GameObject.h"
#include "Transform.h"
#include "ParticleSystem.h"
#include "SpriteRenderer.h"
#include "SpriteAnimator.h"
#include "SpriteAnimation.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Projectile.h"
#include "FlyParticle.h"


ChessBishopFX::ChessBishopFX(Transform* _bishop) noexcept
	: _parent(_bishop)
{
}

ChessBishopFX::~ChessBishopFX(void) noexcept
{
}

void ChessBishopFX::Start(void) noexcept
{
	std::vector<Texture*>	texture;
	std::vector<float>		frames;
	for (int i = 0; i < 4; i++)
	{
		_noiseRenderer[i] = SpriteRenderer::Create(false);

		_noiseObject[i] = GameObject::Instantiate("ChessBishopFXNoiseObject");

		_noiseObject[i]->GetTransform()->SetParent(_parent);

		_noiseObject[i]->AddComponent(_noiseRenderer[i]);
		_noiseObject[i]->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(90.f), XMConvertToRadians(((i * 90.f)) - CHESS_SCENE_ANGLE), 0);
		_noiseObject[i]->GetTransform()->SetLocalPosition(XMVectorSet(0, 0, 0, 1));

		_noiseRenderer[i]->SetBillboard(false);
		_noiseRenderer[i]->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\Electric\\Bolt0.dds"));
		_noiseRenderer[i]->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1, 0.75f, 0.79f, 0));
		_noiseRenderer[i]->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\Electric\\Bolt0.dds"));
		_noiseRenderer[i]->SetPassType(SpriteRenderer::RenderOption::DEM);
		_noiseRenderer[i]->SetSize(5, 2.f);
		_noiseRenderer[i]->SetPivotX(1.f);

		float f = 1.f;
		_noiseRenderer[i]->GetMaterial()->GetShader().SetRawValue("sampling", &f, sizeof(float));

		_noiseAnimator[i] = SpriteAnimator::Create(false);
		for (int j = 0; j < 60; j++)
		{
			std::string str = "Asset\\Texture\\Dungeon\\Electric\\Bolt" + std::to_string(j) + ".dds";
			texture.push_back(ASSETMANAGER->GetTextureData(str.c_str()));
			frames.push_back(0.05f);
		}
		SpriteAnimation* ani = new SpriteAnimation(frames, texture, true);
		_noiseAnimator[i]->InsertAnimation("Bolt", ani);

		_noiseObject[i]->AddComponent(_noiseAnimator[i]);

		_noiseObject[i]->SetLayer(GameObjectLayer::TRANSLUCENT);
		_noiseObject[i]->SetActive(false);


		frames.clear();
		texture.clear();
	}

	for (int i = 0; i < 4; i++)
	{
		_flyParticle[i] = ParticleSystem::Create(ParticleSystem::Type::MESH, ParticleSystem::Shape::BOX);

		_flyObject[i] = GameObject::Instantiate("ChessBishopFXFlyObject");

		_flyObject[i]->GetTransform()->SetLocalEulerAngle(/*XMConvertToRadians(90.f)*/0, XMConvertToRadians(45.f + (i * 90.f) - CHESS_SCENE_ANGLE), 0);
		_flyObject[i]->GetTransform()->SetLocalPosition(XMVectorSet(0, 0, 1.f, 1));
		_flyObject[i]->SetActive(false);
		                                   
		_flyParticle[i]->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Effect\\g_blob01.fbx"));
		_flyParticle[i]->SetStartSize(0.01f);
		_flyParticle[i]->SetEndSize(0.0f);
		_flyParticle[i]->SetLifeTime(1.f);
		_flyParticle[i]->SetRange(Vector3::one);
		_flyParticle[i]->SetParticlePerSecond(0.01f);
		_flyParticle[i]->SetVelocity(2.f);
		_flyParticle[i]->SetColorLerpType(CETween::EaseType::easeLiner);
		_flyParticle[i]->SetStartColor(Vector4(1, 1, 1, 1));
		_flyParticle[i]->SetEndColor(Vector4(1, 1, 1, 1));
		_flyParticle[i]->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\Noise34.dds"));
		_flyParticle[i]->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0, 0, 0, 1));
		_flyParticle[i]->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Color_Ramp_07.dds"));
		_flyParticle[i]->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(0, 0, 0, 0));
		_flyParticle[i]->SetStartEmissiveColor(Vector4(0, 0, 0, 1));
		_flyParticle[i]->SetStartEmissiveColor(Vector4(0, 0, 0, 0));

		_flyObject[i]->AddComponent(_flyParticle[i]);
		_flyObject[i]->SetLayer(GameObjectLayer::TRANSLUCENT);
		_flyObject[i]->SetTag(GameObjectTag::EFFECT);

		FlyParticle* fly = new FlyParticle(3.f);
		_flyObject[i]->AddComponent(fly);
	}

	_lerpTime = 0.f;
	_start = false;
	_explosion = false;
}

void ChessBishopFX::Update(float fElapsedTime) noexcept
{
	if(_hasParent)
	{
		gameObject->GetTransform()->SetParent(nullptr);
		_hasParent = false;
	}
	if(true == _explosion)
	{
		for (int i = 0; i < 4; i++)
		{
			XMVECTOR dir = XMVector3Normalize(_flyObject[i]->GetTransform()->GetForward());
			_flyObject[i]->GetTransform()->Translate(dir * fElapsedTime * 20.f);
		}
	}
}

void ChessBishopFX::LateUpdate(float fElapsedTime) noexcept
{
}

void ChessBishopFX::StartFX(void) noexcept
{
	for (int i = 0; i < 4; i++)
	{
		_noiseObject[i]->SetActive(true);
		_noiseAnimator[i]->Play("Bolt");
	}

	_start = true;
}

void ChessBishopFX::StopFX(void) noexcept
{
	if (false == _start)
		return;

	for (int i = 0; i < 4; i++)
	{
		_noiseAnimator[i]->Stop();
		_noiseObject[i]->SetActive(false);
	}
	_start = false;
}


void ChessBishopFX::Explosion(void) noexcept
{
	// _start가 false면 실행X(죽었거나 이전이펙트가 끝나지않았음)
	if (false == _start)
		return;

	_start = false;
	for (int i = 0; i < 4; i++)
	{
		_noiseObject[i]->SetActive(false);
		_noiseObject[i]->GetTransform()->SetLocalScale(XMVectorSet(1, 1, 1, 1));
		_noiseRenderer[i]->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1, 1, 1, 1));

		_flyObject[i]->GetComponent<FlyParticle>()->StartFX();
		_flyObject[i]->GetComponent<FlyParticle>()->ClearForce();

		XMVECTOR position = _parent->GetWorldMatrix().r[3];
		XMVECTOR dir = XMVector3Normalize(_flyObject[i]->GetTransform()->GetForward());

		position += (dir * 1.5f);

		_flyObject[i]->GetTransform()->SetWorldPosition(XMVectorGetX(position), 0.3f, XMVectorGetZ(position));
		_flyParticle[i]->Play(true);
	}
	_lerpTime = 0.f;
	_explosion = true;
}

void ChessBishopFX::DestroyFX(void) noexcept
{
	for (int i = 0; i < 4; i++)
		_flyObject[i]->GetComponent<FlyParticle>()->DestroyFX();

	gameObject->SetDestroy();
}