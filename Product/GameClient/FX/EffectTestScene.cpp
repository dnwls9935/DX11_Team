#include "pch.h"
#include "EffectTestScene.h"
#include "FadeController.h"
#include "ModelRenderer.h"
#include "Light.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Animator.h"
#include "TerrainRenderer.h"
#include "Terrain.h"
#include "FollowingCamera.h"
#include "DebugRenderer.h"
#include "TerrainRenderer.h"
#include "ShaderManager.h"
#include "PhysicsSystem.h"
#include "PlayerScreen.h"
#include "PowerSwitch.h"
#include "MeshRenderer.h"
#include "Model.h"
#include "ModelRenderer.h"
#include "FreeLookController.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "Raycast.h"
#include "Camera.h"
#include "SirenLight.h"
#include "PathFollower.h"
#include "SquirrelTurret.h"
#include "SquirrelTurretHolder.h"
#include "ParticleSystemAnimation.h"
#include "ChiefProjectileFX.h"
#include "FightFX.h"
#include "ChiefChargeFX.h"
#include "Skybox.h"


/* 미니게임 */
#include "WackamoleBox.h"
#include "WackaMoleMay.h"
#include "WackaMoleCody.h"
#include "WackamoleHole.h"
#include "WackamoleDoor.h"
#include "MiniGameUI.h"

//ChiefProjectTileFX* cf;
//FightFX* cc;
//ChiefChargeFX* ccc;
void EffectTestScene::SetLoadingContents(void) noexcept
{
	AddLoadingAssetDirectory("Asset\\Models\\Basic");
	AddLoadingAssetDirectory("Asset\\Texture");
	AddLoadingAssetDirectory("Asset\\UI");
	AddLoadingAssetDirectory("Asset\\Models\\MeshCollider");
	AddLoadingAssetDirectory("Asset\\Models\\Wackamole");

	AddLoadingTask([&]() {
		GameObject* cam = GameObject::Instantiate("Camera");
		Camera* mainCamera = Camera::Create();
		cam->AddComponent(mainCamera);
		Camera::SetMainCamera(mainCamera);
		cam->AddComponent(new FreeLookController());
		cam->GetTransform()->SetWorldPosition(0, 0, -1);

		cam->SetTag(GameObjectTag::PLAYER);
		//obj->AddComponent(Light::CreateDirectional());
		Camera::SetMainCamera(cam->GetComponent<Camera>());

		mainCamera->RemovePostEffect(PostEffect::EffectType::BLOOM);
		mainCamera->RemovePostEffect(PostEffect::EffectType::DOF);
		mainCamera->RemovePostEffect(PostEffect::EffectType::BLUR);
		mainCamera->RemovePostEffect(PostEffect::EffectType::FOG);
		mainCamera->AddPostEffect(PostEffect::EffectType::HDR);


		Skybox::This().SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\skybox.dds"));
		Camera::GetMainCamera()->SetClearOption(Camera::ClearOption::SkyBox);
		});

	//AddLoadingTask([&]() {
	//	GameObject* t = GameObject::Instantiate("0");
	//	t->AddComponent(cf = new ChiefProjectTileFX());

	//	t = GameObject::Instantiate("01");
	//	t->AddComponent(cc = new FightFX());

	//	t = GameObject::Instantiate("001");
	//	t->AddComponent(ccc = new ChiefChargeFX());
	//	});


	AddLoadingTask([&]() {
		GameObject* direction = GameObject::Instantiate("DirectionLight");
		direction->AddComponent(Light::CreateDirectional());

		GameObject* wacka = GameObject::Instantiate("WakcaMoleBox");
		wacka->AddComponent(new WackamoleBox());

		GameObject* hole = GameObject::Instantiate("WakcaMoleHoleUp");
		hole->AddComponent(new WackamoleHole(WackamoleHole::POSITION::UP));
		hole->GetTransform()->SetParent(wacka->GetTransform());

		hole = GameObject::Instantiate("WakcaMoleHoleDown");
		hole->AddComponent(new WackamoleHole(WackamoleHole::POSITION::DOWN));
		hole->GetTransform()->SetParent(wacka->GetTransform());

		hole = GameObject::Instantiate("WakcaMoleHoleLeft");
		hole->AddComponent(new WackamoleHole(WackamoleHole::POSITION::LEFT));
		hole->GetTransform()->SetParent(wacka->GetTransform());

		hole = GameObject::Instantiate("WakcaMoleHoleRight");
		hole->AddComponent(new WackamoleHole(WackamoleHole::POSITION::RIGHT));
		hole->GetTransform()->SetParent(wacka->GetTransform());

		GameObject* door = GameObject::Instantiate("WakcaMoleDoor");
		door->AddComponent(new WackamoleDoor());
		door->GetTransform()->SetParent(wacka->GetTransform());

		GameObject* player = GameObject::Instantiate("WakcaMoleMay");
		player->AddComponent(new WackaMoleMay());
		player->GetTransform()->SetParent(wacka->GetTransform());

		player = GameObject::Instantiate("WakcaMoleCody");
		player->AddComponent(new WackaMoleCody());
		player->GetTransform()->SetParent(wacka->GetTransform());



		});

	AddLoadingTask([&]() {
		MiniGameUI::MINIGAMETEXT text{};
		text.maintext = L"코디 때리기";

		GameObject* minigameui = GameObject::Instantiate("minigameUI");
		minigameui->AddComponent(new MiniGameUI(text));
	});
}

HRESULT EffectTestScene::Init(void) noexcept
{
	FadeController::This()->StartFadeIn(0.1f);
	PhysicsSystem::Instance()->SetGravity(-9.8f);

	bool use = true;
	ShaderManager::This().GetShader(Shader::Type::VIEWPORT)->SetRawValue("useForceDownSpecular", (void*)&use, sizeof(bool));

	return S_OK;
}

void EffectTestScene::FixedUpdate(float fElapsedTime) noexcept
{
	static bool enable = false;

	if (INPUT->GetKeyDown(VK_F2))
	{
		enable = !enable;
		DebugRenderer::This().SetEnable(enable);
	}

	if (INPUT->GetKeyDown('1'))
	{
		GameObject::FindObjectByName("minigameUI")->GetComponent<MiniGameUI>()->OnMainBanner();
	}

	//if (INPUT->GetKeyDown('1'))
	//	cf->Charge();
	//if (INPUT->GetKeyDown('2'))
	//	cf->Projectile();
	//if (INPUT->GetKeyDown('3'))
	//	cf->Hit();
	//if (INPUT->GetKeyDown('4'))
	//	cf->DestroyFX();
	/*if (INPUT->GetKeyDown('1'))
			cc->StartFX(false);
	if (INPUT->GetKeyDown('2'))
			cc->StartFX(true);

	if (INPUT->GetKeyDown('3'))
			cf->Charge();
	if (INPUT->GetKeyDown('4'))
			cf->Projectile();

	if (INPUT->GetKeyDown('5'))
			cf->DestroyFX();

	if (INPUT->GetKeyDown('6'))
			ccc->StartFX();*/

}

void EffectTestScene::Update(float fElapsedTime) noexcept
{
			
}

void EffectTestScene::LateUpdate(float fElapsedTime) noexcept
{

}

void EffectTestScene::Render(void) noexcept
{

}

void EffectTestScene::LateRender(void) noexcept
{

}

void EffectTestScene::Release(void) noexcept
{

}
