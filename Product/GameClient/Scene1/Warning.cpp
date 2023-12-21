#include "pch.h"
#include "Warning.h"
#include "GameObject.h"
#include "Transform.h"
#include "Image.h"
#include "RectTransform.h"

Warning::Warning(void) noexcept
{
}

void Warning::Start(void) noexcept
{
	ASSETMANAGER->LoadDirectory("Asset\\UI\\Stage3\\Warning");

	transform->SetWorldPosition(WINCX * 0.5f, WINCY * 0.25f, 0.);

	_img = UI::Image::Create();
	_img->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Stage3\\Warning\\warning0.dds"));

	gameObject->AddComponent(_img);
	_rectTransform = gameObject->GetComponent<RectTransform>();
	_rectTransform->SetWidth(150);
	_rectTransform->SetHeight(150);
	_rectTransform->SetPivot(0.5f, 0.5f);

	gameObject->SetSortOrder(5);

	_show = false;
	_timeAcc = 0.f;

	_img->SetEnable(false);
}

void Warning::Update(float fElapsedTime) noexcept
{
	if (false == _show)
		return;

	_timeAcc += fElapsedTime;

	float size = CETween::Lerp(0, 200, std::min(1.f, _timeAcc), CETween::EaseType::easeOutBounce);

	if (_timeAcc >= 1.5f)
	{
		_timeAcc = 0.f;
		_show = false;
		_img->SetEnable(false);
		size = 200.f;
	}

	_rectTransform->SetWidth(150 + size);
	_rectTransform->SetHeight(150 + size);
}

void Warning::Show(void) noexcept
{
	_show = true;
	_img->SetEnable(true);
}
