#include "pch.h"
#include "PlayerScore.h"
#include "GameObject.h"
#include "Transform.h"
#include "RectTransform.h"
#include "EffectPlayer.h"
#include "AudioSource.h"

PlayerScore::PlayerScore(PLAYER player) noexcept
	: _player(player)
{
	/* 16°³ */
}

void PlayerScore::Start(void) noexcept
{
	if (_player == PLAYER::MAY)
	{
		_borderObject = GameObject::Instantiate("PlayerScoreBorder");
		_borderTransform = _borderObject->GetTransform();
		_borderTransform->SetParent(transform);
		_borderImage = UI::Image::Create();
		_borderImage->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Input\\Frame_Rect.dds"));
		_borderImage->SetColor(1, 1, 1, 0);
		_borderObject->AddComponent(_borderImage);
		_borderObject->SetSortOrder(10);
		_borderRectTransform = _borderObject->GetComponent<RectTransform>();
		_borderRectTransform->SetPivot(1, 0.5f);
		_borderRectTransform->SetWidth(300);
		_borderRectTransform->SetHeight(80);
		_borderTransform->SetWorldPosition(XMVectorSet(WINCX / 2.45f, WINCY / 7.f, 0.f, 1.f));
	}
	else
	{
		_borderObject = GameObject::Instantiate("PlayerScoreBorder");
		_borderTransform = _borderObject->GetTransform();
		_borderTransform->SetParent(transform);
		_borderImage = UI::Image::Create();
		_borderImage->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Input\\Frame_Rect.dds"));
		_borderImage->SetColor(1, 1, 1, 1);
		_borderObject->AddComponent(_borderImage);
		_borderObject->SetSortOrder(10);
		_borderRectTransform = _borderObject->GetComponent<RectTransform>();
		_borderRectTransform->SetPivot(0, 0.5f);
		_borderRectTransform->SetWidth(300);
		_borderRectTransform->SetHeight(80);
		_borderTransform->SetWorldPosition(XMVectorSet(WINCX / 1.7f, WINCY / 7.f, 0.f, 1.f));
	}


	for(int i = 0 ; i < 16; i++)
	{
		_scoreObject[i] = GameObject::Instantiate("PlayerScoreBorder");
		_scoreTransform[i] = _scoreObject[i]->GetTransform();
		_scoreTransform[i]->SetParent(_borderTransform);
		_scoreImage[i] = UI::Image::Create();
		_scoreImage[i]->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Option\\SoundBar.dds"));
		_scoreImage[i]->SetColor(1, 1, 1, 0.1f);
		_scoreObject[i]->AddComponent(_scoreImage[i]);
		_scoreObject[i]->SetSortOrder(15);
		_scoreRectTransform[i]= _scoreObject[i]->GetComponent<RectTransform>();
		_scoreRectTransform[i]->SetPivot(0.5f, 0.5f);
		_scoreRectTransform[i]->SetWidth(30.f);
		_scoreRectTransform[i]->SetHeight(70);

		if (_player == PLAYER::MAY)
		{
			int index = -30 + (-16 * i);
			_scoreTransform[i]->SetLocalPosition(index, 0, 0);
		}
		else
		{
			int index = 30 + (16 * i);
			_scoreTransform[i]->SetLocalPosition(index, 0, 0);
		}
	}

	gameObject->SetActive(false);

}

void PlayerScore::Update(float fElapsedTime) noexcept
{
	for (int i = 0; i < _currentScore; i++)
	{
		if(_player == PLAYER::MAY)
			_scoreImage[i]->SetColor(0.26f, 0.67f, 0.87f, 1);
		else
			_scoreImage[i]->SetColor(0.66f, 0.815f, 0.49f, 1);
	}
}

void PlayerScore::SetScore(int score) noexcept
{
	if (_currentScore < 16)
		_currentScore = score;
	else
		_currentScore = 16;
}

void PlayerScore::SetAlpha(float alpha) noexcept
{
	_borderImage->SetColor(1,1,1, alpha);
}
