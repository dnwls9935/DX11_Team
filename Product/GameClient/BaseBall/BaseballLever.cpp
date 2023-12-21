#include "pch.h"
#include "BaseballLever.h"
#include "GameObject.h"
#include "Transform.h"

#include "BaseballPlayer.h"
#include "EffectPlayer.h"
#include "AudioSource.h"
#include "Animator.h"

BaseballLever::BaseballLever(bool ismay) noexcept
	: _down(false)
	, _up(true)
	, _state(IDLE)
{
	if (ismay)
	{
		_player = GameObject::FindLoadingObjectByName("ToyBody01")->GetComponent<BaseballPlayer>();
		_reverse = 1.f;
	}
	else
	{
		_player = GameObject::FindLoadingObjectByName("ToyBody02")->GetComponent<BaseballPlayer>();
		_reverse = -1.f;
	}
}

void BaseballLever::Start(void) noexcept
{
	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);
}

void BaseballLever::Update(float fElapsedTime) noexcept
{
	switch (_state)
	{
	case BaseballLever::IDLE:
		break;
	case BaseballLever::DOWN:
		TurnLeverDown(fElapsedTime);
		break;
	case BaseballLever::UP:
		TurnLeverUp(fElapsedTime);
		break;
	default:
		break;
	}
}

void BaseballLever::TurnLeverDown(float fElapsedTime) noexcept
{

	if (XMVectorGetX(transform->GetLocalEulerAngle()) <= CE_MATH::ToRadian(-132.847))
	{
		_state = UP;
		_effectPlayer->Play(ASSETMANAGER->GetAudioAsset("Asset\\Audio\\MiniGame\\baseball\\Play_World_SideContent_Playroom_Minigames_Baseball_Lever_Up.ogg"), 0.01f);

	}
	else
	{
		transform->Rotate(-3 * fElapsedTime, 0, 0, false);
		_player->GetTransform()->Rotate(0, 0, 7 * fElapsedTime * _reverse, false);
	}
}

void BaseballLever::TurnLeverUp(float fElapsedTime) noexcept
{

	if (XMVectorGetX(transform->GetLocalEulerAngle()) >= CE_MATH::ToRadian(-80.449))
	{
		_state = IDLE;
		_playerobj->GetComponent<Animator>()->Play(1, true);
	}
	else
	{
		transform->Rotate(3 * fElapsedTime, 0, 0, false);
		_player->GetTransform()->Rotate(0, 0, -7 * fElapsedTime * _reverse, false);
	}
}

void BaseballLever::SetLeverDown(GameObject* obj) noexcept
{
	if (_state == IDLE)
	{
		_state = DOWN;
		_effectPlayer->Play(ASSETMANAGER->GetAudioAsset("Asset\\Audio\\MiniGame\\baseball\\Play_World_SideContent_Playroom_Minigames_Baseball_Lever.ogg"), 0.01f);
	}
		_playerobj = obj;
	
}
