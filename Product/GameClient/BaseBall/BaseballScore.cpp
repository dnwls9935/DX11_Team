#include "pch.h"
#include "BaseballScore.h"
#include "GameObject.h"
#include "Transform.h"
#include "RectTransform.h"
#include "BaseballPlayerScore.h"
#include "Text.h"
#include "BaseBallUI.h"

BaseballScore::BaseballScore(void) noexcept
{
}

void BaseballScore::Start(void) noexcept
{
	for (int i = 0; i < (int)PLAYER::END; i++)
	{
		_playerScoreObject[i] = GameObject::Instantiate("PlayerScore");
		_playerScoreTransform[i] = _playerScoreObject[i]->GetTransform();
		_playerScoreObject[i]->AddComponent(_playerScore[i] = new BaseballPlayerScore((BaseballPlayerScore::PLAYER)i));

		_playerNameObject[i] = GameObject::Instantiate("PlayerName");
		_nameText[i] = UI::Text::Create();
		_nameText[i]->SetAligement(Text::Aligement::CENTER_MIDDLE);
		_nameText[i]->SetSize(20);
		_playerNameObject[i]->AddComponent(_nameText[i]);
		_playerNameObject[i]->SetSortOrder(20);
		_playerNameObject[i]->GetComponent<RectTransform>()->SetPivot(0.5f, 0.5f);

		if (i == (int)PLAYER::MAY)
		{
			_nameText[i]->SetColor(0.26f, 0.67f, 0.87f, 1);
			_playerNameObject[i]->GetTransform()->SetWorldPosition(XMVectorSet(WINCX / 2.f - 74, WINCY / 6.f - 5, 0.f, 1.f));
		}
		else
		{ 
			_nameText[i]->SetColor(0.66f, 0.815f, 0.49f, 1);
			_playerNameObject[i]->GetTransform()->SetWorldPosition(XMVectorSet(WINCX / 2.f + 74, WINCY / 6.f - 5, 0.f, 1.f));
		}
	}

	_vsObject = GameObject::Instantiate("BaseballPlayerScoreBorder");
	_vsTransform = _vsObject->GetTransform();
	_vsTransform->SetParent(transform);
	_vsImage = UI::Image::Create();
	_vsImage->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\MiniGames\\VS_Icon.dds"));
	_vsImage->SetColor(1, 1, 1, 0);
	_vsObject->AddComponent(_vsImage);
	_vsObject->SetSortOrder(20);
	_vsRectTransform = _vsObject->GetComponent<RectTransform>();
	_vsRectTransform->SetPivot(0.5f, 0.5f);
	_vsRectTransform->SetWidth(70);
	_vsRectTransform->SetHeight(70);
	_vsTransform->SetWorldPosition(XMVectorSet(WINCX / 2.f, WINCY / 7.f + 20, 0.f, 1.f));

	gameObject->SetActive(false);
	_lerpEnd = false;
	_endGame = false;
}

void BaseballScore::Update(float fElapsedTime) noexcept
{ 


	if (true == _endGame)
		return;
	
	if (false == _lerpEnd)
	{
		_lerpTime += fElapsedTime;

		if (_lerpTime >= 1.f)
		{
			_lerpTime = 1.f;
			_lerpEnd = true;
		}

		float alpha = CETween::Lerp(0, 1, std::min(1.f, _lerpTime), CETween::EaseType::easeLiner);

		for (int i = 0; i < (int)PLAYER::END; i++)
		{
			_playerScore[i]->SetAlpha(alpha);

			if (alpha == 1)
			{
				if (i == (int)PLAYER::MAY)
					_nameText[i]->SetString(L"May");
				else
					_nameText[i]->SetString(L"Cody");
			}
				
		}
		_vsImage->SetColor(1, 1, 1, alpha);
	}
	else
	{

		for (int i = 0; i< (int)PLAYER::END; i++)
		{
			int score = _playerScore[i]->GetScore();
	
			if (score >= 20)
			{
				_endGame = true;
				BaseballPlayerScore::PLAYER type = _playerScore[i]->GetPlayer();

				if (type == BaseballPlayerScore::PLAYER::MAY)
					_winner = "May Win!!!";
				else if (type == BaseballPlayerScore::PLAYER::CODY)
					_winner = "Cody Win!!!";
			}
		}
	
	}
}

void BaseballScore::GetScore(PLAYER player, int score) noexcept
{
	BaseBallUI* obj = GameObject::FindObjectByName("BaseballUI")->GetComponent<BaseBallUI>();
	if (obj == nullptr)
		return;

	BaseBallUI::STEP step = obj->GetStep();
	if (step == BaseBallUI::STEP::WIN_AND_LOSE
		|| step == BaseBallUI::STEP::INTRO)
		return;

}

void BaseballScore::StartBanner(void) noexcept
{
	gameObject->SetActive(true);

	for (int i = 0; i < (int)PLAYER::END; i++)
	{
		_playerScoreObject[i]->SetActive(true);
	}
}
