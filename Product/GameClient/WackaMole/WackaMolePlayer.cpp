#include "pch.h"
#include "WackaMolePlayer.h"
#include "GameObject.h"
#include "Animator.h"
#include "ModelRenderer.h"
#include "WackaInput.h"
#include "ScoreBanner.h"
#include "Damaged.h"

WackaMolePlayer::WackaMolePlayer(TYPE type) noexcept
	: _type(type)
{
}

WackaMolePlayer::~WackaMolePlayer(void) noexcept
{
	if (nullptr != _inputHandler)
	{
		delete _inputHandler;
		_inputHandler = nullptr;
	}
}

void WackaMolePlayer::Start(void) noexcept
{
	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	if (TYPE::MAY == _type)
	{
		_renderer = ModelRenderer::Create(false);
		_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Wackamole\\WackMay.fbx"));
		_renderer->SetOffsetMatrix(offsetMat);
		gameObject->AddComponent(_renderer);

		_animator = Animator::Create();
		gameObject->AddComponent(_animator);
		_animator->Play("May.ao|May_EV_Shed_Hammer_WAC_mh", true);
	}
	else if (TYPE::CODY == _type)
	{
		_renderer = ModelRenderer::Create(false);
		_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Wackamole\\WackCody.fbx"));
		_renderer->SetOffsetMatrix(offsetMat);
		gameObject->AddComponent(_renderer);

		_animator = Animator::Create();
		gameObject->AddComponent(_animator);
		_animator->Play("Cody.ao|Cody_EV_Shed_Hammer_WAC_mh_var1", true);
	}

	_isDirty = false;
	_direction = DIRECTION::END;
	_dirtyDirection = DIRECTION::END;

	_isAnimating = false;
	_score = 0;

	gameObject->SetLayer(GameObjectLayer::UNIT);
	gameObject->SetTag(GameObjectTag::PLAYER);
}

void WackaMolePlayer::FixedUpdate(float fElapsedTime) noexcept
{
	if (_scoreBanner == nullptr)
		_scoreBanner = GameObject::FindObjectByName("ScoreBannerObject")->GetComponent<ScoreBanner>();

	_inputHandler->FixedUpdate(fElapsedTime);
}

void WackaMolePlayer::Update(float fElaspedTime) noexcept
{
	_inputHandler->Update(fElaspedTime);
}

void WackaMolePlayer::LateUpdate(float fElapsedTime) noexcept
{
	_inputHandler->LateUpdate(fElapsedTime);
}

void WackaMolePlayer::AnimationUp(void) noexcept
{
	_isDirty = true;

	if (_dirtyDirection != DIRECTION::UP)
	{
		_dirtyDirection = DIRECTION::UP;
		_checkKey = false;
	}
	else
		_checkKey = true;

}

void WackaMolePlayer::AnimationDown(void) noexcept
{
	_isDirty = true;
	if (_dirtyDirection != DIRECTION::DOWN)
	{
		_dirtyDirection = DIRECTION::DOWN;
		_checkKey = false;
	}
	else
		_checkKey = true;
}

void WackaMolePlayer::AnimationLeft(void) noexcept
{
	_isDirty = true;
	if (_dirtyDirection != DIRECTION::LEFT)
	{
		_dirtyDirection = DIRECTION::LEFT;
		_checkKey = false;
	}
	else
		_checkKey = true;
}

void WackaMolePlayer::AnimationRight(void) noexcept
{
	_isDirty = true;
	if (_dirtyDirection != DIRECTION::RIGHT)
	{
		_dirtyDirection = DIRECTION::RIGHT;
		_checkKey = false;
	}
	else
		_checkKey = true;
}

void WackaMolePlayer::PadAnimationUp(void) noexcept
{
}

void WackaMolePlayer::PadAnimationDown(void) noexcept
{
}

void WackaMolePlayer::PadAnimationLeft(void) noexcept
{
}

void WackaMolePlayer::PadAnimationRight(void) noexcept
{
}

void WackaMolePlayer::OnEndAnimation(void) noexcept
{
	_isAnimating = false;
}

bool WackaMolePlayer::GetAnimation(void) noexcept
{
	return _isAnimating;
}

void WackaMolePlayer::OnStartAnimation(void) noexcept
{
	_isAnimating = true;
}
