#include "pch.h"
#include "WackaMoleMay.h"
#include "WackaInput.h"
#include "GameObject.h"
#include "Transform.h"
#include "BoneChildObject.h"
#include "ModelRenderer.h"
#include "WeaponCollider.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Animator.h"
#include "ScoreBanner.h"
#include "Damaged.h"
#include "EffectPlayer.h"
#include "AudioSource.h"

WackaMoleMay::WackaMoleMay(void) noexcept
	: WackaMolePlayer(TYPE::MAY)
	, _effectPlayer(nullptr)
{
	_inputHandler = new WackaInput(false);
	_inputHandler->SetWackaMolePlayer(this);
}

WackaMoleMay::TURN WackaMoleMay::Turn(DIRECTION dir) noexcept
{
	switch (_direction)
	{
	case WackaMolePlayer::DIRECTION::UP:
		if (WackaMolePlayer::DIRECTION::LEFT == dir)
			return TURN::R90;
		else if (WackaMolePlayer::DIRECTION::RIGHT == dir)
			return TURN::L90;
		else if (WackaMolePlayer::DIRECTION::DOWN == dir)
			return TURN::R180;
		break;
	case WackaMolePlayer::DIRECTION::DOWN:
		if (WackaMolePlayer::DIRECTION::LEFT == dir)
			return TURN::L90;
		else if (WackaMolePlayer::DIRECTION::RIGHT == dir)
			return TURN::R90;
		else if (WackaMolePlayer::DIRECTION::UP == dir)
			return TURN::R180;
		break;
	case WackaMolePlayer::DIRECTION::LEFT:
		if (WackaMolePlayer::DIRECTION::UP == dir)
			return TURN::L90;
		else if (WackaMolePlayer::DIRECTION::RIGHT == dir)
			return TURN::R180;
		else if (WackaMolePlayer::DIRECTION::DOWN == dir)
			return TURN::R90;
		break;
	case WackaMolePlayer::DIRECTION::RIGHT:
		if (WackaMolePlayer::DIRECTION::UP == dir)
			return TURN::R90;
		else if (WackaMolePlayer::DIRECTION::LEFT == dir)
			return TURN::R180;
		else if (WackaMolePlayer::DIRECTION::DOWN == dir)
			return TURN::L90;
		break;
	case WackaMolePlayer::DIRECTION::END:
		break;
	default:
		break;
	}

	return TURN::END;
}

void WackaMoleMay::Hit(void) noexcept
{
	_animator->Play("May.ao|May_EV_Shed_Hammer_WAC_Hit", false, std::bind(&WackaMoleMay::HitEnd, this));
	_effectPlayer->Play(ASSETMANAGER->GetAudioAsset("Asset\\Audio\\MiniGame\\wackacody\\Play_World_SideContent_Music_Interactions_HammerToy_Hit.ogg"), 0.025f);
	_turn = TURN::END;
}

void WackaMoleMay::HitEnd(void) noexcept
{
	_isDirty = true;
	OnEndAnimation();
}

void WackaMoleMay::IdleHit(void) noexcept
{
	_isDirty = true;
	_turn = TURN::END;
	_checkKey = false;
	OnEndAnimation();
}

void WackaMoleMay::Start(void) noexcept
{
	__super::Start();

	transform->SetLocalPosition(0, 2.4f, 0);

	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	_hammerObject = GameObject::Instantiate("Hammer");
	_hammerTransform = _hammerObject->GetTransform();
	_hammerTransform->SetParent(transform);
	_hammerTransform->SetLocalPosition(0, 0, 0);
	_hammerObject->AddComponent(new BoneChildObject(_renderer->GetModel()->GetHierarchyNode("Backpack"), offsetMat));

	/* Hammer ModelRenderer 달자 */
	ModelRenderer* renderer = ModelRenderer::Create(false);
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Wackamole\\Hammer.fbx"));

	offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationZ(XMConvertToRadians(90.f));
	renderer->SetOffsetMatrix(offsetMat);
	_hammerObject->AddComponent(renderer);

	WeaponCollider* weaponCollider = new WeaponCollider(this);
	_hammerObject->AddComponent(weaponCollider);

	Rigidbody* rigid = Rigidbody::Create(false);
	rigid->SetUseGravity(false);
	_hammerObject->AddComponent(rigid);

	Collider* collider = Collider::CreateBoxCollider(0.2f, 0.15f, 0.2f, XMVectorSet(-0.4f, 0.6f, 0, 0));
	collider->SetIsTrigger(true);
	_hammerObject->AddComponent(collider);

	_dirtyDirection = DIRECTION::DOWN;
	_isDirty = true;
	_isTurn = false;
	_turnTimeAcc = 0.f;

	_animator->SetPlaySpeed(1.5f);

	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);
}

void WackaMoleMay::FixedUpdate(float ElapsedTime) noexcept
{
	__super::FixedUpdate(ElapsedTime);
}

void WackaMoleMay::Update(float ElapsedTime) noexcept
{

	__super::Update(ElapsedTime);

	if (false == _isTurn)
		return;

	float angle = XMVectorGetY(transform->GetLocalEulerAngle());
	float accAngle = 0.f;

	switch (_turn)
	{
	case WackaMoleMay::TURN::L90:
		accAngle = 90.f;
		transform->SetLocalEulerAngle(0, angle + XMConvertToRadians(accAngle), 0);
		break;
	case WackaMoleMay::TURN::R90:
		accAngle = -90.f;
		transform->SetLocalEulerAngle(0, angle + XMConvertToRadians(accAngle), 0);
		break;
	case WackaMoleMay::TURN::R180:
		accAngle = 180.f;
		transform->SetLocalEulerAngle(0, angle + XMConvertToRadians(accAngle), 0);
		break;
	case WackaMoleMay::TURN::NONE:
	case WackaMoleMay::TURN::END:
		break;
	default:
		CE_ASSERT("dnwls9935", "TURN 계산이 잘못됬습니다.");
		break;
	}
	_isTurn = false;
}

void WackaMoleMay::LateUpdate(float ElapsedTime) noexcept
{
	__super::LateUpdate(ElapsedTime);

	if (_isDirty)
	{
		if (_checkKey == true)
		{
			_animator->Play("May.ao|May_EV_Shed_Hammer_WAC_Hit", false, std::bind(&WackaMoleMay::IdleHit, this));
			_effectPlayer->Play(ASSETMANAGER->GetAudioAsset("Asset\\Audio\\MiniGame\\wackacody\\Play_World_SideContent_Music_Interactions_HammerToy_Hit.ogg"), 0.025f);
			OnStartAnimation();
			_isDirty = false;
			return;
		}

		if (true == _isTurn)
			return;

		_isDirty = false;
		_turn = Turn(_dirtyDirection);

		switch (_turn)
		{
		case WackaMoleMay::TURN::L90:
			_animator->Play("May.ao|May_EV_Shed_Hammer_WAC_Turn_Left90", false, std::bind(&WackaMoleMay::Hit, this));
			OnStartAnimation();
			break;
		case WackaMoleMay::TURN::R90:
			_animator->Play("May.ao|May_EV_Shed_Hammer_WAC_Turn_Right90", false, std::bind(&WackaMoleMay::Hit, this));
			OnStartAnimation();
			break;
		case WackaMoleMay::TURN::R180:
			_animator->Play("May.ao|May_EV_Shed_Hammer_WAC_Turn_Right180", false, std::bind(&WackaMoleMay::Hit, this));
			OnStartAnimation();
			break;
		case WackaMoleMay::TURN::NONE:
			_animator->Play("May.ao|May_EV_Shed_Hammer_WAC_Hit", false);
			OnStartAnimation();
			break;
		case WackaMoleMay::TURN::END:
			_animator->Play("May.ao|May_EV_Shed_Hammer_WAC_mh", true);
			break;
		default:
			CE_ASSERT("dnwls9935", "TURN 계산이 잘못됬습니다.");
			break;
		}

		_isTurn = true;
		_turnTimeAcc = 0.f;
		_direction = _dirtyDirection;
	}
}

void WackaMoleMay::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	GameObject* obj = other->GetGameObject();

	if ("WakcaMoleCody" != obj->GetName())
		return;

	_score += 2;

	_scoreBanner->GetScore(ScoreBanner::PLAYER::MAY, _score);
	_scoreBanner->PlaySound(ScoreBanner::PLAYER::MAY);

	GameObject* score = GameObject::Instantiate("GetMayScore");
	score->AddComponent(new Damaged(_hammerTransform, 2, 1));
}

void WackaMoleMay::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
}
