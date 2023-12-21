#include "pch.h"
#include "WinLoseBanner.h"
#include "GameObject.h"
#include "Transform.h"
#include "RectTransform.h"
#include "EffectPlayer.h"
#include "AudioSource.h"

WinLoseBanner::WinLoseBanner(void) noexcept
{
}

void WinLoseBanner::Start(void) noexcept
{
	_backGroundObject = GameObject::Instantiate("PlayerScoreBorder");
	_backGroundTransform = _backGroundObject->GetTransform();
	_backGroundTransform->SetParent(transform);
	_backGroundImage = UI::Image::Create();
	_backGroundImage->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Option\\Banner.dds"));
	_backGroundImage->SetColor(1, 1, 1, 0);
	_backGroundObject->AddComponent(_backGroundImage);
	_backGroundObject->SetSortOrder(1);
	_backGroundRectTransform = _backGroundObject->GetComponent<RectTransform>();
	_backGroundRectTransform->SetPivot(0.5f, 0.5f);
	_backGroundRectTransform->SetWidth(500);
	_backGroundRectTransform->SetHeight(400);
	_backGroundTransform->SetWorldPosition(XMVectorSet(WINCX / 2.f, WINCY / 2.f, 0.f, 1.f));

	_textObject = GameObject::Instantiate("PlayerScoreBorder");
	_textTransform = _textObject->GetTransform();
	_textTransform->SetParent(transform);
	_text = UI::Text::Create();
	_text->SetAligement(Text::Aligement::CENTER_MIDDLE);
	_text->SetColor(1, 1, 1, 0);
	_text->SetString("");
	_text->SetSize(50);
	_textObject->AddComponent(_text);
	_textObject->SetSortOrder(10);
	_textRectTransform = _textObject->GetComponent<RectTransform>();
	_textRectTransform->SetPivot(0.5f, 0.5f);
	_textRectTransform->SetWidth(400);
	_textRectTransform->SetHeight(400);
	_textTransform->SetWorldPosition(XMVectorSet(WINCX / 2.f, WINCY / 2.f, 0.f, 1.f));

	_lerpTime = 0.f;
	_end = false;
	gameObject->SetActive(false);

	GameObject* obj = GameObject::Instantiate("winlosebanner_sound");
	_effectPlayer = new EffectPlayer(AudioSource::Create());
	obj->AddComponent(_effectPlayer);
}

void WinLoseBanner::Update(float fElapsedTime) noexcept
{
	_lerpTime += fElapsedTime;
	_endTime += fElapsedTime;

	if (_endTime >= 2.f)
	{
		_end = true;
		gameObject->SetActive(false);
		return;
	}

	if (_lerpTime >= 1.f)
		_lerpTime = 1;

	float alpha = CETween::Lerp(0, 1, std::min(1.f, _lerpTime), CETween::EaseType::easeLiner);
	_backGroundImage->SetColor(1, 1, 1, alpha);

	_text->SetColor(1, 1, 1, alpha);
}

void WinLoseBanner::SetText(std::string str) noexcept
{
	_text->SetString(str.c_str());
}

void WinLoseBanner::PlaySound(void) noexcept
{
	_effectPlayer->Play(ASSETMANAGER->GetAudioAsset("Asset\\Audio\\MiniGame\\ui\\Play_UI_OnScreen_SideContent_MiniGame_Screen_WinningConfetti.ogg"), 0.1f);
}
