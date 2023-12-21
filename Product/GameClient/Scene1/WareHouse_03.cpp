#include "pch.h"
#include "WareHouse_03.h"
#include "FadeController.h"
#include "ModelRenderer.h"
#include "Light.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Animator.h"
#include "TerrainRenderer.h"
#include "Terrain.h"
#include "FollowingCamera.h"
#include "May.h"
#include "Cody.h"
#include "DebugRenderer.h"
#include "TerrainRenderer.h"
#include "ShaderManager.h"
#include "PhysicsSystem.h"
#include "PlayerScreen.h"
#include "BigFan.h"
#include "PumpValve.h"
#include "Dumbbell.h"
#include "BigFan_Vertical.h"
#include "VacuumBoss.h"
#include "SuctionEffect.h"
#include "Lever.h"

#include "PressBtn.h"
#include "BigFan_Module.h"
#include "BGMPlayer.h"

void WareHouse_03::SetLoadingContents(void) noexcept
{
	//AddLoadingAssetDirectory("Asset");
	AddLoadingAssetDirectory("Asset\\Models\\May");
	AddLoadingAssetDirectory("Asset\\Models\\Cody");
	AddLoadingAssetDirectory("Asset\\Texture");
	AddLoadingAssetDirectory("Asset\\UI");

	AddLoadingAssetDirectory("Asset\\Models\\Plank");
	AddLoadingAssetDirectory("Asset\\Models\\Floor_Beam_01_Combined_01");
	AddLoadingAssetDirectory("Asset\\Models\\Shed2\\Misc");
	AddLoadingAssetDirectory("Asset\\Models\\Shed2\\Shedwares");
	AddLoadingAssetDirectory("Asset\\Models\\Structure1\\ChickenWire");

	AddLoadingAssetDirectory("Asset\\Models\\Utilty");
	AddLoadingAssetDirectory("Asset\\Models\\VentilationShaft");
	AddLoadingAssetDirectory("Asset\\Models\\Wall");

	AddLoadingAssetDirectory("Asset\\Models\\BigFan");
	AddLoadingAssetDirectory("Asset\\Models\\PumpValve");

	AddLoadingAssetDirectory("Asset\\Models\\Dumbbell");
	AddLoadingAssetDirectory("Asset\\Models\\Shed2\\Machine");

	AddLoadingAssetDirectory("Asset\\Models\\Dumbbell");
	AddLoadingAssetDirectory("Asset\\Models\\Character\\VacuumBoss");
	AddLoadingAssetDirectory("Asset\\Models\\lever");

	AddLoadingAssetDirectory("Asset\\Audio");

	AddLoadingAssetDirectory("Asset\\Models\\Effect");
	AddLoadingInfo("Asset\\SceneInfo\\WareHouse_03.sceneinfo");

	PlayerPrefs::This().SetString("LastScene", std::string("WareHouse_03").c_str());
	PlayerPrefs::This().Save();

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("Cody");
		obj->GetTransform()->SetWorldPosition(-235.71f, -24.87f, 72.23f);
		Cody* player = new Cody(true);
		obj->AddComponent(player);

		FollowingCamera* followCam = new FollowingCamera(obj->GetTransform()
			, FollowingCamera::Range(0.35f, 0.65f, 0.5f, 1.f)
			, Vector3(0, 10, -10));

		obj = GameObject::Instantiate("Cody_Cam");
		obj->AddComponent(Camera::Create());
		obj->GetComponent<Camera>()->SetClearOption(Camera::ClearOption::SolidColor);
		
		obj->AddComponent(followCam);

		Camera* camera = obj->GetComponent<Camera>();
		camera->SetTargetTexture(PlayerScreen::This().GetRenderTextureR());

		camera->SetClearColor(Vector4(0.f, 0.f, 0.f, 1));

		player->SetFollowCamera(obj->GetComponent<FollowingCamera>());

		camera->AddPostEffect(PostEffect::EffectType::FOG);
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("May");
		obj->GetTransform()->SetWorldPosition(-235.71f, -24.87f, 75.23f);
		May* player = new May(false);
		obj->AddComponent(player);

		Camera::SetMainCamera(obj->GetComponent<Camera>());
		FollowingCamera* followCam = new FollowingCamera(obj->GetTransform()
			, FollowingCamera::Range(0.35f, 0.65f, 0.5f, 1.f)
			, Vector3(0, 15, -15));

		obj = GameObject::Instantiate("May_Cam");
		Camera* camera = Camera::Create();
		//camera->SetTargetTexture(PlayerScreen::This().GetRenderTextureL());
		camera->SetClearOption(Camera::ClearOption::SolidColor);
		obj->AddComponent(camera);

		camera->SetClearColor(Vector4(0.f, 0.f, 0.f, 1));

		obj->GetTransform()->SetWorldPosition(0, 0, -10);
		obj->AddComponent(followCam);
		player->SetFollowCamera(obj->GetComponent<FollowingCamera>());

		camera->AddPostEffect(PostEffect::EffectType::FOG);
		});


	AddLoadingTask([&]() {
		GameObject* obj = GameObject::FindLoadingObjectByName("Big_Fan_0");
		BigFan* bigFan = new BigFan(false);
		obj->AddComponent(bigFan);

		obj = GameObject::FindLoadingObjectByName("Big_Fan_1");
		bigFan = new BigFan(false);
		obj->AddComponent(bigFan);

		GameObject* bfc0 = GameObject::FindLoadingObjectByName("BigFan_Control");
		bfc0->AddComponent(new BigFan_Vertical());

		GameObject* bfc1 = GameObject::FindLoadingObjectByName("BigFan_Control_Out");
		bfc1->AddComponent(new BigFan_Vertical());

		obj = GameObject::Instantiate("PumpValve");
		obj->SetTag(GameObjectTag::OBJECT);
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-278.316f, -18.94f, -140.06f, 1.f));
		obj->GetTransform()->SetLocalScale(1.f, 1.f, 1.f);
		obj->GetTransform()->SetLocalEulerAngle(0, CE_MATH::Deg2Rad * 180.f, 0);
		PumpValve* pumpValve = new PumpValve();
		pumpValve->SetController(bfc0, bfc1);
		obj->AddComponent(pumpValve);

		obj = GameObject::FindLoadingObjectByName("Big_Fan_Module_Bar_0");
		obj->GetTransform()->SetLocalEulerAngle(0, CE_MATH::Deg2Rad * 90.f, 0);
		obj = GameObject::FindLoadingObjectByName("Big_Fan_Module_Bar_1");
		obj->GetTransform()->SetLocalEulerAngle(0, CE_MATH::Deg2Rad * 90.f, 0);
		obj = GameObject::FindLoadingObjectByName("Big_Fan_Module_Bar_2");
		obj->GetTransform()->SetLocalEulerAngle(0, CE_MATH::Deg2Rad * 90.f, 0);

		obj = GameObject::FindLoadingObjectByName("Big_Fan_Module_0");
		obj->AddComponent(new BigFan_Module());
		obj = GameObject::FindLoadingObjectByName("Big_Fan_Module_1");
		obj->AddComponent(new BigFan_Module());
		obj = GameObject::FindLoadingObjectByName("Big_Fan_Module_2");
		obj->AddComponent(new BigFan_Module());

	});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("Dumbbell");
		obj->SetTag(GameObjectTag::OBJECT);
		Dumbbell* dumbbell = new Dumbbell();
		obj->AddComponent(dumbbell);
	});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("VacuumBoss");
		obj->SetTag(GameObjectTag::OBJECT);
		VacuumBoss* vacuumBoss = new VacuumBoss();
		obj->AddComponent(vacuumBoss);
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("SuctionEffect");
		obj->SetTag(GameObjectTag::EFFECT);
		obj->SetLayer(GameObjectLayer::OBJECT);
		obj->GetTransform()->SetWorldPosition(-70.86f, -12.09f, -143.3678f);
		obj->GetTransform()->SetLocalEulerAngle(CE_MATH::Deg2Rad* XMVectorSet(0, -90.f,0, 1));
		obj->GetTransform()->SetLocalScale(4, 4, 0.5f);
		SuctionEffect* suctionEffect = new SuctionEffect(nullptr,
														 nullptr,
														 nullptr,
														 Vector3(0,-2.3f,40.f),
														 Vector3(CE_MATH::ToRadian(130.f), CE_MATH::ToRadian(0), CE_MATH::ToRadian(0)),
														 true);
		obj->AddComponent(suctionEffect);
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("Lever");
		obj->SetTag(GameObjectTag::OBJECT);
		Lever* lever = new Lever();
		obj->AddComponent(lever);
		});
}

HRESULT WareHouse_03::Init(void) noexcept
{
	BGMPlayer::This()->Stop();
	FadeController::This()->StartFadeIn(0.1f);
	PhysicsSystem::Instance()->SetGravity(-9.8 * 5);

	PlayerPrefs::This().SetFloat("SavePointX", -235.71f);
	PlayerPrefs::This().SetFloat("SavePointY", -24.87f);
	PlayerPrefs::This().SetFloat("SavePointZ", 75.23f);



	/* ============ BGM ============= */
	int bgmindex = BGMPlayer::This()->GetBGM("ShedMain");
	if (-1 == bgmindex)
	{
		bgmindex = BGMPlayer::This()->AddBGM("Asset\\Audio\\Shed\\Mus_Shed_Vacuum_Main_LP_156_C.ogg", "ShedMain");
	}
	BGMPlayer::This()->AutoPlay(bgmindex);
	/* ============================== */

	return S_OK;
}

void WareHouse_03::FixedUpdate(float fElapsedTime) noexcept
{
	static bool enable = false;

	if (INPUT->GetKeyDown(VK_F2))
	{
		enable = !enable;
		DebugRenderer::This().SetEnable(enable);
	}


	if (INPUT->GetKeyDown('1'))
	{
		GameObject::FindObjectByName("Cody")->GetTransform()->SetWorldPosition(-282.3f, -17.47f, -133.9f);
		GameObject::FindObjectByName("May")->GetTransform()->SetWorldPosition(-282.3f, -17.47f, -133.9f);
	}
	if (INPUT->GetKeyDown('2'))
	{
		GameObject::FindObjectByName("Cody")->GetTransform()->SetWorldPosition(-158.17f, -16.2f, -142.94f);
		GameObject::FindObjectByName("May")->GetTransform()->SetWorldPosition(-158.17f, -16.2f, -142.94f);
	}
}

void WareHouse_03::Update(float fElapsedTime) noexcept
{
	if (_divide == false)
		_divideTime += fElapsedTime;

	if (_divideTime >= 1.2f && _divide == false)
	{
		_divide = true;
		Camera::GetMainCamera()->SetTargetTexture(PlayerScreen::This().GetRenderTextureL());
		PlayerScreen::This().Show();
	}
}

void WareHouse_03::LateUpdate(float fElapsedTime) noexcept
{

}

void WareHouse_03::Render(void) noexcept
{

}

void WareHouse_03::LateRender(void) noexcept
{

}

void WareHouse_03::Release(void) noexcept
{
	
}
