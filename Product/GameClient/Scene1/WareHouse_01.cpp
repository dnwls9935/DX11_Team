#include "pch.h"
#include "WareHouse_01.h"
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
#include "PowerSwitch.h"
#include "FuseLight.h"
#include "FuseCrab.h"
#include "PressBtn.h"
#include "MachineGearFrame.h"
#include "MachineGear.h"
#include "WallSlider.h"
#include "BandSawBlade.h"
#include "CircularSaw.h"
#include "SuctionHoze.h"
#include "SuctionEffect.h"
#include "LoadingScene.h" 
#include "TargetUI.h"
#include "BGMPlayer.h"
#include "CableTunnel.h"
#include "OptionPopup.h"
#include "NarrationManager.h"
#include "VolumeFogRenderer.h"
#include "WareHouseCinematic.h"

void WareHouse_01::SetLoadingContents(void) noexcept
{
	AddLoadingAssetDirectory("Asset\\Models\\May");
	AddLoadingAssetDirectory("Asset\\Models\\Cody");
	AddLoadingAssetDirectory("Asset\\Texture");
	AddLoadingAssetDirectory("Asset\\UI");
	AddLoadingAssetDirectory("Asset\\Models\\Basic");

	AddLoadingAssetDirectory("Asset\\Models\\Plank");
	AddLoadingAssetDirectory("Asset\\Models\\Floor_Beam_01_Combined_01");
	AddLoadingAssetDirectory("Asset\\Models\\Character\\FuseCrab");
	AddLoadingAssetDirectory("Asset\\Models\\Character\\SucktionHoze");
	AddLoadingAssetDirectory("Asset\\Models\\Character\\VacuumHub");
	AddLoadingAssetDirectory("Asset\\Models\\Characters");
	AddLoadingAssetDirectory("Asset\\Models\\Shed2\\Machine");
	AddLoadingAssetDirectory("Asset\\Models\\Shed2\\Shedwares");
	AddLoadingAssetDirectory("Asset\\Models\\Structure1");
	AddLoadingAssetDirectory("Asset\\Models\\VacuumFan");
	AddLoadingAssetDirectory("Asset\\Models\\Wall");
	AddLoadingAssetDirectory("Asset\\Models\\Wood");
	AddLoadingAssetDirectory("Asset\\Models\\VentilationShaft");

	AddLoadingAssetDirectory("Asset\\Models\\Environment\\Environment\\Prop\\Fantasy\\Shed\\Boss");
	AddLoadingAssetDirectory("Asset\\Models\\Shed0\\Decoration");
	AddLoadingAssetDirectory("Asset\\Models\\Shed0\\Furniture");
	AddLoadingAssetDirectory("Asset\\Models\\Utilty");
	AddLoadingAssetDirectory("Asset\\Models\\ToolBox");
	AddLoadingAssetDirectory("Asset\\Models\\CircularSaw");
	AddLoadingAssetDirectory("Asset\\Models\\Environment\\Environment\\Prop\\Fantasy\\Shed\\Decoration");
	AddLoadingAssetDirectory("Asset\\Models\\Environment\\Environment\\Prop\\Fantasy\\Shed\\Machine");

	AddLoadingAssetDirectory("Asset\\Models\\Effect");
	AddLoadingAssetDirectory("Asset\\Audio");
	AddLoadingAssetDirectory("Asset\\Narrative");
	AddLoadingAssetDirectory("Asset\\Cinematic\\Stage1");
	AddLoadingAssetDirectory("Asset\\PathData");


	AddLoadingAsset("Asset\\Models\\Shed2\\Machine\\BandSaw.fbx");

	AddLoadingInfo("Asset\\SceneInfo\\WareHouse_01.sceneinfo");

	PlayerPrefs::This().SetString("LastScene", std::string("WareHouse_01").c_str());
	PlayerPrefs::This().Save();

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("Cody");
		obj->GetTransform()->SetWorldPosition(-43.72f, 2.5f, -7.69f);
		_cody = new Cody(true);
		obj->AddComponent(_cody);


		FollowingCamera* followCam = new FollowingCamera(obj->GetTransform()
			, FollowingCamera::Range(0.35f, 0.65f, 0.5f, 1.f)
			, Vector3(0, 15, -15));

		obj = GameObject::Instantiate("Cody_Cam");
		Camera* camera = Camera::Create();
		camera->SetClearOption(Camera::ClearOption::SolidColor);
		camera->SetClearColor(Vector4(0.f, 0.f, 0.f, 1.f));
		camera->SetTargetTexture(PlayerScreen::This().GetRenderTextureR());
		//camera->SetUseFrustum(true);
		obj->AddComponent(camera);
		obj->AddComponent(followCam);
		_cody->SetFollowCamera(followCam);
		
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("May");
		obj->GetTransform()->SetWorldPosition(-40.72f, 2.5f, -7.69f);
		//May* player = new May(false);
		_may = new May(false);
		obj->AddComponent(_may);

		FollowingCamera* followCam = new FollowingCamera(obj->GetTransform()
			, FollowingCamera::Range(0.35f, 0.65f, 0.5f, 1.f)
			, Vector3(0, 15, -15));

		obj = GameObject::Instantiate("May_Cam");
		Camera* camera = Camera::Create();
		//camera->SetTargetTexture(PlayerScreen::This().GetRenderTextureL());
		camera->SetClearOption(Camera::ClearOption::SolidColor);
		camera->SetClearColor(Vector4(0.f, 0.f, 0.f, 1));
		Camera::SetMainCamera(camera);
		//camera->SetUseFrustum(true);
		obj->AddComponent(camera);


		obj->GetTransform()->SetWorldPosition(0, 0, -10);
		obj->AddComponent(followCam);

	
		_may->SetFollowCamera(followCam);
		});



	AddLoadingTask([&]() {
		GameObject* objPowerSwitch = GameObject::Instantiate("PowerSwitch");
		objPowerSwitch->SetTag(GameObjectTag::OBJECT);
		PowerSwitch* powerSwitch = new PowerSwitch();
		objPowerSwitch->AddComponent(powerSwitch);

		GameObject* objFL0 = GameObject::Instantiate("FuseLight0");
		objFL0->SetTag(GameObjectTag::OBJECT);
		FuseLight* fuseLight = new FuseLight(0);
		objFL0->AddComponent(fuseLight);
		objFL0->SetLayer(GameObjectLayer::OBJECT);

		GameObject* objFL1 = GameObject::Instantiate("FuseLight1");
		objFL1->SetTag(GameObjectTag::OBJECT);
		fuseLight = new FuseLight(1);
		objFL1->AddComponent(fuseLight);
		objFL1->SetLayer(GameObjectLayer::OBJECT);

		GameObject* objFL2 = GameObject::Instantiate("FuseLight2");
		objFL2->SetTag(GameObjectTag::OBJECT);
		fuseLight = new FuseLight(2);
		objFL2->AddComponent(fuseLight);
		objFL2->SetLayer(GameObjectLayer::OBJECT);


		GameObject* objSuctionHoze = GameObject::Instantiate("SuctionHoze_PowerSwitch");
		objSuctionHoze->SetTag(GameObjectTag::OBJECT);
		SuctionHoze* suctionHoze = new SuctionHoze();
		suctionHoze->SetFuseLight(0, objFL0);
		suctionHoze->SetFuseLight(1, objFL1);
		suctionHoze->SetFuseLight(2, objFL2);
		XMVECTOR position = XMVectorSet(-64.38f, -2.03f, 39.31f, 1.f);
		objSuctionHoze->GetTransform()->SetWorldPosition(position);
		XMVECTOR angle = CE_MATH::Deg2Rad * XMVectorSet(-90.f, -92.898f, 0, 0);
		objSuctionHoze->GetTransform()->Rotate(angle);
		objSuctionHoze->GetTransform()->SetLocalScale(2, 2, 2);
		objSuctionHoze->AddComponent(suctionHoze);


		GameObject* objSuctionEffect = GameObject::Instantiate("SuctionEffect");
		objSuctionEffect->SetTag(GameObjectTag::EFFECT);
		objSuctionEffect->SetLayer(GameObjectLayer::OBJECT);
		objSuctionEffect->GetTransform()->SetWorldPosition(XMVectorSet(-64.12f, -1.97f, 37.18f, 1.f));
		objSuctionEffect->GetTransform()->SetLocalEulerAngle(CE_MATH::Deg2Rad* XMVectorSet(0, 180.f, 0, 0));
		objSuctionEffect->GetTransform()->SetLocalScale(2, 2, 2);
		SuctionEffect* suctionEffect = new SuctionEffect(objSuctionHoze,
														nullptr,
														nullptr,
														Vector3(0,-0.5, 8),
														Vector3(0, CE_MATH::ToRadian(-55), 1),
														true);
		suctionEffect->SetSuction(true);
		objSuctionEffect->AddComponent(suctionEffect);
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("FuseCrab0");
		FuseCrab* fuseCrab = new FuseCrab(0);
		obj->AddComponent(fuseCrab);
		obj->SetTag(GameObjectTag::OBJECT);

		obj = GameObject::Instantiate("FuseCrab1");
		fuseCrab = new FuseCrab(1);
		obj->AddComponent(fuseCrab);
		obj->SetTag(GameObjectTag::OBJECT);


		obj = GameObject::Instantiate("FuseCrab2");
		fuseCrab = new FuseCrab(2);
		obj->AddComponent(fuseCrab);
		obj->SetTag(GameObjectTag::OBJECT);
		});


	AddLoadingTask([&]() {
		GameObject* mgf0 = GameObject::Instantiate("MachineGearFrame");

		mgf0->SetTag(GameObjectTag::DEFAULT);
		mgf0->GetTransform()->SetWorldPosition(-85.34f, 42.15f, -100.96f);
		MachineGearFrame* machineGearFrame = new MachineGearFrame(MachineGearFrame::POSITION::UP);
		mgf0->AddComponent(machineGearFrame);

		GameObject* mgf1 = GameObject::Instantiate("MachineGearFrame");
		machineGearFrame = new MachineGearFrame(MachineGearFrame::POSITION::DOWN);
		mgf1->AddComponent(machineGearFrame);
		mgf1->SetTag(GameObjectTag::DEFAULT);
		mgf1->GetTransform()->SetWorldPosition(-89.88f, 28.21f, -100.97f);

		GameObject* mg = GameObject::Instantiate("MachineGear");
		mg->GetTransform()->SetWorldPosition(-89.88f, 28.21f, -99.32f);
		MachineGear* machineGear = new MachineGear();
		mg->AddComponent(machineGear);
		mg->SetTag(GameObjectTag::OBSTACLE);

		GameObject* btnObj0 = GameObject::Instantiate("PressBtn0");
		PressBtn* pressBtn = new PressBtn();
		btnObj0->SetTag(GameObjectTag::GROUND);
		btnObj0->GetTransform()->SetWorldPosition(-87.93f, 22.06f, -89.93f);
		btnObj0->AddComponent(pressBtn);

		GameObject* btnObj1 = GameObject::Instantiate("PressBtn1");
		pressBtn = new PressBtn();
		btnObj1->SetTag(GameObjectTag::GROUND);
		btnObj1->GetTransform()->SetWorldPosition(-92.64f, 22.06f, -89.817f);
		btnObj1->AddComponent(pressBtn);

		mgf0->GetComponent<MachineGearFrame>()->SetBtn(btnObj0, btnObj1);
		mgf1->GetComponent<MachineGearFrame>()->SetBtn(btnObj0, btnObj1);
		mg->GetComponent<MachineGear>()->SetBtn(btnObj0, btnObj1);

		GameObject* wallSliderobj = GameObject::Instantiate("WallSlider");
		WallSlider* wallSlider = new WallSlider();
		wallSliderobj->AddComponent(wallSlider);
		wallSlider->SetBtn(btnObj0, btnObj1);
		
		GameObject* bandsawobj = GameObject::Instantiate("BandSawBlade");
		BandSawBlade* bandsaw = new BandSawBlade();
		bandsawobj->AddComponent(bandsaw);
		bandsawobj->SetTag(GameObjectTag::OBJECT);
		bandsawobj->GetTransform()->SetWorldPosition(-50.51f, 27.18f, -107.36f);
		bandsaw->SetBtn(btnObj0, btnObj1);
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("CircularSaw");
		obj->GetTransform()->SetWorldPosition(6.99f, 9.34f, -67.34f);
		CircularSaw* circularSaw = new CircularSaw();
		obj->AddComponent(circularSaw);
		obj->SetTag(GameObjectTag::OBJECT);
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("Cinematic0");
		GameObject* camera[2]{};
		camera[0] = GameObject::FindLoadingObjectByName("May_Cam");
		camera[1] = GameObject::FindLoadingObjectByName("Cody_Cam");
	
		WareHouseCinematic::CINEMADESC cinemadesc{};
		cinemadesc.csvfilename = "Asset\\Cinematic\\Stage1\\Cinema_Ware_House.csv";
		cinemadesc.followtime = 31.f;
		cinemadesc.offsetrotation[0] = { 0.f,CE_MATH::ToRadian(-39.2f),0.f };
		cinemadesc.offsetrotation[1] = { 0.f,CE_MATH::ToRadian(-41.3f),0.f };
		cinemadesc.worldposition[0] = { -37.8f, -3.76f, 29.9f };
		cinemadesc.worldposition[1] = { -37.8f, -3.76f, 31.7f };
		cinemadesc.animationindex[0] = 0;
		cinemadesc.animationindex[1] = 0;
		cinemadesc.animationspeed = 1.25f;

		std::vector<WareHouseCinematic::CINEMADESC> cinema{ cinemadesc };

		obj->AddComponent(new WareHouseCinematic(cinema	, camera));


		});
}

HRESULT WareHouse_01::Init(void) noexcept
{
	FadeController::This()->StartFadeIn(0.1f);
	PhysicsSystem::Instance()->SetGravity(-9.8 * 5);

	PlayerPrefs::This().SetFloat("SavePointX", -40.72f);
	PlayerPrefs::This().SetFloat("SavePointY", 2.5f);
	PlayerPrefs::This().SetFloat("SavePointZ", -7.69f);

	/* ============ BGM ============= */
	int bgmindex = BGMPlayer::This()->AddBGM("Asset\\Audio\\Shed\\Mus_Shed_Vacuum_Main_LP_156_C.ogg", "ShedMain");
	BGMPlayer::This()->AutoPlay(bgmindex);
	/* ============================== */

	if (!NarrationManager::This().SetCsvData("Asset\\Narrative\\Ware_House\\Ware_House_01_FindFuseCrab.csv"))
	{
		CE_ASSERT("rldnr", "NarrationData가 없습니다.");
	}
	NarrationManager::This().SetNarrationVolume(1.0f);
	NarrationManager::This().Play();

	//SCENEMANAGER->LoadSceneAsync("WareHouse_02", false);

	_cinema = false;

	return S_OK;
}

void WareHouse_01::FixedUpdate(float fElapsedTime) noexcept
{
	NarrationManager::This().Update(fElapsedTime);

	
	if (_cinema)
	{
		GameObject* cinema = GameObject::FindObjectByName("Cinematic0");
		cinema->GetComponent<WareHouseCinematic>()->Play();
		_cinema = false;
	}

	static bool enable = false;

	if (INPUT->GetKeyDown(VK_F2))
	{
		enable = !enable;
		DebugRenderer::This().SetEnable(enable);
	}

	if (INPUT->GetKeyDown(VK_F6))
	{
		_cinema = true;
	}

	if (INPUT->GetKeyDown('1'))
	{
		GameObject::FindObjectByName("Cody")->GetTransform()->SetWorldPosition(-113.f, 24.69f, -56.66f);
		GameObject::FindObjectByName("May")->GetTransform()->SetWorldPosition(-115.f, 24.69f, -56.66f);
	}

	if (INPUT->GetKeyDown('2'))
	{
		GameObject::FindObjectByName("Cody")->GetTransform()->SetWorldPosition(2.f, 7.5f, -80.5f);
		GameObject::FindObjectByName("May")->GetTransform()->SetWorldPosition(4.f, 7.5f, -80.5f);
	}



}

void WareHouse_01::Update(float fElapsedTime) noexcept
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

void WareHouse_01::LateUpdate(float fElapsedTime) noexcept
{

}

void WareHouse_01::Render(void) noexcept
{

}

void WareHouse_01::LateRender(void) noexcept
{

}

void WareHouse_01::Release(void) noexcept
{
}
