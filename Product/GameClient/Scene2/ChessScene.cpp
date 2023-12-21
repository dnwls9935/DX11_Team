#include "pch.h"
#include "ChessScene.h"
#include "GameObject.h"
#include "Transform.h"
#include "DebugRenderer.h"
#include "PhysicsSystem.h"
#include "FadeController.h"
#include "Camera.h"

#include "WizardCody.h"
#include "CastleMay.h"
#include "PlayerManager.h"
#include "DungeonCamera.h"
#include "OptionPopup.h"
#include "ChessBoss.h"

#include "ChessBishop.h"
#include "ChessKnight.h"
#include "ChessPawn.h"
#include "ChessRook.h"
#include "BGMPlayer.h"

#include "ChessBoardDataManager.h"
#include "WareHouseCinematic.h"
#include "DungeonCinematic.h"
#include "NarrationManager.h"
#include "HallsDoor.h"
#include "DungeonStatus.h"
#include "AudioListener.h"

void ChessScene::SetLoadingContents(void) noexcept
{
	AddLoadingAssetDirectory("Asset\\Models\\Castle_Dungeon\\Napkin_Carpet_01");
	AddLoadingAssetDirectory("Asset\\Models\\Castle_Dungeon\\Wall_Torch_01");
	AddLoadingAssetDirectory("Asset\\Models\\Castle_Chessboard");
	AddLoadingAssetDirectory("Asset\\Models\\Castle_Halls");
	AddLoadingAssetDirectory("Asset\\Models\\Cody_Wizard");
	AddLoadingAssetDirectory("Asset\\Models\\May_Castle");
	AddLoadingAssetDirectory("Asset\\Models\\Monster");
	AddLoadingAssetDirectory("Asset\\Models\\Effect");
	AddLoadingAssetDirectory("Asset\\Models\\Item");
	AddLoadingAssetDirectory("Asset\\UI\\");
	AddLoadingAssetDirectory("Asset\\Texture\\");
	AddLoadingAssetDirectory("Asset\\Texture\\Dungeon");
//	AddLoadingAssetDirectory("Asset\\UI\\Dungeon");
	AddLoadingAssetDirectory("Asset\\Audio");
	AddLoadingAssetDirectory("Asset\\Cinematic\\Stage2");
	AddLoadingAssetDirectory("Asset\\PathData");

	AddLoadingInfo("Asset\\SceneInfo\\ChessRoom.sceneinfo");

	PlayerPrefs::This().SetString("LastScene", std::string("Chess").c_str());
	PlayerPrefs::This().Save();

	AddLoadingTask([&]() {

		GameObject* cody = GameObject::Instantiate("Cody");
		wCody = new WizardCody(true);
		cody->AddComponent(wCody);
		cody->GetTransform()->SetWorldPosition(10.53112f, 0.5f, -9.439049f);

		GameObject* may = GameObject::Instantiate("May");
		cMay = new CastleMay(false);
		may->AddComponent(cMay);
		may->GetTransform()->SetWorldPosition(7.466937f, 0.5f, -12.0102f);

		GameObject* cam = GameObject::Instantiate("Camera");
		Camera* mainCamera = Camera::Create();
		cam->AddComponent(mainCamera);
		DungeonCamera* dungeonCam = new DungeonCamera(may
													, cody
													, Vector3(0, 30.f, -19)// Vector3(6, 22.f, -16)
													, Vector3(CE_MATH::ToRadian(60), 0, 0)
													, 20
													, 30
													, false);
		cam->AddComponent(dungeonCam);
		cam->AddComponent(new PlayerManager(cMay, wCody,"Chess"));

		mainCamera->SetClearOption(Camera::ClearOption::SolidColor);
		Camera::SetMainCamera(mainCamera);
		mainCamera->SetClearColor(Vector4(0, 0, 0, 1));

		cMay->SetCamera(mainCamera);
		wCody->SetCamera(mainCamera);
		cam->AddComponent(AudioListener::Create());

		});

	AddLoadingTask([&]() {
		GameObject* chessboard = GameObject::Instantiate("ChessBoardMgr");
		/*========================================*/
		_boardmanager = new ChessBoardDataManager();
		/*========================================*/
		chessboard->AddComponent(_boardmanager);

		GameObject* chessBoss = GameObject::Instantiate("ChessBoss Mgr");
		chessBoss->AddComponent(new ChessBoss(_boardmanager));
		//chessBoss->GetComponent<ChessBoss>()->visible(false);
	});

	AddLoadingTask([&]() {
		GameObject::FindLoadingObjectByName("Halls_Door_01_Left")->AddComponent(new HallsDoor());
		GameObject::FindLoadingObjectByName("Halls_Door_01")->AddComponent(new HallsDoor());
		GameObject::FindLoadingObjectByName("Halls_Door_01_Cutie_Left")->AddComponent(new HallsDoor());
		GameObject::FindLoadingObjectByName("Halls_Door_01_Cutie_Right")->AddComponent(new HallsDoor());

		GameObject::FindLoadingObjectByName("NextScene")->SetActive(false);
		});

	AddLoadingTask([&]() {
		GameObject* obj = GameObject::Instantiate("Cinematic0");
		GameObject* maincamera = GameObject::FindLoadingObjectByName("Camera");

		std::vector<Cinematic::CINEMADESC> cinemavec{};

		Cinematic::CINEMADESC cinemadesc{};
		cinemadesc.csvfilename = "Asset\\Cinematic\\Stage2\\Cinema_ChessBoard.csv";
		cinemadesc.followtime = 25.95f;
		cinemadesc.offsetrotation[0] = { 0.f,CE_MATH::ToRadian(52.088f),0.f };
		cinemadesc.offsetrotation[1] = { 0.f,CE_MATH::ToRadian(52.088f),0.f };
		cinemadesc.worldposition[0] = { 9.079734f, 0.f, -12.645f };
		cinemadesc.worldposition[1] =  { 11.081f, 0.f, -11.311f };
		cinemadesc.animationindex[0] = 0;
		cinemadesc.animationindex[1] = 0;
		cinemadesc.animationspeed = 1.25f;

		cinemavec.emplace_back(cinemadesc);

		Cinematic::CINEMADESC cinemadesc1{};

		cinemadesc1.csvfilename = "Asset\\Cinematic\\Stage2\\Cinema_ChessBoard.csv";
		cinemadesc1.followtime = 15.f;

		cinemavec.emplace_back(cinemadesc1);

		obj->AddComponent(new DungeonCinematic(cinemavec, maincamera));



		});


	PostEffect::This().SetmiddleGrey(0.8f);
	PostEffect::This().SetWhitness(0.1f);
}

HRESULT ChessScene::Init(void) noexcept
{
	FadeController::This()->StartFadeIn(1.f);
	PhysicsSystem::Instance()->SetGravity(-9.8 * 5);


	BGMPlayer::This()->Stop();
	NarrationManager::This().Stop();

	NarrationManager::This().SetNarrationVolume(2.f);
	/* ============ BGM ============= */
	int bgmindex = BGMPlayer::This()->AddBGM("Asset\\Audio\\Stage2\\Environment\\Mus_Playroom_Castle_Chess_Stage02_LP_155_A_01.ogg", "ChessMain");
	BGMPlayer::This()->AutoPlay(bgmindex);
	/* ============================== */

	return S_OK;
}

void ChessScene::FixedUpdate(float fElapsedTime) noexcept
{
	if (!_cinema )
	{
		GameObject::FindObjectByName("Cinematic0")->GetComponent<DungeonCinematic>()->PlayChess();
		_cinema = true;
	}



	if (INPUT->GetKeyDown(VK_F6))
	{
		_cinema = true;
	}

	GameObject::FindObjectByName("ChessRookFX")->SetActive(true);

	if (INPUT->GetKeyDown('L'))
	{
		GameObject::TList tList = GameObject::FindObjectsByName("Rook");

		for (int i = 0 ; i < tList.size(); i++)
		{
			tList[i]->GetComponent<ChessRook>()->Spawn(CE_MATH::Random(63));
		}
	}
}

void ChessScene::Update(float fElapsedTime) noexcept
{
	NarrationManager::This().Update(fElapsedTime);

	static bool enable = false;
	if (INPUT->GetKeyDown(VK_F2))
	{
		enable = !enable;
		DebugRenderer::This().SetEnable(enable);
	}
	if (INPUT->GetKeyDown(VK_F4))
	{
		cMay->_move = !cMay->_move;
		wCody->_move = !wCody->_move;
	}
}

void ChessScene::LateUpdate(float fElapsedTime) noexcept
{

}

void ChessScene::Render(void) noexcept
{
}

void ChessScene::LateRender(void) noexcept
{
}

void ChessScene::Release(void) noexcept
{
}
