#include "pch.h"
#include "BaseballScene.h"
#include "Transform.h"
#include "FadeController.h"
#include "DebugRenderer.h"
#include "PhysicsSystem.h"
#include "ShaderManager.h"

#include "Light.h"
#include "Camera.h"
#include "BaseBallUI.h"

#include "BaseballCody.h"
#include "BaseballMay.h"

#include "BaseballBall.h"
#include "BaseballLever.h"
#include "BaseballPlayer.h"

void BaseballScene::SetLoadingContents(void) noexcept
{
	AddLoadingAssetDirectory("Asset\\Models\\BattingTeam");
	AddLoadingAssetDirectory("Asset\\Models\\Space\\pillow");
	AddLoadingAssetDirectory("Asset\\Texture");
	AddLoadingAssetDirectory("Asset\\UI");
	AddLoadingAssetDirectory("Asset\\Audio\\MiniGame");

	AddLoadingInfo("Asset\\SceneInfo\\MiniGame_Baseball.sceneinfo");

	AddLoadingTask([&]() {
		GameObject* ui = GameObject::Instantiate("BaseballUI");
		ui->AddComponent(new BaseBallUI());
		});


	AddLoadingTask([&]() {
		GameObject* obj = GameObject::FindLoadingObjectByName("ToyBody01");
		obj->GetTransform()->SetLocalEulerAngle(CE_MATH::ToRadian(-90), 0, CE_MATH::ToRadian(150));
		BaseballPlayer* baseballplayer = new BaseballPlayer(true);
		obj->AddComponent(baseballplayer);


		obj = GameObject::FindLoadingObjectByName("ToyBody02");
		obj->GetTransform()->SetLocalEulerAngle(CE_MATH::ToRadian(-90), 0, CE_MATH::ToRadian(-150));
		baseballplayer = new BaseballPlayer(false);
		obj->AddComponent(baseballplayer);
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::FindLoadingObjectByName("Toy02Crank");
		obj->GetTransform()->SetLocalEulerAngle(CE_MATH::ToRadian(-80.449), 0, CE_MATH::ToRadian(180));
		obj->AddComponent(new BaseballLever(true));

		obj = GameObject::FindLoadingObjectByName("Toy01Crank");
		obj->GetTransform()->SetLocalEulerAngle(CE_MATH::ToRadian(-80.449), 0, CE_MATH::ToRadian(180));
		obj->AddComponent(new BaseballLever(false));
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::FindLoadingObjectByName("Player02");
		obj->GetTransform()->SetLocalEulerAngle(CE_MATH::ToRadian(-90), 0, 0);
		obj->AddComponent(new BaseballBall());
		obj->GetComponent<Rigidbody>()->SetFreezeRotationZ(false);
		obj->GetComponent<Rigidbody>()->SetFreezeRotationX(true);
		obj->GetComponent<Rigidbody>()->SetFreezeRotationY(true);
		obj->GetComponent<Rigidbody>()->SetFreezePositionX(true);
		obj->GetComponent<Rigidbody>()->SetFreezePositionY(true);
		obj->GetComponent<Rigidbody>()->SetFreezePositionZ(true);
		obj->GetComponent<Rigidbody>()->SetAngularDrag(0.01f);
		obj->GetComponent<Rigidbody>()->SetDrag(0.01f);

		obj = GameObject::FindLoadingObjectByName("Player01");
		obj->GetTransform()->SetLocalEulerAngle(CE_MATH::ToRadian(-90), 0, 0);
		obj->AddComponent(new BaseballBall());
		obj->GetComponent<Rigidbody>()->SetFreezeRotationZ(false);
		obj->GetComponent<Rigidbody>()->SetFreezeRotationX(true);
		obj->GetComponent<Rigidbody>()->SetFreezeRotationY(true);
		obj->GetComponent<Rigidbody>()->SetFreezePositionX(true);
		obj->GetComponent<Rigidbody>()->SetFreezePositionY(true);
		obj->GetComponent<Rigidbody>()->SetFreezePositionZ(true);
		obj->GetComponent<Rigidbody>()->SetAngularDrag(0.01f);
		obj->GetComponent<Rigidbody>()->SetDrag(0.01f);
		});

	AddLoadingTask([&]() {

		GameObject* may = GameObject::Instantiate("May");
		_may = new BaseballMay(false);
		may->AddComponent(_may);
		may->GetTransform()->SetWorldPosition(2.509f, 2.432, -12.468f);


		GameObject* cody = GameObject::Instantiate("Cody");

		_cody = new BaseballCody(true);
		cody->AddComponent(_cody);
		cody->GetTransform()->SetWorldPosition(7.291f, 2.514, -12.517f);
		});


}

HRESULT BaseballScene::Init(void) noexcept
{
	FadeController::This()->StartFadeIn(0.1f);
	PhysicsSystem::Instance()->SetGravity(-9.8f * 5);


	Camera* camera = Camera::Create();
	camera->SetClearOption(Camera::ClearOption::SolidColor);
	camera->SetClearColor(Vector4(0, 0, 0, 0));
	camera->AddPostEffect(PostEffect::EffectType::HDR);
	camera->AddPostEffect(PostEffect::EffectType::DOF);
	camera->AddPostEffect(PostEffect::EffectType::FOG);


	_save = GameObject::Instantiate();
	_save->AddComponent(camera);
	_save->GetTransform()->SetLocalEulerAngle(CE_MATH::ToRadian(23), 0, 0);
	_save->GetTransform()->SetWorldPosition(5.3, 14, -25);
	bool use = true;
	ShaderManager::This().GetShader(Shader::Type::VIEWPORT)->SetRawValue("useForceDownSpecular", (void*)&use, sizeof(bool));
	GameObject::FindObjectByName("BaseballUI")->GetComponent<BaseBallUI>()->OnMainBanner();

	return S_OK;
}

void BaseballScene::FixedUpdate(float fElapsedTime) noexcept
{
}

void BaseballScene::Update(float fElapsedTime) noexcept
{
	static bool enable = false;
	if (INPUT->GetKeyDown(VK_F2))
	{
		enable = !enable;
		DebugRenderer::This().SetEnable(enable);
	}

}

void BaseballScene::LateUpdate(float fElapsedTime) noexcept
{
}

void BaseballScene::Render(void) noexcept
{
}

void BaseballScene::LateRender(void) noexcept
{
}

void BaseballScene::Release(void) noexcept
{
}
