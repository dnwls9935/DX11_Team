#include "pch.h"
#include "BaseBallUI.h"
#include "GameObject.h"
#include "Image.h"
#include "Text.h"
#include "Transform.h"
#include "RectTransform.h"
#include "SpriteRenderer.h"

#include "MainBanner.h"
#include "CountBanner.h"
#include "WinLoseBanner.h"
#include "BaseballScore.h"

#include "FadeController.h"
#include "LoadingScene.h"
#include "RestartScene.h"

BaseBallUI::BaseBallUI(void) noexcept
{
}

void BaseBallUI::Start(void) noexcept
{
	MainBanner::MAINBANNER mainbannerInfo{};
	mainbannerInfo.originScale = 2.f;
	mainbannerInfo.textSize = 30.f;
	mainbannerInfo.bannerwidth = 400.f;
	mainbannerInfo.bannerheight = 100.f;
	mainbannerInfo.bannername = L"¹èÆÃ ÆÀ";

	_step = STEP::INTRO;

	gameObject->AddComponent(_mainBanner = new MainBanner(mainbannerInfo));

	GameObject* countObject = GameObject::Instantiate("BaseballCountObject");
	countObject->AddComponent(_countBanner = new CountBanner());

	GameObject* scoreBanner = GameObject::Instantiate("BaseballScoreObject");
	scoreBanner->AddComponent(_scoreBanner = new BaseballScore());

	GameObject* winloseBanner = GameObject::Instantiate("BaseballWinloseObject");
	winloseBanner->AddComponent(_winloseBanner = new WinLoseBanner());

	_loading = false;
}

void BaseBallUI::Update(float fElapsedTime) noexcept
{
	switch (_step)
	{
	case BaseBallUI::STEP::INTRO:
		if (_mainBanner->GetEnd())
		{
			_step = STEP::COUNT;
			_countBanner->GetGameObject()->SetActive(true);
		}
		break;
	case BaseBallUI::STEP::COUNT:
		if (_countBanner->GetEnd())
		{
			_step = STEP::INGAME;
			_scoreBanner->StartBanner();
		}
		break;
	case BaseBallUI::STEP::INGAME:
		if (_scoreBanner->GetEndGame())
		{
			_step = STEP::WIN_AND_LOSE;
			_winloseBanner->SetText(_scoreBanner->GetWinner());
			_winloseBanner->GetGameObject()->SetActive(true);
		} 
		break;
	case BaseBallUI::STEP::WIN_AND_LOSE:
		if (true == _loading)
			return;
		if (_winloseBanner->End())
		{
			_loading = true;
			FadeController::This()->StartFadeOut(3.0f
												, true
												, false
												, [&]() {
													RestartScene::SetNextSceneName("Ending");
													SCENEMANAGER->LoadScene("Restart");
												});
		}
		break;
	case BaseBallUI::STEP::END:
		break;
	default:
		break;
	}
}

void BaseBallUI::OnMainBanner(void) noexcept
{
	gameObject->GetComponent<MainBanner>()->OnBanner();
}
