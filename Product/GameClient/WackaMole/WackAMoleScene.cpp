#include "pch.h"
#include "WackAMoleScene.h"

#include "FreeLookController.h"
#include "FadeController.h"
#include "Light.h"
#include "PhysicsSystem.h"
#include "ShaderManager.h"
#include "DebugRenderer.h"
/* 미니게임 */
#include "WackamoleBox.h"
#include "WackaMoleMay.h"
#include "WackaMoleCody.h"
#include "WackamoleHole.h"
#include "WackamoleDoor.h"
#include "MiniGameUI.h"


void WackAMoleScene::SetLoadingContents(void) noexcept
{
	AddLoadingAssetDirectory("Asset\\Texture");
	AddLoadingAssetDirectory("Asset\\UI");
	AddLoadingAssetDirectory("Asset\\Font");

	AddLoadingAssetDirectory("Asset\\Models\\Wackamole");
	AddLoadingAssetDirectory("Asset\\Models\\Plank");
	AddLoadingAssetDirectory("Asset\\Models\\Environment\\Environment\\Prop\\Fantasy\\Shed\\Decoration");
	AddLoadingAssetDirectory("Asset\\Audio\\MiniGame");
	

	AddLoadingInfo("Asset\\SceneInfo\\Wackamole.sceneinfo");


	AddLoadingTask([&]() {
		GameObject* cam = GameObject::Instantiate("Camera");
		Camera* mainCamera = Camera::Create();
		cam->AddComponent(mainCamera);
		Camera::SetMainCamera(mainCamera);
		cam->GetTransform()->SetWorldPosition(10.41f, 10.76f, 0);
		cam->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(38.717f), XMConvertToRadians(-90.f), 0);
		mainCamera->SetClearColor(Vector4(0, 0, 0, 0));

		cam->SetTag(GameObjectTag::CAMERA);
		//obj->AddComponent(Light::CreateDirectional());
		Camera::SetMainCamera(cam->GetComponent<Camera>());

		mainCamera->RemovePostEffect(PostEffect::EffectType::BLOOM);
		mainCamera->RemovePostEffect(PostEffect::EffectType::DOF);
		mainCamera->RemovePostEffect(PostEffect::EffectType::BLUR);
		mainCamera->RemovePostEffect(PostEffect::EffectType::FOG);
		mainCamera->AddPostEffect(PostEffect::EffectType::HDR);

		Camera::GetMainCamera()->SetClearOption(Camera::ClearOption::SolidColor);
		});

	AddLoadingTask([&]() {
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

HRESULT WackAMoleScene::Init(void) noexcept
{
	FadeController::This()->StartFadeIn(0.1f);
	PhysicsSystem::Instance()->SetGravity(-9.8f);

	bool use = true;
	ShaderManager::This().GetShader(Shader::Type::VIEWPORT)->SetRawValue("useForceDownSpecular", (void*)&use, sizeof(bool));

	GameObject::FindObjectByName("minigameUI")->GetComponent<MiniGameUI>()->OnMainBanner();

	return S_OK;
}

void WackAMoleScene::FixedUpdate(float fElapsedTime) noexcept
{
	static bool enable = false;

	if (INPUT->GetKeyDown(VK_F2))
	{
		enable = !enable;
		DebugRenderer::This().SetEnable(enable);
	}
}

void WackAMoleScene::Update(float fElapsedTime) noexcept
{
}

void WackAMoleScene::LateUpdate(float fElapsedTime) noexcept
{
}

void WackAMoleScene::Render(void) noexcept
{
}

void WackAMoleScene::LateRender(void) noexcept
{
}

void WackAMoleScene::Release(void) noexcept
{
}