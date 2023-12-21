#include "pch.h"
#include "WackaMoleCody.h"
#include "WackaInput.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "WackamoleHole.h"
#include "Animator.h"
#include "ScoreBanner.h"
#include "Damaged.h"
#include "EffectPlayer.h"
#include "AudioSource.h"

WackaMoleCody::WackaMoleCody(void) noexcept
	: WackaMolePlayer(TYPE::CODY)
{
	_inputHandler = new WackaInput(true);
	_inputHandler->SetWackaMolePlayer(this);
}

void WackaMoleCody::Start(void) noexcept
{
	__super::Start();
	transform->SetLocalPosition(0, 0, 0);

	_animator->SetPlaySpeed(1.f);

	_rigid = Rigidbody::Create(false);
	_rigid->SetUseGravity(false);
	gameObject->AddComponent(_rigid);

	_collider = Collider::CreateBoxCollider(0.5f, 0.5f, 0.5f, XMVectorSet(0, 1.f, 0, 0));
	gameObject->AddComponent(_collider);

	_hole[(int)POSITION::UP] = GameObject::FindLoadingObjectByName("WakcaMoleHoleUp")->GetComponent<WackamoleHole>();
	_hole[(int)POSITION::DOWN] = GameObject::FindLoadingObjectByName("WakcaMoleHoleDown")->GetComponent<WackamoleHole>();
	_hole[(int)POSITION::LEFT] = GameObject::FindLoadingObjectByName("WakcaMoleHoleLeft")->GetComponent<WackamoleHole>();
	_hole[(int)POSITION::RIGHT] = GameObject::FindLoadingObjectByName("WakcaMoleHoleRight")->GetComponent<WackamoleHole>();

	_on = false;
	_collider->SetEnable(false);
	_scoreTime = 0.f;

	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);
}

void WackaMoleCody::FixedUpdate(float ElapsedTime) noexcept
{
	__super::FixedUpdate(ElapsedTime);
}

void WackaMoleCody::Update(float ElapsedTime) noexcept
{
	__super::Update(ElapsedTime);

	if (true == _clearPosition)
		return;

	_lerpTime += ElapsedTime;

	if (_lerpTime >= 0.25f && false == _on)
	{
		_on = true;
		_collider->SetEnable(_on);
		_scoreTime = 0.f;
	}

	if (_lerpTime >= 1.f)
		_lerpTime = 1.f;

	if (true == _on)
	{
		_scoreTime += ElapsedTime * 3.f;

		_score += _scoreTime;

		if (_scoreTime >= 1)
		{
			_scoreTime = 0.f;
			GameObject* score = GameObject::Instantiate("Score");
			score->AddComponent(new Damaged(transform, 1, 2));
			_scoreBanner->PlaySound(ScoreBanner::PLAYER::CODY);
		}

		_scoreBanner->GetScore(ScoreBanner::PLAYER::CODY, _score);
	}

	float y = CETween::Lerp(0, 2.f, std::min(1.f, _lerpTime), CETween::EaseType::easeOutBack);
	transform->SetLocalPosition(XMVectorGetX(_position), y, XMVectorGetZ(_position));
}

void WackaMoleCody::LateUpdate(float ElapsedTime) noexcept
{
	__super::LateUpdate(ElapsedTime);

	if (_isDirty)
	{
		_isDirty = false;
		_direction = _dirtyDirection;
		_lerpTime = 0.f;

		int random = CE_MATH::Random(3);

		if(random == 0)
			_animator->Play("Cody.ao|Cody_EV_Shed_Hammer_WAC_mh_var1", true);
		else if (random == 1)
			_animator->Play("Cody.ao|Cody_EV_Shed_Hammer_WAC_mh_var2", true);
		else if (random == 2)
			_animator->Play("Cody.ao|Cody_EV_Shed_Hammer_WAC_mh_var3", true);


		switch (_dirtyDirection)
		{
		case WackaMolePlayer::DIRECTION::UP:
			_position = XMVectorSet(-1.5f, 0, 0, 0);
			break;
		case WackaMolePlayer::DIRECTION::DOWN:
			_position = XMVectorSet(1.5f, 0, 0, 0);
			break;
		case WackaMolePlayer::DIRECTION::LEFT:
			_position = XMVectorSet(0.f, 0, -1.4f, 0);
			break;
		case WackaMolePlayer::DIRECTION::RIGHT:
			_position = XMVectorSet(0, 0, 1.4f, 0);
			break;
		case WackaMolePlayer::DIRECTION::END:
			break;
		default:
			break;
		}

		for (int i = 0; i < (int)POSITION::END; i++)
		{
			if(i == (int)_dirtyDirection)
				_hole[i]->GetGameObject()->SetActive(false);
			else
				_hole[i]->GetGameObject()->SetActive(true);
		}

		transform->SetLocalPosition(_position);
		_on = false;
		_collider->SetEnable(_on);
	}
}

void WackaMoleCody::AnimationUp(void) noexcept
{
	if (_direction == DIRECTION::UP)
		return;
	else
	{
		_isDirty = true;
		_dirtyDirection = DIRECTION::UP;
		_clearPosition = false;
	}
}

void WackaMoleCody::AnimationDown(void) noexcept
{
	if (_direction == DIRECTION::DOWN)
		return;
	else
	{
		_isDirty = true;
		_dirtyDirection = DIRECTION::DOWN;
		_clearPosition = false;
	}
}

void WackaMoleCody::AnimationLeft(void) noexcept
{
	if (_direction == DIRECTION::LEFT)
		return;
	else
	{
		_isDirty = true;
		_dirtyDirection = DIRECTION::LEFT;
		_clearPosition = false;
	}
}

void WackaMoleCody::AnimationRight(void) noexcept
{
	if (_direction == DIRECTION::RIGHT)
		return;
	else
	{
		_isDirty = true;
		_dirtyDirection = DIRECTION::RIGHT;
		_clearPosition = false;
	}
}

void WackaMoleCody::AnimationIdle(void) noexcept
{
	transform->SetLocalPosition(0, 1.5f, 0);
	_clearPosition = true;
	_isDirty = true;
	_dirtyDirection = DIRECTION::END;

}


void WackaMoleCody::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	GameObject* obj = other->GetGameObject()->GetTransform()->GetParent()->GetGameObject();
	
	if ("WakcaMoleMay" != obj->GetName())
		return;

	AnimationIdle();
	_effectPlayer->Play(ASSETMANAGER->GetAudioAsset("Asset\\Audio\\MiniGame\\wackacody\\Play_World_SideContent_Shed_Minigame_WhackACody_CodyHit.ogg"), 0.1f);

}

void WackaMoleCody::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
}
