#include "pch.h"
#include "MiniGameUI.h"

#include "GameObject.h"
#include "Image.h"
#include "Text.h"
#include "Transform.h"
#include "RectTransform.h"
#include "SpriteRenderer.h"
#include "MainBanner.h"
#include "CountBanner.h"
#include "ScoreBanner.h"
#include "WinLoseBanner.h"
#include "FadeController.h"
#include "LoadingScene.h"
#include "RestartScene.h"

MiniGameUI::MiniGameUI(const MINIGAMETEXT& textinfo) noexcept
{
	_textInfo = textinfo;
}

void MiniGameUI::Start(void) noexcept
{
	MainBanner::MAINBANNER mainbannerInfo{};
	mainbannerInfo.originScale	= 2.f;
	mainbannerInfo.textSize		= 35.f;
	mainbannerInfo.bannerwidth	= 600.f;
	mainbannerInfo.bannerheight = 100.f;
	mainbannerInfo.bannername = L"코디 때리기";

	_step = STEP::INTRO;

	gameObject->AddComponent(_mainBanner = new MainBanner(mainbannerInfo));

	GameObject* countObject = GameObject::Instantiate("CountBannerObject");
	countObject->AddComponent(_countBanner = new CountBanner()); 

	GameObject* scoreBanner = GameObject::Instantiate("ScoreBannerObject");
	scoreBanner->AddComponent(_scoreBanner = new ScoreBanner());

	GameObject* winloseBanner = GameObject::Instantiate("WinloseBannerObject");
	winloseBanner->AddComponent(_winloseBanner = new WinLoseBanner());

	_loading = false;
}

void MiniGameUI::Update(float fElapsedTime) noexcept
{
	switch (_step)
	{
	case MiniGameUI::STEP::INTRO:
		if (_mainBanner->GetEnd())
		{
			_step = STEP::COUNT;
			_countBanner->GetGameObject()->SetActive(true);
		}
		break;
	case MiniGameUI::STEP::COUNT:
		if (_countBanner->GetEnd())
		{
			_step = STEP::INGAME;
			_scoreBanner->StartBanner();
		}
		break;
	case MiniGameUI::STEP::INGAME:
		if (_scoreBanner->GetEndGame())
		{
			_step = STEP::WIN_AND_LOSE;
			_winloseBanner->SetText(_scoreBanner->GetWinner());
			_winloseBanner->GetGameObject()->SetActive(true);
			_winloseBanner->PlaySound();
		} 
		break;
	case MiniGameUI::STEP::WIN_AND_LOSE:
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
	case MiniGameUI::STEP::END:
		break;
	default:
		break;
	}

}

void MiniGameUI::OnMainBanner(void) noexcept
{
	gameObject->GetComponent<MainBanner>()->OnBanner();
}
