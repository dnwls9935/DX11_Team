#include "pch.h"
#include "AcornBulletFX.h"
#include "GameObject.h"
#include "Transform.h"
#include "ParticleSystem.h"
#include "TrailRenderer.h"
#include "ModelRenderer.h"
#include "HierarcyNode.h"

AcornBulletFX::AcornBulletFX(Transform* parent, Model* model) noexcept
	: _parent(parent)
	, _parentModel(model)
{
}

AcornBulletFX::~AcornBulletFX(void) noexcept
{
}

void AcornBulletFX::Start(void) noexcept
{
	//CreateSmokeParticle();
	//CreateTwinkleParticle();
	CreateTopTrail();
	CreateCenterTrail();
	CreateBottomTrail();

	gameObject->SetActive(false);
}

void AcornBulletFX::Update(float fElapsedTime) noexcept
{
}

void AcornBulletFX::DestroyFX(void) noexcept
{
	Transform::TList& childs = const_cast<Transform::TList&>(transform->GetChilds());

	for (int i = 0; i < childs.size(); ++i)
		GameObject::Destroy(childs[i]->GetGameObject());
}

void AcornBulletFX::StartFX(void) noexcept
{
	gameObject->SetActive(true);
	//_twinkleParticle->Play(true);
}

void AcornBulletFX::CreateSmokeParticle()
{
	_smokeParticleObject = GameObject::Instantiate("AcornBulletFX_SmokeParticle");
	_smokeParticleTransform = _smokeParticleObject->GetTransform();
	_smokeParticleTransform->SetParent(_parent);
	_smokeParticleTransform->SetLocalPosition(0, 0, 1.f);

	_smokeParticle = ParticleSystem::Create();
	_smokeParticleObject->AddComponent(_smokeParticle);

	_smokeParticle->SetRange(Vector3::zero);
	_smokeParticle->SetStartColor(Vector4(0.2352941f, 0.1220141f, 0.06055364f, 0.4f));
	_smokeParticle->SetEndColor(Vector4(0.2352941f, 0.1220141f, 0.06055364f, 0.4f));
	_smokeParticle->SetParticlePerSecond(0.1f);
	_smokeParticle->SetVelocity(0);
	_smokeParticle->SetPass(ParticleSystem::PASS::ALPHABLENDING);
	_smokeParticle->SetStartSize(3);
	_smokeParticle->SetEndSize(5);
	_smokeParticle->SetLifeTime(1.f);
	_smokeParticle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
											, ASSETMANAGER->GetTextureData("Asset\\Texture\\UniqueProjectile\\UniqueProjectile_01.dds"));

	_smokeParticleObject->SetLayer(GameObjectLayer::TRANSLUCENT);

	_smokeParticle->Stop();
}

void AcornBulletFX::CreateTwinkleParticle()
{
	_twinkleParticleObject = GameObject::Instantiate("AcornBulletFX_TwinkleParticle");
	_twinkleParticleTransform = _twinkleParticleObject->GetTransform();
	_twinkleParticleTransform->SetParent(_parent);
	_twinkleParticleTransform->SetLocalPosition(0, 0, 0.f);

	_twinkleParticle = ParticleSystem::Create();
	_twinkleParticleObject->AddComponent(_twinkleParticle);

	_twinkleParticle->SetRange(Vector3::back);
	_twinkleParticle->SetStartColor(Vector4(1.f, 0.4232251f, 0.1102941f, 1) * 5.f);
	_twinkleParticle->SetEndColor(Vector4(1.f, 0.4232251f, 0.1102941f, 1) * 2.f);
	_twinkleParticle->SetParticlePerSecond(0.05f);
	_twinkleParticle->SetLifeTime(1.f);
	_twinkleParticle->SetVelocity(0);
	_twinkleParticle->SetPass(ParticleSystem::PASS::ALPHABLENDING);
	_twinkleParticle->SetStartSize(0.3f);
	_twinkleParticle->SetEndSize(0.0f);
	_twinkleParticle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
												, ASSETMANAGER->GetTextureData("Asset\\Texture\\UniqueProjectile\\UniqueProjectile_01.dds"));
	_twinkleParticle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE
												, ASSETMANAGER->GetTextureData("Asset\\Texture\\UniqueProjectile\\UniqueProjectile_01.dds"));

	_twinkleParticle->SetColorLerpType(CETween::EaseType::easeOutExpo);
	_twinkleParticle->SetSizeLerpType(CETween::EaseType::easeLiner);
	_twinkleParticle->SetStartEmissiveColor(Vector4(1.f, 0.4232251f, 0.1102941f, 0.4f) * 5.f);
	_twinkleParticle->SetEndEmissiveColor(Vector4(1.f, 0.4232251f, 0.1102941f, 0.4f) * 2.f);

	_twinkleParticleObject->SetLayer(GameObjectLayer::TRANSLUCENT);

	_twinkleParticle->Stop();
}

void AcornBulletFX::CreateTopTrail()
{
	if (nullptr == _parent)
		return;

	HierarcyNode* trail0 = _parentModel->GetHierarchyNode("Trail_01_0");
	HierarcyNode* trail1 = _parentModel->GetHierarchyNode("Trail_01_1");
	if (nullptr == trail0 || nullptr == trail1)
		return;

	_topTrailObject = GameObject::Instantiate("AcornBulletFX_TopTrailRenderer");
	_topTrailTransform = _topTrailObject->GetTransform();
	_topTrailTransform->SetParent(_parent);
	_topTrailTransform->SetLocalPosition(0, 0, 0.f);

	_topTrail = TrailRenderer::Create();
	_topTrailObject->AddComponent(_topTrail);

	_topTrail->SetLifeTime(0.2f);
	_topTrail->SetSmoothLevel(12);
	_topTrail->SetNode1(trail0);
	_topTrail->SetNode2(trail1);

	_topTrail->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\LaserBeam04.dds"));
	_topTrail->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.378f, 0.1329517f, 0, 1.f));
	_topTrail->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\Noise02.dds"));
	_topTrail->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1.f, 0.4232251f, 0.1102941f, 0.4f));
	_topTrail->SetPassType(TrailRenderer::TrailRenderOption::ALPHABLEND);
	_topTrail->SetActive(true);

	_topTrailObject->SetLayer(GameObjectLayer::TRANSLUCENT);
}

void AcornBulletFX::CreateCenterTrail()
{
	if (nullptr == _parent)
		return;

	HierarcyNode* trail0 = _parentModel->GetHierarchyNode("Trail_03_0");
	HierarcyNode* trail1 = _parentModel->GetHierarchyNode("Trail_03_1");
	if (nullptr == trail0 || nullptr == trail1)
		return;

	_centerTrailObject = GameObject::Instantiate("AcornBulletFX_CenterTrailRenderer");
	_centerTrailTransform = _centerTrailObject->GetTransform();
	_centerTrailTransform->SetParent(_parent);
	_centerTrailTransform->SetLocalPosition(0, 0, 0.f);

	_centerTrail = TrailRenderer::Create();
	_centerTrailObject->AddComponent(_centerTrail);

	_centerTrail->SetLifeTime(0.2f);
	_centerTrail->SetSmoothLevel(12);
	_centerTrail->SetNode1(trail0);
	_centerTrail->SetNode2(trail1);

	_centerTrail->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
											, ASSETMANAGER->GetTextureData("Asset\\Texture\\LaserBeam04.dds"));
	_centerTrail->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.378f, 0.1329517f, 0, 1.f));
	_centerTrail->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\Noise02.dds"));
	_centerTrail->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1.f, 0.4232251f, 0.1102941f, 0.4f));
	_centerTrail->SetPassType(TrailRenderer::TrailRenderOption::ALPHABLEND);
	_centerTrail->SetActive(true);

	_centerTrailObject->SetLayer(GameObjectLayer::TRANSLUCENT);
}

void AcornBulletFX::CreateBottomTrail()
{
	if (nullptr == _parent)
		return;

	HierarcyNode* trail0 = _parentModel->GetHierarchyNode("Trail_02_0");
	HierarcyNode* trail1 = _parentModel->GetHierarchyNode("Trail_02_1");
	if (nullptr == trail0 || nullptr == trail1)
		return;

	_botTrailObject = GameObject::Instantiate("AcornBulletFX_BotTrailRenderer");
	_botTrailTransform = _botTrailObject->GetTransform();
	_botTrailTransform->SetParent(_parent);
	_botTrailTransform->SetLocalPosition(0, 0, 0.f);

	_botTrail = TrailRenderer::Create();
	_botTrailObject->AddComponent(_botTrail);

	_botTrail->SetLifeTime(0.2f);
	_botTrail->SetSmoothLevel(12);
	_botTrail->SetNode1(trail0);
	_botTrail->SetNode2(trail1);

	_botTrail->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\LaserBeam04.dds"));
	_botTrail->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.378f, 0.1329517f, 0, 1.f));
	_botTrail->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\Noise02.dds"));
	_botTrail->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1.f, 0.4232251f, 0.1102941f, 0.4f));
	_botTrail->SetPassType(TrailRenderer::TrailRenderOption::ALPHABLEND);
	_botTrail->SetActive(true);

	_botTrailObject->SetLayer(GameObjectLayer::TRANSLUCENT);
}
