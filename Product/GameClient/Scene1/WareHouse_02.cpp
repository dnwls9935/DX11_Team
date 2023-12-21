#include "pch.h"
#include "WareHouse_02.h"
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
#include "Water.h"
#include "SuctionHoze.h"
#include "VacuumFan.h"
#include "VacuumHub.h"
#include "WeatherVane.h"
#include "PressBtn.h"
#include "SceneChanger.h"
#include "SuctionEffect.h"
#include "Lever.h"
#include "OptionPopup.h"
#include "BGMPlayer.h"
#include "PathFollower.h"

void WareHouse_02::SetLoadingContents(void) noexcept
{
	//AddLoadingAssetDirectory("Asset");
	AddLoadingAssetDirectory("Asset\\Models\\May");
	AddLoadingAssetDirectory("Asset\\Models\\Cody");
	AddLoadingAssetDirectory("Asset\\Texture");
	AddLoadingAssetDirectory("Asset\\UI");

	AddLoadingAssetDirectory("Asset\\Models\\Plank");
	AddLoadingAssetDirectory("Asset\\Models\\Floor_Beam_01_Combined_01");
	AddLoadingAssetDirectory("Asset\\Models\\Character");
	AddLoadingAssetDirectory("Asset\\Models\\Shed2\\Machine");
	AddLoadingAssetDirectory("Asset\\Models\\Shed2\\Misc");
	AddLoadingAssetDirectory("Asset\\Models\\Shed2\\Shedwares");
	AddLoadingAssetDirectory("Asset\\Models\\Structure1\\ChickenWire");
	AddLoadingAssetDirectory("Asset\\Models\\Structure1\\CinderBlock");
	AddLoadingAssetDirectory("Asset\\Models\\Structure1\\Concrete");
	AddLoadingAssetDirectory("Asset\\Models\\VacuumFan");
	AddLoadingAssetDirectory("Asset\\Models\\VentilationShaft");
	AddLoadingAssetDirectory("Asset\\Models\\Wall");
	AddLoadingAssetDirectory("Asset\\Models\\Wood");
	AddLoadingAssetDirectory("Asset\\Models\\WeatherVane");

	AddLoadingAssetDirectory("Asset\\Models\\lever");
	AddLoadingAssetDirectory("Asset\\Models\\VacuumPlatform");

	AddLoadingAssetDirectory("Asset\\Models\\Effect");
	AddLoadingAssetDirectory("Asset\\Audio");
	AddLoadingAssetDirectory("Asset\\PathData");

	AddLoadingInfo("Asset\\SceneInfo\\WareHouse_02.sceneinfo");

	PlayerPrefs::This().SetString("LastScene", std::string("WareHouse_02").c_str());
	PlayerPrefs::This().Save();

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("May");
		obj->GetTransform()->SetWorldPosition(-67.9f, -44.3f, 171.4f); //swamp
		May* player = new May(false);
		obj->AddComponent(player);

		FollowingCamera* followCam = new FollowingCamera(obj->GetTransform()
			, FollowingCamera::Range(0.35f, 0.65f, 0.5f, 1.f)
			, Vector3(0, 15, -15));

		obj = GameObject::Instantiate("May_Cam");
		Camera* camera = Camera::Create();
		//camera->SetTargetTexture(PlayerScreen::This().GetRenderTextureR());
		camera->SetClearOption(Camera::ClearOption::SolidColor);
		obj->AddComponent(camera);

		PathFollower* pathfollower = PathFollower::Create();
		obj->AddComponent(pathfollower);

		camera->SetClearColor(Vector4(0.f, 0.f, 0.f, 1));

		obj->GetTransform()->SetWorldPosition(0, 0, -10);
		obj->AddComponent(followCam);
		player->SetFollowCamera(obj->GetComponent<FollowingCamera>());
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("Cody");
		obj->GetTransform()->SetWorldPosition(-69.9f, -44.3f, 171.4f); //swamp
		Cody* player = new Cody(true);
		obj->AddComponent(player);

		FollowingCamera* followCam = new FollowingCamera(obj->GetTransform()
			, FollowingCamera::Range(0.35f, 0.65f, 0.5f, 1.f)
			, Vector3(0, 10, -10));

		obj = GameObject::Instantiate("Cody_Cam");
		Camera* camera = Camera::Create();
		obj->AddComponent(camera);
		obj->GetComponent<Camera>()->SetClearOption(Camera::ClearOption::SolidColor);

		obj->AddComponent(followCam);

		PathFollower* pathfollower = PathFollower::Create();
		obj->AddComponent(pathfollower);

		camera->SetClearColor(Vector4(0.f, 0.f, 0.f, 1));

		player->SetFollowCamera(obj->GetComponent<FollowingCamera>());
		});


	

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("Swamp");
		TerrainRenderer* renderer = TerrainRenderer::Create();
		obj->AddComponent(renderer);
		Terrain* terrain = Terrain::Create(1000, 1000, 5);
		renderer->SetTerrain(terrain);

		renderer->GetMaterial().SetShader(ShaderManager::This().GetShader(Shader::Type::WATER));
		renderer->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
			, ASSETMANAGER->GetTextureData("Asset\\Texture\\Vacuum_Goo_01_T1.dds"));
		renderer->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_NORMAL
			, ASSETMANAGER->GetTextureData("Asset\\Texture\\Vacuum_Goo_01_T2.dds"));

		Water::WATERDESC waterDesc;
		waterDesc._waveHeight = 1.f;
		waterDesc._speed = 1.5f;
		waterDesc._waveFrequency = 10.f;
		waterDesc._uvSpeed = 0.02f;
		Water* water = new Water(waterDesc);
		obj->AddComponent(water);

		obj->GetTransform()->SetWorldPosition(-350.6f, -49.f, 32.2f);
		});


	AddLoadingTask([&]() {
		GameObject* objSuctionHoze = GameObject::Instantiate("SuctionHoze_Swamp");
		objSuctionHoze->SetTag(GameObjectTag::OBJECT);
		SuctionHoze* suctionHoze = new SuctionHoze();
		objSuctionHoze->GetTransform()->SetWorldPosition(XMVectorSet(-143.65f, -35.51f, 246.72f, 1.f));
		XMVECTOR angle = CE_MATH::Deg2Rad * XMVectorSet(-90.f, 206.093f, 0, 0);
		objSuctionHoze->GetTransform()->Rotate(angle);
		objSuctionHoze->GetTransform()->SetLocalScale(1, 1, 1);
		objSuctionHoze->AddComponent(suctionHoze);

		objSuctionHoze = GameObject::Instantiate("SuctionHoze_Swamp");
		objSuctionHoze->SetTag(GameObjectTag::OBJECT);
		suctionHoze = new SuctionHoze();
		objSuctionHoze->GetTransform()->SetWorldPosition(XMVectorSet(-166.22f, -43.93f, 294.45f, 1.f));
		angle = CE_MATH::Deg2Rad * XMVectorSet(-90.f, 90.f, 0, 0);
		objSuctionHoze->GetTransform()->Rotate(angle);
		objSuctionHoze->GetTransform()->SetLocalScale(1, 1, 1);
		objSuctionHoze->AddComponent(suctionHoze);

		objSuctionHoze = GameObject::Instantiate("SuctionHoze_Swamp_Up");
		objSuctionHoze->SetTag(GameObjectTag::OBJECT);
		objSuctionHoze->GetTransform()->SetWorldPosition(XMVectorSet(-218.7f, -40.6f, 222.823f, 1.f));
		angle = CE_MATH::Deg2Rad * XMVectorSet(0, -180.f, 19.554f, 0);
		objSuctionHoze->GetTransform()->SetLocalEulerAngle(angle);
		objSuctionHoze->GetTransform()->SetLocalScale(1, 1, 1);
		suctionHoze = new SuctionHoze();
		objSuctionHoze->AddComponent(suctionHoze);

		///* Suction Effect */
		GameObject* objSuctionEffect = GameObject::Instantiate("SuctionEffect1_Enter");
		objSuctionEffect->SetTag(GameObjectTag::EFFECT);
		objSuctionEffect->SetLayer(GameObjectLayer::OBJECT);
		objSuctionEffect->GetTransform()->SetWorldPosition(XMVectorSet(-143.315f, -35.49f, 246.355f, 1.f));
		objSuctionEffect->GetTransform()->SetLocalEulerAngle(CE_MATH::Deg2Rad* XMVectorSet(0, 112.406f, 0, 0));
		objSuctionEffect->GetTransform()->SetLocalScale(0.5f, 0.5f, 0.5f);
		SuctionEffect* suctionEffect = new SuctionEffect(nullptr,
														 nullptr,
														 nullptr,
														 Vector3(5.8f,-1.5,40),
														 Vector3(CE_MATH::ToRadian(-92), CE_MATH::ToRadian(-55),1),
														 true);
		suctionEffect->SetSuction(true);
		suctionEffect->SetNext("SuctionEffect1_Exit");
		objSuctionEffect->AddComponent(suctionEffect);

		objSuctionEffect = GameObject::Instantiate("SuctionEffect1_Exit");
		objSuctionEffect->SetTag(GameObjectTag::EFFECT);
		objSuctionEffect->SetLayer(GameObjectLayer::OBJECT);
		objSuctionEffect->GetTransform()->SetWorldPosition(XMVectorSet(-166.22f, -43.93f, 294.45f, 1.f));
		objSuctionEffect->GetTransform()->SetLocalEulerAngle(CE_MATH::Deg2Rad* XMVectorSet(0, 0, 0, 0));
		objSuctionEffect->GetTransform()->SetLocalScale(0.5f, 0.5f, 0.5f);
		suctionEffect = new SuctionEffect(nullptr,
										  nullptr,
										  nullptr,
										  Vector3(0,-1.f,-2.f),
										  Vector3(0, CE_MATH::ToRadian(-55), 1),
										  false);
		suctionEffect->SetNext("");
		suctionEffect->SetSuction(false);
		objSuctionEffect->AddComponent(suctionEffect);

		/**/
		objSuctionEffect = GameObject::Instantiate("SuctionEffect2_Enter");
		objSuctionEffect->SetTag(GameObjectTag::EFFECT);
		objSuctionEffect->SetLayer(GameObjectLayer::OBJECT);
		objSuctionEffect->GetTransform()->SetWorldPosition(XMVectorSet(-198.04f, -34.18f, 222.47f, 1.f));
		objSuctionEffect->GetTransform()->SetLocalEulerAngle(CE_MATH::Deg2Rad* XMVectorSet(-56.347f, 90.f, 0, 0));
		objSuctionEffect->GetTransform()->SetLocalScale(1.f, 1.f, 1.f);
		suctionEffect = new SuctionEffect(nullptr,
										  nullptr,
										  nullptr,
										  Vector3(2.f,-1.5f,15.f),
										  Vector3(CE_MATH::ToRadian(-90.f), CE_MATH::ToRadian(-165.f), 0.4f),
										  true);
		suctionEffect->SetSuction(true);
		suctionEffect->SetNext("SuctionEffect2_Exit");
		objSuctionEffect->AddComponent(suctionEffect);

		objSuctionEffect = GameObject::Instantiate("SuctionEffect2_Exit");
		objSuctionEffect->SetTag(GameObjectTag::EFFECT);
		objSuctionEffect->SetLayer(GameObjectLayer::OBJECT);
		objSuctionEffect->GetTransform()->SetWorldPosition(XMVectorSet(-220.05f, -40.32f, 222.91f, 1.f));
		objSuctionEffect->GetTransform()->SetLocalEulerAngle(CE_MATH::Deg2Rad* XMVectorSet(-17.802f, -90.f, 0, 0));
		objSuctionEffect->GetTransform()->SetLocalScale(1.f, 1.f, 1.f);
		suctionEffect = new SuctionEffect(nullptr,
										  nullptr,
										  nullptr,
										  Vector3(0,-0.7f,-2.f),
										  Vector3(CE_MATH::ToRadian(-85), CE_MATH::ToRadian(0), CE_MATH::ToRadian(0)),
										  false);
		suctionEffect->SetNext("");
		suctionEffect->SetSuction(false);
		objSuctionEffect->AddComponent(suctionEffect);


		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("Vacuum_Fan0");
		obj->SetTag(GameObjectTag::OBJECT);
		VacuumFan* vacuumFan = new VacuumFan(true);
		obj->AddComponent(vacuumFan);
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-120.07f, -47.78f, 239.01f, 1.f));

		obj = GameObject::Instantiate("Vacuum_Fan1");
		obj->SetTag(GameObjectTag::OBJECT);
		vacuumFan = new VacuumFan(true);
		obj->AddComponent(vacuumFan);
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-129.79f, -47.78f, 241.25f, 1.f));

		obj = GameObject::Instantiate("Vacuum_Fan3");
		obj->SetTag(GameObjectTag::OBJECT);
		vacuumFan = new VacuumFan(true);
		obj->AddComponent(vacuumFan);
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-199.7f, -44.8f, 205.3f, 1.f));
		obj->GetTransform()->SetLocalScale(3, 3, 3);

		obj = GameObject::Instantiate("Vacuum_Fan3");
		obj->SetTag(GameObjectTag::OBJECT);
		vacuumFan = new VacuumFan(true);
		obj->AddComponent(vacuumFan);
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-199.7f, -44.8f, 176.2f, 1.f));
		obj->GetTransform()->SetLocalScale(3, 3, 3);
		});


	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("Vacuum_HubSmall");
		obj->SetTag(GameObjectTag::OBJECT);
		VacuumHub* vacuumHubSmall = new VacuumHub();
		obj->AddComponent(vacuumHubSmall);
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-168.1f, -39.82f, 274.69f, 1.f));
		XMVECTOR angle = CE_MATH::Deg2Rad * XMVectorSet(0, 90.f, 0, 0);
		obj->GetTransform()->Rotate(angle);

	/*	obj = GameObject::Instantiate("Vacuum_HubSmall");
		obj->SetTag(GameObjectTag::OBJECT);
		vacuumHubSmall = new VacuumHub();
		obj->AddComponent(vacuumHubSmall);
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-58.67f, -46.01f, 239.42f, 1.f));
		angle = CE_MATH::Deg2Rad * XMVectorSet(-90.f, 0, 0, 0);
		obj->GetTransform()->Rotate(angle);*/

		/*	obj = GameObject::Instantiate("Vacuum_HubSmall");
			obj->SetTag(GameObjectTag::OBJECT);
			vacuumHubSmall = new VacuumHub();
			obj->AddComponent(vacuumHubSmall);
			obj->GetTransform()->SetWorldPosition(XMVectorSet(-224.29f, -24.6f, 166.59f, 1.f));
			angle = CE_MATH::Deg2Rad * XMVectorSet(-90.f, 0, 0, 0);
			obj->GetTransform()->Rotate(angle);*/

		obj = GameObject::Instantiate("Vacuum_HubSmall_Move");
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-180.7f, -51.43f, 299.5f, 1.f));
		angle = CE_MATH::Deg2Rad * XMVectorSet(0, -90.f, 0, 0);
		obj->GetTransform()->SetLocalEulerAngle(angle);
		obj->SetTag(GameObjectTag::GROUND);
		vacuumHubSmall = new VacuumHub();
		obj->AddComponent(vacuumHubSmall);
		

		obj = GameObject::Instantiate("Vacuum_Hub");
		obj->SetTag(GameObjectTag::OBJECT);
		vacuumHubSmall = new VacuumHub();
		obj->AddComponent(vacuumHubSmall);
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-200.26f, -41.49327f, 222.4887f, 1.f));
		angle = CE_MATH::Deg2Rad * XMVectorSet(-90.f, 0, 0, 0);
		obj->GetTransform()->Rotate(angle);

		obj = GameObject::Instantiate("Vacuum_HubBig");
		obj->SetTag(GameObjectTag::OBJECT);
		vacuumHubSmall = new VacuumHub();
		obj->AddComponent(vacuumHubSmall);
		obj->GetTransform()->SetWorldPosition(XMVectorSet(-216.33f, -25.44f, 147.42f, 1.f));
		angle = CE_MATH::Deg2Rad * XMVectorSet(-90.f, -133.567f, 0, 0);
		obj->GetTransform()->Rotate(angle);

		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("WeatherVane");
		obj->SetTag(GameObjectTag::OBJECT);
		WeatherVane* weatherVane = new WeatherVane();
		obj->AddComponent(weatherVane);
		});


	AddLoadingTask([&]() {
	/*	GameObject* btnObj = GameObject::Instantiate("PressBtn0_0");
		PressBtn* pressBtn = new PressBtn();
		btnObj->SetTag(GameObjectTag::GROUND);
		btnObj->GetTransform()->SetWorldPosition(-87.71f, -44.29f, 202.27f);
		btnObj->AddComponent(pressBtn);*/

		//GameObject* btnObj1 = GameObject::Instantiate("PressBtn0_1");
		//PressBtn* pressBtn = new PressBtn();
		//btnObj1->SetTag(GameObjectTag::GROUND);
		//btnObj1->GetTransform()->SetWorldPosition(-140.873f, -35.71f, 232.42f);
		//btnObj1->AddComponent(pressBtn);

		GameObject* leverObj0 = GameObject::Instantiate("Lever0");
		leverObj0->SetTag(GameObjectTag::OBJECT);
		Lever* lever = new Lever();
		leverObj0->AddComponent(lever);

		GameObject* leverObj1 = GameObject::Instantiate("Lever1");
		leverObj1->SetTag(GameObjectTag::OBJECT);
		lever = new Lever();
		leverObj1->AddComponent(lever);


		GameObject* objSuctionEffect = GameObject::Instantiate("VacuumFan0");
		objSuctionEffect->SetTag(GameObjectTag::EFFECT);
		objSuctionEffect->SetLayer(GameObjectLayer::OBJECT);
		objSuctionEffect->GetTransform()->SetWorldPosition(XMVectorSet(-120.07f, -47.78f, 239.01f, 1.f));
		objSuctionEffect->GetTransform()->SetLocalEulerAngle(CE_MATH::Deg2Rad* XMVectorSet(-90.f, 0, 0, 0));
		objSuctionEffect->GetTransform()->SetLocalScale(1, 1, 1);
		SuctionEffect* suctionEffect = new SuctionEffect(nullptr,
														leverObj0,
														leverObj1,
														Vector3(0, 0, 0),
														Vector3(0, 0, 0),
														false);
	
		objSuctionEffect->AddComponent(suctionEffect);

		objSuctionEffect = GameObject::Instantiate("VacuumFan1");
		objSuctionEffect->SetTag(GameObjectTag::EFFECT);
		objSuctionEffect->SetLayer(GameObjectLayer::OBJECT);
		objSuctionEffect->GetTransform()->SetWorldPosition(XMVectorSet(-129.79f, -47.78f, 241.25f, 1.f));
		objSuctionEffect->GetTransform()->SetLocalEulerAngle(CE_MATH::Deg2Rad* XMVectorSet(-90.f, 0, 0, 0));
		objSuctionEffect->GetTransform()->SetLocalScale(1, 1, 1);
		suctionEffect = new SuctionEffect(nullptr,
										 leverObj0,
										 leverObj1,
									     Vector3(0, 0, 0),
									     Vector3(0, 0, 0),
									     false);

		objSuctionEffect->AddComponent(suctionEffect);



		GameObject* btnObj = GameObject::Instantiate("PressBtn");
		PressBtn* pressBtn = new PressBtn();
		btnObj->SetTag(GameObjectTag::GROUND);
		btnObj->GetTransform()->SetWorldPosition(-180.45f, -34.33F, 221.68f);
		btnObj->AddComponent(pressBtn);

		objSuctionEffect = GameObject::Instantiate("VacuumFan");
		objSuctionEffect->SetTag(GameObjectTag::EFFECT);
		objSuctionEffect->SetLayer(GameObjectLayer::OBJECT);
		objSuctionEffect->GetTransform()->SetWorldPosition(XMVectorSet(-245.19f, -46.23f, 223.23f, 1.f));
		objSuctionEffect->GetTransform()->SetLocalEulerAngle(CE_MATH::Deg2Rad* XMVectorSet(-90.f, 0, 0, 0));
		objSuctionEffect->GetTransform()->SetLocalScale(1, 1, 1);
		suctionEffect = new SuctionEffect(nullptr, btnObj);
		objSuctionEffect->AddComponent(suctionEffect);


		btnObj = GameObject::Instantiate("PressBtn");
		pressBtn = new PressBtn();
		btnObj->SetTag(GameObjectTag::GROUND);
		btnObj->GetTransform()->SetWorldPosition(-266.56f, -29.53f, 230.42f);
		btnObj->AddComponent(pressBtn);

		objSuctionEffect = GameObject::Instantiate("VacuumFanBig");
		objSuctionEffect->SetTag(GameObjectTag::EFFECT);
		objSuctionEffect->SetLayer(GameObjectLayer::OBJECT);
		objSuctionEffect->GetTransform()->SetWorldPosition(XMVectorSet(-199.74f, -43.59f, 205.4f, 1.f));
		objSuctionEffect->GetTransform()->SetLocalEulerAngle(CE_MATH::Deg2Rad* XMVectorSet(-90.f, 0, 0, 0));
		objSuctionEffect->GetTransform()->SetLocalScale(5, 5, 5);
		suctionEffect = new SuctionEffect(nullptr,
										  btnObj,
										  nullptr,
										  Vector3(0,0,0.5),
									      Vector3(0,0,0),
									      false);
		objSuctionEffect->AddComponent(suctionEffect);


		objSuctionEffect = GameObject::Instantiate("VacuumFanBig");
		objSuctionEffect->SetTag(GameObjectTag::EFFECT);
		objSuctionEffect->SetLayer(GameObjectLayer::OBJECT);
		objSuctionEffect->GetTransform()->SetWorldPosition(XMVectorSet(-199.74f, -43.59f, 176.f, 1.f));
		objSuctionEffect->GetTransform()->SetLocalEulerAngle(CE_MATH::Deg2Rad* XMVectorSet(-90.f, 0, 0, 0));
		objSuctionEffect->GetTransform()->SetLocalScale(5, 5, 5);
		suctionEffect = new SuctionEffect(nullptr,
										  btnObj,
										  nullptr,
										  Vector3(0, 0, 0.5),
										  Vector3(0, 0, 0),
										  false);
		objSuctionEffect->AddComponent(suctionEffect);
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("SceneChanger");
		obj->SetTag(GameObjectTag::OBJECT);
		SceneChanger* sceneChanger = new SceneChanger();
		obj->AddComponent(sceneChanger);
		});
}

HRESULT WareHouse_02::Init(void) noexcept
{
	BGMPlayer::This()->Stop();
	FadeController::This()->StartFadeIn(0.1f);
	PhysicsSystem::Instance()->SetGravity(-9.8 * 5);

	PlayerPrefs::This().SetFloat("SavePointX", -67.9f);
	PlayerPrefs::This().SetFloat("SavePointY", -44.3f);
	PlayerPrefs::This().SetFloat("SavePointZ", 171.4f);

	GameObject* obj = GameObject::FindObjectByName("Cody_Cam");
	obj->GetComponent<Camera>()->SetTargetTexture(PlayerScreen::This().GetRenderTextureR());



	/* ============ BGM ============= */
	int bgmindex = BGMPlayer::This()->GetBGM("ShedMain");
	if (-1 == bgmindex)
	{
		bgmindex = BGMPlayer::This()->AddBGM("Asset\\Audio\\Shed\\Mus_Shed_Vacuum_Main_LP_156_C.ogg", "ShedMain");
	}

	BGMPlayer::This()->AutoPlay(bgmindex);
	
	return S_OK;
}

void WareHouse_02::FixedUpdate(float fElapsedTime) noexcept
{
	static bool enable = false;

	if (INPUT->GetKeyDown(VK_F2))
	{
		enable = !enable;
		DebugRenderer::This().SetEnable(enable);
	}

	if (INPUT->GetKeyDown('1'))
	{
		GameObject::FindObjectByName("Cody")->GetTransform()->SetWorldPosition(-140.74f, -35.f, 233.f);
		GameObject::FindObjectByName("May")->GetTransform()->SetWorldPosition(-138.74f, -35.f, 233.f);
	}
	if (INPUT->GetKeyDown('2'))
	{
		GameObject::FindObjectByName("Cody")->GetTransform()->SetWorldPosition(-180.66f, -42.08f, 259.59f);
		GameObject::FindObjectByName("May")->GetTransform()->SetWorldPosition(-178.66f, -42.08f, 259.59f);
	}
	if (INPUT->GetKeyDown('3'))
	{
		GameObject::FindObjectByName("Cody")->GetTransform()->SetWorldPosition(-243.31f, -21.f, 126.49f);
		GameObject::FindObjectByName("May")->GetTransform()->SetWorldPosition(-243.31f, -21.f, 126.49f);
	}
	
}

void WareHouse_02::Update(float fElapsedTime) noexcept
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

void WareHouse_02::LateUpdate(float fElapsedTime) noexcept
{

}

void WareHouse_02::Render(void) noexcept
{

}

void WareHouse_02::LateRender(void) noexcept
{

}

void WareHouse_02::Release(void) noexcept
{

}
