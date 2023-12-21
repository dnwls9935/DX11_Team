#include "pch.h"
#include "BaseballPlayerScore.h"
#include "GameObject.h"
#include "Transform.h"
#include "RectTransform.h"
#include "text.h"

BaseballPlayerScore::BaseballPlayerScore(PLAYER player) noexcept
	: _player(player)
	, _currentScore(0)
	, _start(false)
{
}

void BaseballPlayerScore::Start(void) noexcept
{
	_textObject = GameObject::Instantiate("PlayerScoreText");
	_scoreText = UI::Text::Create();
	_scoreText->SetAligement(Text::Aligement::CENTER_MIDDLE);
	_scoreText->SetColor(1.f, 1.f, 1.f, 0.f);
	_scoreText->SetSize(30);
	_scoreText->SetFont(AssetManager::Instance()->GetFontData("Asset\\Font\\gothic.spritefont"));
	_textObject->AddComponent(_scoreText);
	_textObject->SetSortOrder(20);
	_textObject->GetComponent<RectTransform>()->SetPivot(0.5f, 0.5f);

	if (_player == PLAYER::MAY)
	{
		_borderObject = GameObject::Instantiate("PlayerScoreBorder");
		_borderTransform = _borderObject->GetTransform();
		_borderTransform->SetParent(transform);
		_borderImage = UI::Image::Create();
		_borderImage->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Input\\Frame_Rect.dds"));
		_borderImage->SetColor(0.26f, 0.67f, 0.87f, 1);
		_borderObject->AddComponent(_borderImage);
		_borderObject->SetSortOrder(10);
		_borderRectTransform = _borderObject->GetComponent<RectTransform>();
		_borderRectTransform->SetPivot(1, 0.5f);
		_borderRectTransform->SetWidth(120);
		_borderRectTransform->SetHeight(150);
		_borderTransform->SetWorldPosition(XMVectorSet(WINCX / 2.45f + 50, WINCY / 7.f + 20.f, 0.f, 1.f));
		_textObject->GetTransform()->SetWorldPosition(XMVectorSet(WINCX / 2.45f - 11, WINCY / 7.f + 20.f, 0.f, 1.f));

	}
	else
	{
		_borderObject = GameObject::Instantiate("PlayerScoreBorder");
		_borderTransform = _borderObject->GetTransform();
		_borderTransform->SetParent(transform);
		_borderImage = UI::Image::Create();
		_borderImage->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Input\\Frame_Rect.dds"));
		_borderImage->SetColor(0.66f, 0.815f, 0.49f, 1);
		_borderObject->AddComponent(_borderImage);
		_borderObject->SetSortOrder(10);
		_borderRectTransform = _borderObject->GetComponent<RectTransform>();
		_borderRectTransform->SetPivot(0, 0.5f);
		_borderRectTransform->SetWidth(120);
		_borderRectTransform->SetHeight(150);
		_borderTransform->SetWorldPosition(XMVectorSet(WINCX / 1.7f - 50, WINCY / 7.f + 20.f, 0.f, 1.f));
		_textObject->GetTransform()->SetWorldPosition(XMVectorSet(WINCX / 1.7f + 9, WINCY / 7.f + 20.f, 0.f, 1.f));

	}



	gameObject->SetActive(false);
}

void BaseballPlayerScore::Update(float fElapsedTime) noexcept
{

}

void BaseballPlayerScore::SetScore(void) noexcept
{
	if(_start)
	{ 
		if(_currentScore<20)
			_currentScore += 1;

		_scoreText->SetString(std::to_string(_currentScore).c_str());
	}
}

void BaseballPlayerScore::SetAlpha(float alpha) noexcept
{
	if (_player == PLAYER::MAY)
		_borderImage->SetColor(0.26f, 0.67f, 0.87f, alpha);
	else
		_borderImage->SetColor(0.66f, 0.815f, 0.49f, alpha);

	_scoreText->SetColor(1.f, 1.f, 1.f, alpha);

	if (alpha == 1)
	{
		_start = true;
		_scoreText->SetString(std::to_string(_currentScore).c_str());
	}
	
}
