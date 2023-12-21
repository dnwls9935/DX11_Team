#include "pch.h"
#include "Animator.h"
#include "AudioSource.h"
#include "AudioAsset.h"
#include "BoneChildObject.h"
#include "May.h"
#include "CircularSaw.h"
#include "Cody.h"
#include "FuseCrab.h"
#include "FollowingCamera.h"
#include "GameObject.h"
#include "HierarcyNode.h"
#include "Lever.h"
#include "MachineGear.h"
#include "ModelRenderer.h"
#include "PlayerInput.h"
#include "PowerSwitch.h"
#include "PumpValve.h"
#include "PhysicsSystem.h"
#include "Rigidbody.h"
#include "SuctionEffect.h"
#include "SuctionHoze.h"
#include "Transform.h"
#include "WallSlider.h"
#include "DustGenerate.h"
#include "WareHouseCinematic.h"
#include "OptionPopup.h"
#include "PathFollower.h"
#include "EffectPlayer.h"
#include "AudioListener.h"


May::May(bool bOperationPad) noexcept
{
	/* May InputHandler 연동 */
	_inputHandler = new PlayerInput(bOperationPad);
	_inputHandler->SetActor(this);


	_actorType = Actor::Type::PLAYER;
}

May::~May(void) noexcept
{
	if (nullptr != _inputHandler)
	{
		delete _inputHandler;
		_inputHandler = nullptr;
	}
}

void May::Start(void) noexcept
{
	/* Audio Listener*/
	gameObject->AddComponent(AudioListener::Create());

	/* Renderer */
	_renderer = ModelRenderer::Create();
	gameObject->AddComponent(_renderer);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\May\\May_FSM_01.fbx"));
	Material::TList& mats = _renderer->GetMaterials();
	for (auto& mat : mats)
	{
		mat.SetColor(ShaderSemantic::Type::COLOR_DEPTHEFFECT, Vector4(0, 0, 1.f, 0.5f));
	}

	XMMATRIX OffsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
	_renderer->SetOffsetMatrix(OffsetMat);

	/* Animator */
	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play("May.ao|May_mh", true);

	_isDirtyState = true;
	_dirtyState = State::FALLING;

	/* RigidBody */
	_rigidbody = Rigidbody::Create(false);
	_rigidbody->SetFreezeRotationX(true);
	_rigidbody->SetFreezeRotationZ(true);
	_rigidbody->SetDrag(1.f);
	_rigidbody->SetAngularDrag(1.f);
	_rigidbody->SetUseGravity(true);
	gameObject->AddComponent(_rigidbody);

	/* Collider */
	_collider = Collider::CreateCapsuleCollider(0.3f, 0.6f, XMVectorSet(0, 0.9f, 0, 1));
	gameObject->AddComponent(_collider);

	/* Collider */
	_colliderTriggerBody = Collider::CreateCapsuleCollider(0.31f, 0.65f, XMVectorSet(0, 0.9f, 0, 1));
	_colliderTriggerBody->SetIsTrigger(true);
	gameObject->AddComponent(_colliderTriggerBody);


	_rightHandBone = gameObject->GetComponent<ModelRenderer>()->GetModel()->GetHierarchyNode("RightHand_IK");

	_PickUpBone = gameObject->GetComponent<ModelRenderer>()->GetModel()->GetHierarchyNode("Align");

	/* Camera Bone */
	GameObject* cameraBone = GameObject::Instantiate("MayCamera");
	cameraBone->GetTransform()->SetParent(transform);

	HierarcyNode* RootBone = gameObject->GetComponent<ModelRenderer>()->GetModel()->GetHierarchyNode("Root");
	BoneChildObject* boneChild = BoneChildObject::Create(RootBone, OffsetMat);
	cameraBone->AddComponent(boneChild);


	gameObject->SetTag(GameObjectTag::PLAYER);
	gameObject->SetLayer(GameObjectLayer::UNIT);

	_speed = 10.f;
	_OnGround = true;

	/* ============ MovementAudio ==============*/
	_audioasset[static_cast<uint>(eAudioType::WALK)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Characters_Movement_May_Idle_Footstep.ogg");
	_audioasset[static_cast<uint>(eAudioType::CROUCH)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Footstep_Default_Crouch_Hard_May.ogg");
	_audioasset[static_cast<uint>(eAudioType::LAND)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Characters_Movement_May_Land_Default_Heavy.ogg");
	_audioasset[static_cast<uint>(eAudioType::SLIDESTOP)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Character_Movement_May_Slide_Ass_Default_Stop_Smooth.ogg");
	_audioasset[static_cast<uint>(eAudioType::SLIDESTART)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Character_Movement_May_Slide_Ass_Default_Start_Smooth.ogg");
	_audioasset[static_cast<uint>(eAudioType::SLIDELOOP)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Character_Movement_May_Slide_Ass_Default_Loop_Smooth.ogg");
	_audioasset[static_cast<uint>(eAudioType::JUMP)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Character_Movement_May_Jump_Default.ogg");
	_audioasset[static_cast<uint>(eAudioType::GROUNDPOUNDFALL)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Character_Movement_May_Action_GroundPound_Default_Whoosh.ogg");
	_audioasset[static_cast<uint>(eAudioType::GROUNDPOUNDSTART)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Character_Movement_May_Action_GroundPound_Default_InAir_Movement.ogg");
	_audioasset[static_cast<uint>(eAudioType::GROUNDPOUNDLAND)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Character_Movement_May_Action_GroundPound_Default_Impact.ogg");
	_audioasset[static_cast<uint>(eAudioType::DOUBLEJUMP)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Character_Movement_May_Action_DoubleJump.ogg");
	_audioasset[static_cast<uint>(eAudioType::DASH)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_Character_Movement_May_Action_Dash.ogg");

	/* =========== Voice =============== */
	_audioasset[static_cast<uint>(eAudioType::VO_DASH)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_VO_Gameplay_Efforts_Dash_May.ogg");
	_audioasset[static_cast<uint>(eAudioType::VO_AIRDASH)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_VO_Gameplay_Efforts_Air_Dash_May.ogg");
	_audioasset[static_cast<uint>(eAudioType::VO_DOUBLEJUMP)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_VO_Gameplay_Efforts_DoubleJump_May.ogg");
	_audioasset[static_cast<uint>(eAudioType::VO_GROUNDPOUNTSTART)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_VO_Gameplay_Efforts_Ground_Pound_Start_May.ogg");
	_audioasset[static_cast<uint>(eAudioType::VO_GROUNDPOUNDJUMP)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_VO_Gameplay_Efforts_Ground_Pound_Jump_May.ogg");
	_audioasset[static_cast<uint>(eAudioType::VO_GROUNDPOUNDLAND)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_VO_Gameplay_Efforts_Ground_Pound_Land_May.ogg");
	_audioasset[static_cast<uint>(eAudioType::VO_JUMP)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_VO_Gameplay_Efforts_Jump_May.ogg");
	_audioasset[static_cast<uint>(eAudioType::VO_WALLGRAB)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_VO_Gameplay_Efforts_Wall_Grab_May.ogg");
	_audioasset[static_cast<uint>(eAudioType::VO_WALLJUMP)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\May\\Play_VO_Gameplay_Efforts_Wall_Jump_Away_May.ogg");


	_movementEffectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_movementEffectPlayer);

	_voiceEffectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_voiceEffectPlayer);



	{
		_leftFootDustFX = GameObject::Instantiate("mayLeftFootDustFX");
		DustGenerate* DustFX = new DustGenerate(transform);
		_leftFootDustFX->AddComponent(DustFX);

		_rightFootDustFX = GameObject::Instantiate("mayRightFootDustFX");
		DustFX = new DustGenerate(transform);
		_rightFootDustFX->AddComponent(DustFX);
	}

}


void May::FixedUpdate(float fElapsedTime) noexcept
{
	/*std::string temp = std::to_string(static_cast<int>(_state)) + "\n";
	OutputDebugStringA(temp.c_str());*/

	if (OptionPopup::This()->IsShow())
		return;

	if (_isDirtyState)
		return;

	if (!_isDead && !_isDeadEnd)
	{
		_inputHandler->FixedUpdate(fElapsedTime);
	}

	uint8 dirInput{ dirInput = static_cast<PlayerInput*>(_inputHandler)->GetDirInput() };
	uint8 actionInput{ actionInput = static_cast<PlayerInput*>(_inputHandler)->GetActionInput() };
	if (_isDead)
	{
		dirInput = 0;
		actionInput = 0;
	}

	float angle = static_cast<PlayerInput*>(_inputHandler)->GetAngle();

	if (dirInput
		&& !_isDash
		&& !_isInteraction
		&& !_isGroundPound
		&& !_isWallSlide
		&& !_isSliding)
		SetDirection(fElapsedTime);

	if (static_cast<PlayerInput*>(_inputHandler)->GetActionState(Actor::State::JUMP)
		|| static_cast<PlayerInput*>(_inputHandler)->GetActionState(Actor::State::JUMPFWD)
		&& !_isInteraction)
	{
		Jump();
	}
	else if (static_cast<PlayerInput*>(_inputHandler)->GetActionState(Actor::State::DASH)
		&& !_isInteraction
		&& !_isPickUp
		&& !_isWallSlide
		&& !_isCrouch)
	{
		Dash();
	}
	else if (static_cast<PlayerInput*>(_inputHandler)->GetActionState(Actor::State::CROUCHSTART)
		&& !_isInteraction
		&& !_isWallSlide
		&& !_isPickUp)
	{
		Crouch();
	}
	else if (static_cast<PlayerInput*>(_inputHandler)->GetActionState(Actor::State::INTERACTION)
		&& _interactObj != nullptr
		&& !_isWallSlide)
	{
		Interaction();
	}
	else
	{
		if (!_isJump
			&& !_isDash
			&& !_isDoubleJump
			&& !_isInteraction
			&& !_isWallSlide)
		{
			if (!dirInput)
				Idle();
			else
			{
				if (!static_cast<PlayerInput*>(_inputHandler)->GetActionState(Actor::State::RUN)
					|| _isPickUp)
					Walk();
				else
					Run();
			}
		}
	}

	if (_isSawEnter)
	{
		InteractSaw(fElapsedTime);
	}


	if (true == _isInteraction && nullptr != _interactObj)
	{
		if ("PumpValveGrabPosition" == _interactObj->GetName())
		{
			uint8 dirInput = static_cast<PlayerInput*>(_inputHandler)->GetDirInput();

			if (dirInput & (1 << 2))
			{
				if (_state == Actor::State::VALVE_ROTATE)
					return;

				_isDirtyState = true;
				_isInteraction = true;
				_dirtyState = Actor::State::VALVE_ROTATE;
			}
			else
			{
				if (_state != Actor::State::VALVE_ROTATE_IDLE)
				{
					_isDirtyState = true;
					_isInteraction = true;
					_dirtyState = Actor::State::VALVE_ROTATE_IDLE;
				}
			}
			transform->SetLocalEulerAngle(0.f, XMConvertToRadians(90.f), 0.f);
		}
		else if (_ValveInteractUseObject != nullptr && "DumbbellMayGrab" == _interactObj->GetName())
		{
			uint8 dirInput = static_cast<PlayerInput*>(_inputHandler)->GetDirInput();
			if (dirInput == 1 << 0)
			{
				if (_state == Actor::State::PUSH)
					return;

				_isDirtyState = true;
				_isInteraction = true;
				_dirtyState = Actor::State::PUSH;
			}
			else {
				if (_state != Actor::State::PUSHIDLE)
				{
					_isDirtyState = true;
					_isInteraction = true;
					_dirtyState = Actor::State::PUSHIDLE;
				}
			}
		}
	}

	if (INPUT->GetKeyDown('O'))
	{
		_isInteraction = false;

		transform->SetWorldPosition(-160.45f, -15.39063f, -129.1f);

		if (_state == Actor::State::PUSHIDLE)
		{
			_isDirtyState = true;
			_dirtyState = Actor::State::PUSHEND;
		}
	}


	if (_isDead && !_isDeadEnd)
	{
		_rigidbody->SetEnable(false);
		_renderer->SetCustomPass(3);
		Material::TList& charactormat = _renderer->GetMaterials();
		_deadLerpTime += fElapsedTime;

		float explode = CETween::Lerp(0, 1, std::min(1.0f, _deadLerpTime * 1.5f), CETween::EaseType::easeOutBack);

		for (auto& mat : charactormat)
		{
			mat.SetValue(ShaderSemantic::Type::FLOAT_EXPLODE, &explode, sizeof(float));
		}

		if (_deadLerpTime * 1.5f >= 1.f)
		{
			transform->SetWorldPosition(PlayerPrefs::This().GetFloat("SavePointX") ,
				PlayerPrefs::This().GetFloat("SavePointY"),
				PlayerPrefs::This().GetFloat("SavePointZ"));
			_deadLerpTime = 0.f;
			_isDeadEnd = true;
		}
	}
	else if (_isDead && _isDeadEnd)
	{
		Material::TList& charactormat = _renderer->GetMaterials();
		_deadLerpTime += fElapsedTime;

		float explode = CETween::Lerp(1, 0, std::min(1.0f, _deadLerpTime * 1.5f), CETween::EaseType::easeOutBack);

		for (auto& mat : charactormat)
		{
			mat.SetValue(ShaderSemantic::Type::FLOAT_EXPLODE, &explode, sizeof(float));
		}

		if (_deadLerpTime >= 1.f)
		{
			_deadLerpTime = 0.f;
			_isDeadEnd = false;
			_isDead = false;
			_renderer->SetCustomPass();
			_rigidbody->SetEnable(true);
		}

	}

	if (!_isCrouch)
	{
		_collider->SetToCapsuleGeometry(0.3f, 0.6f, XMVectorSet(0, 0.9f, 0, 1));
		_colliderTriggerBody->SetToCapsuleGeometry(0.31f, 0.65f, XMVectorSet(0, 0.9f, 0, 1));
	}
	else
	{
		_collider->SetToCapsuleGeometry(0.3f, 0.3f, XMVectorSet(0, 0.6f, 0, 1));
		_colliderTriggerBody->SetToCapsuleGeometry(0.31f, 0.35f, XMVectorSet(0, 0.6f, 0, 1));
	}


	if (_state == Actor::State::WALK)
	{
		if (0.2f <= _animator->GetCurrentProgress() && 0.3f > _animator->GetCurrentProgress())
			_leftFootDustFX->GetComponent<DustGenerate>()->SetPlay(true);
		if (0.6f <= _animator->GetCurrentProgress() && 0.7f > _animator->GetCurrentProgress())
			_rightFootDustFX->GetComponent<DustGenerate>()->SetPlay(true);
	}

}

void May::Update(float fElapsedTime) noexcept
{
	if (OptionPopup::This()->IsShow())
		return;

	XMVECTOR forward = XMVector3Normalize(transform->GetForwardNoY()) * fElapsedTime * _speed;

	uint8 dirInput = static_cast<PlayerInput*>(_inputHandler)->GetDirInput();
	switch (_state)
	{
	case Actor::State::WALK:				FALLTHROUGH
	case Actor::State::WALKSTART:			FALLTHROUGH
	case Actor::State::JUMPLANDJOG:			FALLTHROUGH
	case Actor::State::JUMPLANDWALKSTART:	FALLTHROUGH
	case Actor::State::DASHENDJOG:			FALLTHROUGH
	case Actor::State::CROUCHEXITMOVE:
		transform->Translate(forward);
		break;
	case Actor::State::CROUCH:				FALLTHROUGH
	case Actor::State::CROUCHSTART:
		forward /= 2.f;
		transform->Translate(forward);
		break;
	case Actor::State::RUN:
		forward *= 2.f;
		transform->Translate(forward);
		break;
	case Actor::State::JUMP:				FALLTHROUGH
	case Actor::State::JUMPFWD:				FALLTHROUGH
	case Actor::State::SECOND_JUMP:
		if (dirInput != 0)
		{
			transform->Translate(forward);
		}
		break;
	case Actor::State::INTERACTIONSTART:		FALLTHROUGH
	case Actor::State::INTERACTION:				FALLTHROUGH
	case Actor::State::INTERACTIONIDLE:			FALLTHROUGH
	case Actor::State::INTERACTIONEND:			FALLTHROUGH
	case Actor::State::PICKUP:					FALLTHROUGH
	case Actor::State::SAWENTER:				FALLTHROUGH
	case Actor::State::SAWIDLE:					FALLTHROUGH
	case Actor::State::WALLSLIDERENTER:			FALLTHROUGH
	case Actor::State::WALLSLIDEIDLE:			FALLTHROUGH
	case Actor::State::WALLSLIDEJUMP:			FALLTHROUGH
	case Actor::State::VALVE_ROTATE:			FALLTHROUGH
	case Actor::State::VALVE_ROTATE_IDLE:		FALLTHROUGH
	case Actor::State::PUSHENTER:				FALLTHROUGH
	case Actor::State::PUSHIDLE:				FALLTHROUGH
	case Actor::State::PUSH:					FALLTHROUGH
	case Actor::State::PUSHEND:					FALLTHROUGH
	case Actor::State::LEVERLEFT:				FALLTHROUGH
	case Actor::State::LEVERRIGHT:
		UpdateCheckEvent(fElapsedTime);
		break;
	case Actor::State::SUCTION:
		Suction(fElapsedTime);
		break;
	}
}

void May::LateUpdate(float fElapsedTime) noexcept
{
	if (OptionPopup::This()->IsShow())
		return;

	XMVECTOR forward = XMVector3Normalize(transform->GetForwardNoY());

	uint8 dirInput = static_cast<PlayerInput*>(_inputHandler)->GetDirInput();

	if (_isDirtyState)
	{
		_isDirtyState = false;
		_state = _dirtyState;
		/*_rigidbody->SetAngularDrag(1.f);
		_rigidbody->SetDrag(1.f);*/
		switch (_dirtyState)
		{
		case Actor::State::IDLE:
			if (_isPickUp)
			{
				_animator->Play("May.ao|May_Bhv_PickUp_Mh"
					, true);
			}
			else
			{
				_isGroundPound = false;
				_animator->Play("May.ao|May_mh", true);
			}
			_animator->SetPlaySpeed(1.f);
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);

			break;
		case Actor::State::WALKSTART:
			_animator->Play("May.ao|May_Loco_Jog_Start"
				, false
				, std::bind(&May::OnEndWalk, this));
			_animator->SetPlaySpeed(1.f);
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::JUMPLANDWALKSTART:
			_animator->Play("May.ao|May_Loco_Jog_Start_InMotion"
				, false
				, std::bind(&May::OnEndWalk, this));

			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::WALK:
			if (_isPickUp)
			{
				_animator->Play("May.ao|May_Bhv_PickUp_Loco_Jog"
					, true);
			}
			else
			{
				_animator->Play("May.ao|May_Loco_Jog"
					, true);
			}
			_animator->SetPlaySpeed(1.f);
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::WALK)],0.025f,true);
			break;
		case Actor::State::WALKEND:
			if (_isPickUp)
			{
				_animator->Play("May.ao|May_Bhv_PickUp_Loco_Jog_Stop"
					, false
					, std::bind(&May::OnEndWalk, this));
			}
			else
			{
				_animator->Play("May.ao|May_Loco_Jog_Stop"
					, false
					, std::bind(&May::OnEndWalk, this));
				_animator->SetPlaySpeed(1.3f);
			}
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);

			_movementEffectPlayer->Stop();

			break;
		case Actor::State::RUN:
			_animator->Play("May.ao|May_Loco_Sprint"
				, true
				, std::bind(&May::OnEndWalk, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::ATTACK:
			break;
		case Actor::State::DASH:
			_rigidbody->AddForce(forward * 2000.f);

			if (true == _OnGround)
			{
				_animator->SetPlaySpeed(1.5f);
				_animator->Play("May.ao|May_Trav_Roll_Start"
					, false
					, std::bind(&May::OnEndDash, this)
					, 0.9f
					, std::bind(&May::OnTriggerDash, this));

				_voiceEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::VO_DASH)],1.f);

			}
			else
			{
				_animator->SetPlaySpeed(2.f);
				_rigidbody->SetFreezePositionY(true);
				_rigidbody->SetUseGravity(false);
				_animator->Play("May.ao|May_Trav_AirDash_Start"
					, false
					, std::bind(&May::OnEndDash, this)
					, 0.4f
					, std::bind(&May::OnTriggerDash, this));

				_voiceEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::VO_AIRDASH)], 1.f);
			}

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::DASH)],0.0025f);

			break;
		case Actor::State::DASHEND:
			_animator->Play("May.ao|May_Trav_Roll_Stop"
				, false
				, std::bind(&May::OnEndDashEnd, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::DASHENDJOG:
			_animator->Play("May.ao|May_Trav_Roll_Jog"
				, false
				, std::bind(&May::OnEndDashEnd, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::JUMP:
			_rigidbody->AddForce(XMVectorSet(0.f, 1.f, 0.f, 0.f) * 1500.f);
			if (_isPickUp)
			{
				_animator->Play("May.ao|May_Bhv_PickUp_Trav_Jump_Start"
					, false
					, std::bind(&May::OnEndJump, this));
			}
			else
			{
				_animator->Play("May.ao|May_Trav_Jump_Start",
					false,
					std::bind(&May::OnEndJump, this));
			}
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);

			_voiceEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::VO_JUMP)], 1.f);

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::JUMP)],0.0025f);

			break;
		case Actor::State::JUMPFWD:
			_rigidbody->AddForce(XMVectorSet(0.f, 1.f, 0.f, 0.f) * 1500.f);
			if (_isPickUp)
			{
				_animator->Play("May.ao|May_Bhv_PickUp_Trav_Jump_Start"
					, false
					, std::bind(&May::OnEndJump, this));
			}
			else
			{
				_animator->Play("May.ao|May_Trav_Jump_Start_Fwd"
					, false
					, std::bind(&May::OnEndJump, this));
			}
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);

			_voiceEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::VO_JUMP)], 1.f);

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::JUMP)],0.0025f);

			break;
		case Actor::State::JUMPLAND:
			_rigidbody->SetFreezePositionY(false);
			_rigidbody->SetUseGravity(true);
			if (_isPickUp)
			{
				_animator->Play("May.ao|May_Bhv_PickUp_Trav_Jump_Land"
					, false
					, std::bind(&May::OnEndLand, this));
			}
			else
			{
				_animator->Play("May.ao|May_Trav_Jump_Land"
					, false
					, std::bind(&May::OnEndLand, this));
			}
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);


			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::LAND)],0.005f);

			break;
		case Actor::State::JUMPLANDJOG:
			_rigidbody->SetFreezePositionY(false);
			_rigidbody->SetUseGravity(true);
			if (_isPickUp)
			{
				_animator->Play("May.ao|May_Bhv_PickUp_Trav_Jump_Land_Jog"
					, false
					, std::bind(&May::OnEndLand, this));
			}
			else
			{
				_animator->Play("May.ao|May_Trav_Jump_Land_Jog"
					, false
					, std::bind(&May::OnEndLand, this));
			}
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::LAND)],0.005f);

			break;
		case Actor::State::SECOND_JUMP:
			_rigidbody->SetFreezePositionY(false);
			_rigidbody->SetUseGravity(true);
			_rigidbody->AddForce(XMVectorSet(0.f, 1.f, 0.f, 0.f) * 1000.f);
			_animator->Play("May.ao|May_Trav_Doublejump"
				, false
				, std::bind(&May::OnEndJump, this));

			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);

			if (_isWallSlide)
			{
				transform->Rotate(0, XMConvertToRadians(180.f), 0);
				_isWallSlide = false;
			}

			_voiceEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::VO_DOUBLEJUMP)], 1.f);


			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::DOUBLEJUMP)],0.0005f);

			break;
		case Actor::State::FALLING:
			if (_isPickUp)
			{
				_animator->Play("May.ao|May_Bhv_PickUp_Trav_Jump_Falling", true);
			}
			else
			{
				_animator->Play("May.ao|May_Trav_Jump_Falling", true);
			}
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::INTERACTIONSTART:
			if (nullptr != _interactObj)
				LateUpdateInteractStart(_interactObj->GetName());
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::INTERACTION:
			if (nullptr != _interactObj)
				LateUpdateInteract(_interactObj->GetName());
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::INTERACTIONIDLE:
			if (nullptr != _interactObj)
				LateUpdateInteractIdle(_interactObj->GetName());
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::INTERACTIONEND:
			if (nullptr != _interactObj)
				LateUpdateInteractEnd(_interactObj->GetName());
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;

		case Actor::State::CROUCHENTER:
			_animator->Play("May.ao|May_Trav_Crouch_Enter_MH"
				, false
				, std::bind(&May::OnEndCrouch, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::CROUCHSTART:
			_animator->SetPlaySpeed(2.f);
			_animator->Play("May.ao|May_Trav_Crouch_Walk_Start"
				, false
				, std::bind(&May::OnEndCrouch, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::CROUCHIDLE:
			_animator->SetPlaySpeed(1.f);
			_animator->Play("May.ao|May_Trav_Crouch_MH"
				, true);
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::CROUCH:
			_animator->SetPlaySpeed(1.f);
			_animator->Play("May.ao|May_Trav_Crouch_Walk"
				, true);
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::CROUCH)],0.005f,true);

			break;
		case Actor::State::CROUCHEND:
			_animator->SetPlaySpeed(2.f);
			_animator->Play("May.ao|May_Trav_Crouch_Walk_Stop"
				, false
				, std::bind(&May::OnEndCrouch, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::CROUCHEXIT:
			_animator->Play("May.ao|May_Trav_Crouch_Exit_MH"
				, false
				, std::bind(&May::OnEndCrouch, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::CROUCHEXITMOVE:
			_animator->Play("May.ao|May_Trav_Crouch_Exit_Movement"
				, false
				, std::bind(&May::OnEndCrouch, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::GROUNDPOUNDSTART:
			_rigidbody->SetUseGravity(false);
			_animator->SetPlaySpeed(1.5f);
			_animator->Play("May.ao|May_Bhv_GroundPound_Start"
				, false
				, std::bind(&May::OnEndGroundPound, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);

			_voiceEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::VO_GROUNDPOUNTSTART)], 1.f);

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::GROUNDPOUNDSTART)],0.005f);

			break;
		case Actor::State::GROUNDPOUNDFALLING:
			_rigidbody->SetFreezePositionY(false);
			_rigidbody->SetUseGravity(true);
			_rigidbody->AddForce(XMVectorSet(0.f, -1.f, 0.f, 0.f) * 3000.f);
			_animator->Play("May.ao|May_Bhv_GroundPound_Falling"
				, true);
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);

			_voiceEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::VO_GROUNDPOUNDJUMP)], 1.f);

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::GROUNDPOUNDFALL)],0.005f);
			break;
		case Actor::State::GROUNDPOUNDLAND:
			_camera->ShakeCamera(0.35f, 0.3f);
			_animator->Play("May.ao|May_Bhv_GroundPound_Land"
				, false
				, std::bind(&May::OnEndGroundPound, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);

			_voiceEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::VO_GROUNDPOUNDLAND)], 1.f);

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::GROUNDPOUNDLAND)],0.01f);
			break;
		case Actor::State::GROUNDPOUNDEXIT:
			_animator->SetPlaySpeed(2.f);
			_animator->Play("May.ao|May_Bhv_GroundPound_Land_Exit"
				, false
				, std::bind(&May::OnEndGroundPound, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			break;
		case Actor::State::PICKUP:
			_animator->Play("May.ao|May_Bhv_PickUp"
				, false
				, std::bind(&May::OnEndPickUp, this));
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			_animator->SetPlaySpeed(1.f);
			break;
		case Actor::State::PICKENTER:
			_animator->Play("May.ao|May_Bhv_PickUp_FuseCrab_PutInSocket"
				, false
				, std::bind(&May::OnEndPickUp, this)
				, 0.7f
				, std::bind(&FuseCrab::PlayInsertSound, _oldFuseCrab->GetComponent<FuseCrab>()));
			_animator->SetPlaySpeed(1.f);
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetDrag(1.f);
			transform->SetLocalEulerAngle(0.f, 0.f, 0.f);
			break;
		case Actor::State::WALLSLIDERENTER:
			/* 대쉬상태일때 */
			if (_isDash)
			{
				_animator->Play("May.ao|May_Trav_WallSlide_Enter_FromDash"
					, false
					, std::bind(&May::OnEndWallSlide, this)
					, 0.f
					, std::bind(&May::WallJump, this));
			}
			/* 점프 , 이단점프 일때*/
			else
			{
				_animator->Play("May.ao|May_Trav_WallSlide_Enter"
					, false
					, std::bind(&May::OnEndWallSlide, this)
					, 0.f
					, std::bind(&May::WallJump, this));
			}

			_rigidbody->SetDrag(30.f);
			_rigidbody->SetAngularDrag(30.f);

			_animator->SetPlaySpeed(1.f);

			_voiceEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::VO_WALLGRAB)], 1.f);

			break;
		case Actor::State::WALLSLIDEIDLE:
			_animator->Play("May.ao|May_Trav_WallSlide_MH"
				, true);

			if (_interactObj)
			{
				_rigidbody->SetDrag(30.f);
				_rigidbody->SetAngularDrag(30.f);
			}
			_animator->SetPlaySpeed(1.f);
			break;
		case Actor::State::WALLSLIDEJUMP:
		{
			if (dirInput & 1 << 0)
			{
				_animator->Play("May.ao|May_Trav_WallSlide_Jump_Vertical"
					, false
					, std::bind(&May::OnEndWallSlide, this));
			}
			else
			{
				_animator->Play("May.ao|May_Trav_WallSlide_Jump"
					, false
					, std::bind(&May::OnEndWallSlide, this));
			}
			_rigidbody->SetDrag(1.f);
			_rigidbody->SetAngularDrag(1.f);

			_animator->SetPlaySpeed(1.f);

			transform->SetParent(nullptr);

			_rigidbody->SetEnable(true);

			XMVECTOR vecWallJumpdir = XMVector3Normalize(XMVectorSet(-XMVectorGetX(transform->GetForward()), 1.f, -XMVectorGetZ(transform->GetForward()), 0));

			_rigidbody->AddForce(vecWallJumpdir * 1500.f * _wallJumpPower);


			_voiceEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::VO_WALLJUMP)], 1.f);


			if ("WareHouse_03" == SCENEMANAGER->GetSceneID())
			{
				FollowingCamera* followCam = _camera->GetGameObject()->GetComponent<FollowingCamera>();
				followCam->ExitCameraEvent();
			}

			break;
		}
		case Actor::State::SAWENTER:
			_animator->Play("May.ao|May_EV_Shed_Awakening_Saw_Enter"
				, false
				, std::bind(&May::OnEndSawJump, this));
			_animator->SetPlaySpeed(0.f);
			_rigidbody->SetDrag(1.f);
			_rigidbody->SetAngularDrag(1.f);

			break;
		case Actor::State::SAWIDLE:
			_animator->Play("May.ao|May_EV_Shed_Awakening_Saw_Mh"
				, true);
			_animator->SetPlaySpeed(1.f);
			_rigidbody->SetDrag(1.f);
			_rigidbody->SetAngularDrag(1.f);
			break;
		case Actor::State::SAWSUCCSESS:
			_animator->Play("May.ao|May_EV_Shed_Awakening_Saw_Succsess"
				, false
				, nullptr
				, 0.79f
				, std::bind(&May::OnEndSawJump, this));
			_rigidbody->SetDrag(1.f);
			_rigidbody->SetAngularDrag(1.f);
			_animator->SetPlaySpeed(1.f);
			break;
		case Actor::State::SLIDEENTER:
			_animator->Play("May.ao|May_Trav_Slide_Enter_Falling"
				, false
				, std::bind(&May::OnEndSlide, this));
			_rigidbody->SetDrag(0);
			_rigidbody->AddForce(forward * 10.f, PxForceMode::eVELOCITY_CHANGE);

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::SLIDESTART)],0.005f);

			break;
		case Actor::State::SLIDEIDLE:
			_animator->Play("May.ao|May_Trav_Slide_Neutral"
				, true);
			_rigidbody->SetDrag(0);

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::SLIDELOOP)],0.005f,true);

			break;
		case Actor::State::SLIDEEXIT:
			_animator->Play("May.ao|May_Trav_Slide_Exit_Falling"
				, false
				, std::bind(&May::OnEndSlide, this));

			XMVECTOR fwdvector = XMVector3Normalize(XMVectorSetY(transform->GetForwardNoY(), 1.f));

			_rigidbody->SetDrag(1.f);
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->AddForce(fwdvector * 1500.f);

			_movementEffectPlayer->Play(_audioasset[static_cast<UINT>(eAudioType::SLIDESTOP)],0.005f);

			break;
		case Actor::State::VALVE_ROTATE_IDLE:
			_animator->Play("May.ao|May_Bhv_Generic_Valve_Rotate_Mh"
				, true);
			_rigidbody->SetDrag(1.f);
			_rigidbody->SetAngularDrag(1.f);

			break;
		case Actor::State::VALVE_ROTATE:
			_animator->Play("May.ao|May_Bhv_Generic_Valve_Rotate_R_Short"
				, true);
			_rigidbody->SetDrag(1.f);
			_rigidbody->SetAngularDrag(1.f);
			break;
		case Actor::State::PUSHENTER:
			_animator->Play("May.ao|May_Bhv_Push_Enter"
				, false
				, std::bind(&May::OnEndPush, this));
			_rigidbody->SetDrag(1.f);
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetFreezeRotationY(true);
			break;
		case Actor::State::PUSHIDLE:
			_animator->Play("May.ao|May_Bhv_Push_MH"
				, true);
			_rigidbody->SetDrag(1.f);
			_rigidbody->SetAngularDrag(1.f);
			break;
		case Actor::State::PUSH:
			_animator->Play("May.ao|May_Bhv_Push_Fwd"
				, true
				, std::bind(&May::OnEndPush, this));
			_rigidbody->SetDrag(1.f);
			_rigidbody->SetAngularDrag(1.f);
			break;
		case Actor::State::PUSHEND:
			_animator->Play("May.ao|May_Bhv_Push_Exit"
				, false
				, std::bind(&May::OnEndPush, this));
			_rigidbody->SetDrag(1.f);
			_rigidbody->SetAngularDrag(1.f);
			_rigidbody->SetFreezeRotationY(false);
			break;
		case Actor::State::LEVERLEFT:
			_animator->Play("May.ao|May_Bhv_Lever_Left"
				, false
				, std::bind(&May::OnEndLever, this)
				, 0.3f
				, std::bind(&May::InteractLeverReverse, this));
			_animator->SetPlaySpeed(0);
			break;
		case Actor::State::LEVERRIGHT:
			_animator->Play("May.ao|May_Bhv_Lever_Right"
				, false
				, std::bind(&May::OnEndLever, this)
				, 0.3f
				, std::bind(&May::InteractLeverReverse, this));
			_animator->SetPlaySpeed(0);
			break;
		case Actor::State::GETHIT:
			break;
		case Actor::State::DIE:
			break;
		case Actor::State::END:
			break;
		}
	}


}


void May::Idle(void) noexcept
{
	/* 이어져야할 애니메이션들 */
	if (_state == State::ATTACK
		|| _state == State::IDLE
		|| _state == State::JUMPLAND
		|| _state == State::WALKEND
		|| _state == State::DASHEND
		|| _state == State::INTERACTIONSTART
		|| _state == State::INTERACTION
		|| _state == State::CROUCHENTER
		|| _state == State::CROUCHIDLE
		|| _state == State::CROUCHEND
		|| _state == State::CROUCHEXIT
		|| _state == State::GROUNDPOUNDLAND
		|| _state == State::GROUNDPOUNDEXIT
		|| _state == State::PICKUP
		|| _state == State::SAWIDLE
		|| _state == State::SAWSUCCSESS
		|| _state == State::SAWENTER
		|| _state == State::SLIDEENTER
		|| _state == State::SLIDEIDLE
		|| _state == State::LEVERLEFT
		|| _state == State::LEVERRIGHT
		|| _state == State::SUCTION
		)
		return;



	switch (_state)
	{
	case State::WALK: FALLTHROUGH
	case State::RUN:
		if (_isPickUp)
		{
			_dirtyState = State::IDLE;
			_isDirtyState = true;
		}
		else
		{
			_dirtyState = State::WALKEND;
			_isDirtyState = true;
		}

		break;

	case State::CROUCH:
		_dirtyState = State::CROUCHEND;
		_isDirtyState = true;

		break;
	default:
		if (_state == State::INTERACTIONEND)
		{
			GameObject::FindObjectByName("Cinematic0")->GetComponent<WareHouseCinematic>()->SetOn(0);
		}
		_dirtyState = State::IDLE;
		_isDirtyState = true;

		break;
	}
}

void May::Walk(void) noexcept
{
	/*
			끝날때 바인드된 애니메이션있으면 반드시 필요
		*/
	if (_state == State::ATTACK
		|| _state == State::WALK
		|| _state == State::WALKSTART
		|| _state == State::JUMPLANDWALKSTART
		|| _state == State::JUMP
		|| _state == State::JUMPLANDJOG
		|| _state == State::DASHENDJOG
		|| _state == State::CROUCH
		|| _state == State::CROUCHSTART
		|| _state == State::CROUCHEXITMOVE
		|| _state == State::SAWIDLE
		|| _state == State::SUCTION
		|| _isGroundPound
		)
		return;

	/*
	끝날때 바인드된 애니메이션이없거나
	사이에 들어가는 애니메이션에서 움직임이 필요할때 사용
	*/
	switch (_state)
	{
	case State::IDLE:		FALLTHROUGH
	case State::WALKEND:	FALLTHROUGH
	case State::JUMPLAND:	FALLTHROUGH
	case State::DASHEND:
		if (_isPickUp)
		{
			_dirtyState = State::WALK;
			_isDirtyState = true;
		}
		else
		{
			_dirtyState = State::WALKSTART;
			_isDirtyState = true;
		}
		break;
	case State::JUMPLANDJOG:
		_dirtyState = State::JUMPLANDWALKSTART;
		_isDirtyState = true;

		break;
	case State::CROUCHIDLE:
		_dirtyState = State::CROUCHSTART;
		_isDirtyState = true;

		break;
	}

}

void May::Run(void) noexcept
{
	if (_state == State::ATTACK
		|| _state == State::JUMP
		|| _state == State::RUN
		|| _state == State::DASH
		|| _state == State::SAWIDLE
		|| _state == State::SUCTION)
		return;

	_isCrouch = false;


	_isDirtyState = true;
	_dirtyState = State::RUN;
}

void May::Die(void) noexcept
{
}

void May::Dash(void) noexcept
{
	if (_state == State::ATTACK
		|| _state == State::DASH
		|| _state == State::SAWIDLE
		|| _isDash
		|| _DashCount <= 0
		|| _isGroundPound)
		return;

	_DashCount--;
	_isDash = true;
	_isDirtyState = true;
	_dirtyState = State::DASH;
}

void May::Attack(void) noexcept
{
}
void May::GetHit(float damage, int8 attacker) noexcept
{
}

void May::Jump(void) noexcept
{
	if (_state == State::ATTACK
		|| _state == State::SECOND_JUMP
		|| _state == State::INTERACTIONSTART
		|| _state == State::SAWIDLE
		|| _JumpCount <= 0
		|| _isGroundPound
		|| (_state == State::SUCTION && "WareHouse_01" == SCENEMANAGER->GetSceneID()))
		return;

	if ((_state == State::DASH
		|| _state == State::DASHEND
		|| _state == State::DASHENDJOG)
		&& _OnGround)
	{
		_DashCount = 1;
		_isDash = false;
	}

	if (_state == State::CROUCH
		|| _state == State::CROUCHIDLE
		|| _state == State::CROUCHSTART
		|| _state == State::CROUCHEXIT
		|| _state == State::CROUCHEND)
	{
		_isCrouch = false;
	}

	_isDirtyState = true;
	_OnGround = false;
	_JumpCount--;
	if (false == _isJump)
	{
		if (static_cast<PlayerInput*>(_inputHandler)->GetActionState(Actor::State::JUMP))
		{
			if (_isSliding)
			{
				_dirtyState = State::SLIDEEXIT;
			}
			else if (_isWallSlide)
			{
				_dirtyState = Actor::State::WALLSLIDEJUMP;
			}
			else
			{
				_dirtyState = State::JUMP;
			}
		}
		else if (static_cast<PlayerInput*>(_inputHandler)->GetActionState(Actor::State::JUMPFWD))
		{
			if (_isWallSlide)
			{
				_dirtyState = Actor::State::WALLSLIDEJUMP;
			}
			else
			{
				_dirtyState = State::JUMPFWD;

			}
		}

		_isJump = true;
	}
	else
	{
		if (!_isPickUp)
		{
			_dirtyState = State::SECOND_JUMP;
			_isDoubleJump = true;
			_isJump = false;
		}
	}


}

void May::DoubleJump(void) noexcept
{
}

void May::Falling(void) noexcept
{
	if (_state == State::ATTACK ||
		_state == State::SUCTION ||
		_state == State::FALLING)
		return;
	_isDirtyState = true;
	_dirtyState = State::FALLING;
}

void May::SetDirection(float deltatime) noexcept
{
	if (_state == Actor::State::ATTACK)
		return;

	/*
		uint8 INPUT_FRONT		= 1 << 0;
		uint8 INPUT_LEFT		= 1 << 1;
		uint8 INPUT_RIGHT		= 1 << 2;
		uint8 INPUT_BACKWARD	= 1 << 3;
	*/

	uint8 dir = static_cast<PlayerInput*>(_inputHandler)->GetDirInput();

	/* 캐릭터와 카메라 사이의 각도 */
	float theta = CE_MATH::CalcDotTheta(_camera->GetTransform()->GetForwardNoY(), transform->GetForwardNoY(), true);


	float leftrightdot = CE_MATH::isLeft(XMVectorSetY(_camera->GetTransform()->GetWorldPosition(), 0), transform->GetWorldMatrix());

	float turnspeed = 20.f;

	if (dir & (1 << 0))
	{

		if (theta > 10.f)
		{
			/* 오른쪽 */
			if (leftrightdot < 0)
			{
				transform->Rotate(0, -deltatime * turnspeed, 0);
			}
			/* 왼쪽 */
			else
			{
				transform->Rotate(0, deltatime * turnspeed, 0);
			}
		}
		else
		{
			if (leftrightdot < 0)
			{
				transform->Rotate(0, -deltatime, 0);
			}
			else
			{
				transform->Rotate(0, deltatime, 0);
			}
		}

	}
	else if (dir & 1 << 3)
	{
		if (theta < 170.f)
		{
			/* 오른쪽 */
			if (leftrightdot < 0)
			{
				transform->Rotate(0, deltatime * turnspeed, 0);
			}
			/* 왼쪽 */
			else
			{
				transform->Rotate(0, -deltatime * turnspeed, 0);
			}
		}
		else
		{
			if (leftrightdot < 0)
			{
				transform->Rotate(0, deltatime, 0);
			}
			else
			{
				transform->Rotate(0, -deltatime, 0);
			}
		}

	}

	/* LEFT */
	if (dir & 1 << 1)
	{
		/* 카메라의 -right  = 왼쪽방향 과 캐릭터의 forward 방향을 계산해서 왼쪽으로 가게함*/
		float radcos = CE_MATH::CalcDotTheta(transform->GetForwardNoY(), XMVectorSetY(-_camera->GetTransform()->GetRight(), 0), true);
		if (radcos > 10.f)
		{
			if (theta < 90)
				transform->Rotate(0, deltatime * -turnspeed, 0);
			else
				transform->Rotate(0, deltatime * turnspeed, 0);
		}
		else
		{
			if (theta < 90)
				transform->Rotate(0, -deltatime, 0);
			else
				transform->Rotate(0, deltatime, 0);

		}
	}
	/* RIGHT */
	else if (dir & 1 << 2)
	{
		float radcos = CE_MATH::CalcDotTheta(transform->GetForwardNoY(), XMVectorSetY(_camera->GetTransform()->GetRight(), 0), true);

		if (radcos > 10.f)
		{
			if (theta < 90)
				transform->Rotate(0, deltatime * turnspeed, 0);
			else
				transform->Rotate(0, deltatime * -turnspeed, 0);
		}
		else
		{
			if (theta < 90)
				transform->Rotate(0, deltatime, 0);
			else
				transform->Rotate(0, -deltatime, 0);

		}
		/* 카메라의 right =  오른쪽방향 */
	}

}

void May::CalcDirection(float angle, float deltatime) noexcept
{
	if (_state == Actor::State::ATTACK)
		return;

	/*
		uint8 INPUT_FRONT		= 1 << 0;
		uint8 INPUT_LEFT		= 1 << 1;
		uint8 INPUT_RIGHT		= 1 << 2;
		uint8 INPUT_BACKWARD	= 1 << 3;
	*/

	XMFLOAT3 euler;
	XMStoreFloat3(&euler, transform->GetLocalEulerAngle());

	euler.y = CE_MATH::ToDegree(euler.y);

	if (CE_MATH::equl_f(0, euler.y))
		euler.y = 360;

	if (abs(euler.y - angle) < 10.0f)
		transform->SetLocalEulerAngle(0, CE_MATH::ToRadian(angle), 0);
	else
	{
		if (CE_MATH::equl_f(0, euler.y) || CE_MATH::equl_f(360, euler.y))
		{
			if (abs(360 - angle) > angle)
				euler.y = 0;
			else
				euler.y = 360;
		}
		else if (CE_MATH::equl_f(0, angle) || CE_MATH::equl_f(360, angle))
		{
			if (abs(360 - euler.y) > euler.y)
				angle = 0;
			else
				angle = 360;
		}

		if (euler.y > 270.f && angle < 90.f)
		{
			transform->Rotate(0, 10 * deltatime, 0);
		}
		else if (euler.y < 90.f && angle >= 270.f)
		{
			transform->Rotate(0, -10 * deltatime, 0);
		}
		else
		{
			if (euler.y > angle)
				transform->Rotate(0, -10 * deltatime, 0);
			else
				transform->Rotate(0, 10 * deltatime, 0);
		}
	}
}

void May::Interaction(void) noexcept
{
	if (_interactObj->GetName() == "PowerSwitch")
	{
		switch (_state)
		{
		case State::IDLE:
			if (5 >= std::fabs(XMVectorGetX(XMVector3Length(_interactObj->GetTransform()->GetWorldPosition() - transform->GetWorldPosition()))))
			{
				_dirtyState = Actor::State::INTERACTIONSTART;
				_isDirtyState = true;
			}
			break;
		case State::INTERACTIONIDLE:
			_dirtyState = Actor::State::FALLING;
			_isDirtyState = true;
			break;
		}
	}
	else if (_interactObj->GetName() == "FuseCrab0"
		|| _interactObj->GetName() == "FuseCrab1"
		|| _interactObj->GetName() == "FuseCrab2"
		|| _interactObj->GetName() == "FuseCrab3")
	{
		switch (_state)
		{
		case State::IDLE:
			/* 거리가 3이내 */
			if (!_isPickUp &&
				3.f >= std::fabs(XMVectorGetX(XMVector3Length((_interactObj->GetComponent<Collider>()->GetOffsetPos() + _interactObj->GetTransform()->GetWorldPosition()) - transform->GetWorldPosition()))))
			{
				_isPickUp = true;
				_isDirtyState = true;
				_dirtyState = Actor::State::PICKUP;
				--_JumpCount;

			}
			break;
		}
	}
	else if (_interactObj->GetName() == "FusePutIn0"
		|| _interactObj->GetName() == "FusePutIn1"
		|| _interactObj->GetName() == "FusePutIn2")
	{
		switch (_state)
		{
		case State::IDLE:
			_isDirtyState = true;
			_isInteraction = true;
			_dirtyState = Actor::State::PICKENTER;
			break;
		}
	}
	else if (_interactObj->GetName() == "CircularSaw")
	{
		switch (_state)
		{
		case State::IDLE:
			_isDirtyState = true;
			_dirtyState = Actor::State::SAWENTER;
			break;
		}
	}
	else if (_interactObj->GetName() == "PumpValveGrabPosition")
	{
		switch (_state)
		{
		case State::IDLE:
			_isDirtyState = true;
			_isInteraction = true;
			_dirtyState = Actor::State::VALVE_ROTATE_IDLE;
			break;
		case State::VALVE_ROTATE_IDLE:
			_isDirtyState = true;
			_isInteraction = false;
			_dirtyState = Actor::State::IDLE;
			break;
		}
	}
	else if (_interactObj->GetName() == "DumbbellMayGrab")
	{
		switch (_state)
		{
		case State::IDLE:
			_isDirtyState = true;
			_isInteraction = true;
			_dirtyState = Actor::State::PUSHENTER;
			break;
		case State::PUSHIDLE:
			_isDirtyState = true;
			_isInteraction = false;
			_dirtyState = Actor::State::IDLE;
			break;
		}
	}
	else if (_interactObj->GetName() == "LeverPosition" || _interactObj->GetName() == "Lever0Position" || _interactObj->GetName() == "Lever1Position")
	{
		Lever* lever = nullptr;

		if (_interactObj->GetName() == "LeverPosition")
			lever = GameObject::FindObjectByName("Lever")->GetComponent<Lever>();
		else if (_interactObj->GetName() == "Lever0Position")
			lever = GameObject::FindObjectByName("Lever0")->GetComponent<Lever>();
		else if (_interactObj->GetName() == "Lever1Position")
			lever = GameObject::FindObjectByName("Lever1")->GetComponent<Lever>();

		if (lever == nullptr)
		{
			CE_ASSERT("ckswns", "Lever in May GetComponent가 nullptr입니다.");
		}

		switch (_state)
		{
		case State::IDLE:
			_isDirtyState = true;
			if (!lever->GetDirection())
			{
				_dirtyState = Actor::State::LEVERLEFT;
			}
			else
			{
				_dirtyState = Actor::State::LEVERRIGHT;
			}
			break;
		}
	}
}

void May::Crouch(void) noexcept
{
	switch (_state)
	{
	case State::IDLE:
		_dirtyState = State::CROUCHENTER;
		_isDirtyState = true;
		_isCrouch = true;
		break;
	case State::CROUCHIDLE:
		_dirtyState = State::CROUCHEXIT;
		_isDirtyState = true;
		_isCrouch = false;
		break;
	case State::CROUCH:			FALLTHROUGH
	case State::CROUCHSTART:
		_dirtyState = State::CROUCHEXITMOVE;
		_isCrouch = false;
		_isDirtyState = true;
		break;
	case State::JUMP:			FALLTHROUGH
	case State::JUMPFWD:		FALLTHROUGH
	case State::SECOND_JUMP:	FALLTHROUGH
	case State::FALLING:		FALLTHROUGH
	case State::DASH:
		if (_OnGround)
		{
			break;
		}

		_dirtyState = State::GROUNDPOUNDSTART;
		_isDirtyState = true;
		_isGroundPound = true;
		break;
	}

}

void May::Suction(float fElapsedTime) noexcept
{
	if (nullptr == _interactObj)
		return;


	XMVECTOR suctionHozePosition = XMVectorSet(0, 0, 0, 0);
	XMVECTOR myPosition = XMVectorSet(0, 0, 0, 0);

	if ("WareHouse_01" == SCENEMANAGER->GetSceneID() ||
		"WareHouse_03" == SCENEMANAGER->GetSceneID())
	{
		_rigidbody->SetUseGravity(false);
		suctionHozePosition = _interactObj->GetTransform()->GetWorldMatrix().r[3];
		myPosition = transform->GetWorldMatrix().r[3];

		float x, y, z, size;

		x = CETween::Lerp(XMVectorGetX(myPosition), XMVectorGetX(suctionHozePosition), std::min(1.f, _lerpTime), CETween::EaseType::easeInExpo);
		y = CETween::Lerp(XMVectorGetY(myPosition), XMVectorGetY(suctionHozePosition) + 0.2f, std::min(1.f, _lerpTime), CETween::EaseType::easeInExpo);
		z = CETween::Lerp(XMVectorGetZ(myPosition), XMVectorGetZ(suctionHozePosition), std::min(1.f, _lerpTime), CETween::EaseType::easeInExpo);

		size = CETween::Lerp(1, 0, std::min(1.f, _lerpTime), CETween::EaseType::easeInExpo);

		_lerpTime += fElapsedTime;

		transform->SetWorldPosition(XMVectorSet(x, y, z, 1));
		transform->SetLocalScale(size, size, size);
	}
	else
	{
		/*if (true == ("VacuumFan0" != _interactObj->GetName()) &&
			true == ("VacuumFan1" != _interactObj->GetName()) &&
			true == ("VacuumFan" != _interactObj->GetName()) &&
			true == ("VacuumFanBig" != _interactObj->GetName()))*/
		if("SuctionEffect1_Enter" == _interactObj->GetName())
		{
				gameObject->SetActive(false);
				GameObject* cam = GameObject::FindObjectByName("May_Cam");
				cam->GetComponent<FollowingCamera>()->SetEnable(false);
				cam->GetComponent<PathFollower>()->SetEnable(true);
				cam->GetComponent<PathFollower>()->SetPathData(ASSETMANAGER->GetPathData("Asset\\PathData\\ware2_Effect1.pathData"));
				cam->GetComponent<PathFollower>()->Play(1.5f, false, [&]() {
					auto may = GameObject::FindObjectByName("May");
					may->SetActive(true);
					auto next = GameObject::FindObjectByName(_interactObj->GetComponent<SuctionEffect>()->GetNext());
					may->GetTransform()->SetWorldPosition(next->GetTransform()->GetWorldPosition());

					GameObject* cam = GameObject::FindObjectByName("May_Cam");
					cam->GetComponent<PathFollower>()->SetEnable(false);
					cam->GetComponent<FollowingCamera>()->SetEnable(true);

					_interactObj = nullptr;
					_isDirtyState = true;
					_dirtyState = State::IDLE;
					_rigidbody->SetUseGravity(true);

					_isJump = false;
					_isDoubleJump = false;
					_isDash = false;
					_isInteraction = false;
					_isRun = false;
					_isCrouch = false;
					_isGroundPound = false;
					_isPickUp = false;
					_isWallSlide = false;
					_isSliding = false;
					_OnGround = true;

					_JumpCount = 2;
					_DashCount = 1;
					_lerpTime = 0.f;
					});
			
		}
		else if ("SuctionEffect2_Enter" == _interactObj->GetName())
		{
				gameObject->SetActive(false);
				GameObject* cam = GameObject::FindObjectByName("May_Cam");
				cam->GetComponent<FollowingCamera>()->SetEnable(false);
				cam->GetComponent<PathFollower>()->SetEnable(true);
				cam->GetComponent<PathFollower>()->SetPathData(ASSETMANAGER->GetPathData("Asset\\PathData\\ware2_Effect2.pathData"));
				cam->GetComponent<PathFollower>()->Play(1.f, false, [&]() {
					auto may = GameObject::FindObjectByName("May");
					may->SetActive(true);
					auto next = GameObject::FindObjectByName(_interactObj->GetComponent<SuctionEffect>()->GetNext());
					may->GetTransform()->SetWorldPosition(next->GetTransform()->GetWorldPosition());

					GameObject* cam = GameObject::FindObjectByName("May_Cam");
					
					XMVECTOR forwardwithoutY = XMVector3Normalize(cam->GetTransform()->GetWorldMatrix().r[2]);
					forwardwithoutY =  XMVectorSetY(forwardwithoutY, 0.3f);
					forwardwithoutY *= 3000.f;
					may->GetComponent<Rigidbody>()->AddForce(forwardwithoutY);

					cam->GetComponent<PathFollower>()->SetEnable(false);
					cam->GetComponent<FollowingCamera>()->SetEnable(true);

					_interactObj = nullptr;
					_isDirtyState = true;
					_dirtyState = State::FALLING;

					_rigidbody->SetUseGravity(true);

					_isJump = true;
					_isDoubleJump = true;
					_isDash = false;
					_isInteraction = false;
					_isRun = false;
					_isCrouch = false;
					_isGroundPound = false;
					_isPickUp = false;
					_isWallSlide = false;
					_isSliding = false;
					_OnGround = false;

					_JumpCount = 0;
					_DashCount = 0;
					_lerpTime = 0.f;
					});
			
		}

		else
		{
			transform->Translate(XMVectorSet(0, 1, 0, 0) * fElapsedTime * 3.f);
			_rigidbody->SetUseGravity(false);
		}
	}
}


void May::WallJump(void) noexcept
{
	if (_isWallSlide)
	{
		transform->Rotate(0, XMConvertToRadians(180.f), 0);
	}
	else if (!_isWallSlide)
	{
		_isWallSlide = true;
	}
}

void May::OnEndJump(void) noexcept
{
	_animator->SetPlaySpeed(1.f);
	_isDirtyState = true;

	if (nullptr != _interactObj)
	{
		if ("VacuumFan0" == _interactObj->GetName() ||
			"VacuumFan1" == _interactObj->GetName() ||
			"VacuumFan" == _interactObj->GetName() ||
			"VacuumFanBig" == _interactObj->GetName())
		{
			_dirtyState = State::SUCTION;
			return;
		}
	}
	_dirtyState = State::FALLING;
}

void May::OnEndDash(void) noexcept
{
	_animator->SetPlaySpeed(1.f);
	_isDash = false;
	if (_OnGround)
	{
		_DashCount = 1;
		_isDirtyState = true;
		if (!static_cast<PlayerInput*>(_inputHandler)->GetDirInput())
		{
			_dirtyState = State::DASHEND;
		}
		else
		{
			_dirtyState = State::DASHENDJOG;
		}
	}
	else
	{
		Falling();
	}

}

void May::OnEndDashEnd(void) noexcept
{
	switch (_state)
	{
	case State::DASHEND:
		_dirtyState = State::IDLE;
		break;
	case State::DASHENDJOG:
		_dirtyState = State::JUMPLANDWALKSTART;
		break;
	}
	_isDirtyState = true;
}

void May::OnEndWalk(void) noexcept
{
	switch (_state)
	{
	case State::WALKSTART:			FALLTHROUGH
	case State::JUMPLANDWALKSTART:
		_dirtyState = State::WALK;
		break;
	case State::WALK:
		_dirtyState = State::WALKEND;
		break;
	case State::WALKEND:
		_dirtyState = State::IDLE;
		break;
	}
	_isDirtyState = true;
}

void May::OnEndCrouch(void) noexcept
{
	switch (_state)
	{
	case State::CROUCHENTER:
		_dirtyState = State::CROUCHIDLE;
		break;
	case State::CROUCHSTART:
		_dirtyState = State::CROUCH;
		break;
	case State::CROUCH:
		_dirtyState = State::CROUCHEND;
		break;
	case State::CROUCHEND:
		_dirtyState = State::CROUCHIDLE;
		break;
	case State::CROUCHEXIT:
		_dirtyState = State::IDLE;
		break;
	case State::CROUCHEXITMOVE:
		_dirtyState = State::WALK;
		break;
	}
	_isDirtyState = true;
}

void May::OnEndGroundPound(void) noexcept
{
	switch (_state)
	{
	case State::GROUNDPOUNDSTART:
		_dirtyState = State::GROUNDPOUNDFALLING;
		break;
	case State::GROUNDPOUNDLAND:
		_dirtyState = State::GROUNDPOUNDEXIT;
		break;
	case State::GROUNDPOUNDEXIT:
		_dirtyState = State::IDLE;
		break;
	}
	_isDirtyState = true;
}

void May::OnEndPickUp(void) noexcept
{
	switch (_state)
	{
	case State::PICKUP:
		_dirtyState = State::IDLE;
		_isInteraction = false;
		break;
	case State::PICKENTER:
		_oldFuseCrab->GetComponent<FuseCrab>()->ClearTarget();
		_oldFuseCrab->GetComponent<FuseCrab>()->PlayLightSound();
		_oldFuseCrab = nullptr;
		_JumpCount = 2;
		_interactObj = nullptr;
		_isInteraction = false;
		_isPickUp = false;
		_dirtyState = State::IDLE;
		break;
	}
	_isDirtyState = true;
}

void May::OnEndWallSlide(void) noexcept
{
	switch (_state)
	{
	case State::WALLSLIDERENTER:
		_dirtyState = State::WALLSLIDEIDLE;
		_isDash = false;
		_isJump = false;
		_isDoubleJump = false;

		break;
	case State::WALLSLIDEJUMP:
		_dirtyState = State::FALLING;
		_isWallSlide = false;
		break;
	}
	_isDirtyState = true;
}

void May::OnEndSawJump(void) noexcept
{
	switch (_state)
	{
	case Actor::State::SAWENTER:
		XMMATRIX mat =
			gameObject->GetComponent<ModelRenderer>()->GetModel()->GetHierarchyNode("Root")->GetCombinedTransform()
			* XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-90.f))
			* transform->GetWorldMatrix();

		transform->SetWorldEulerAngle(CE_MATH::CalculateEuler(mat));
		transform->Rotate(0, CE_MATH::Deg2Rad * 90.f, 0);
		transform->SetWorldPosition(mat.r[3]);
		_dirtyState = State::SAWIDLE;

		_interactObj->GetComponent<CircularSaw>()->SetAwake(CircularSaw::PLAYER::MAY, true);

		break;
	case Actor::State::SAWSUCCSESS:
		mat =
			gameObject->GetComponent<ModelRenderer>()->GetModel()->GetHierarchyNode("Root")->GetCombinedTransform()
			* XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-90.f))
			* transform->GetWorldMatrix();

		transform->SetWorldEulerAngle(CE_MATH::CalculateEuler(mat));
		transform->Rotate(0, CE_MATH::Deg2Rad * 90.f, 0);
		transform->SetWorldPosition(mat.r[3]);

		XMVECTOR jumpforce = transform->GetForwardNoY();
		XMVectorSetY(jumpforce, 0.5f);

		_rigidbody->AddForce(jumpforce * 2000.f);
		_OnGround = false;
		_isJump = true;
		_isInteraction = false;
		_dirtyState = State::FALLING;
		break;
	}
	_isDirtyState = true;
}

void May::OnEndSlide(void) noexcept
{
	switch (_state)
	{
	case State::SLIDEENTER:
		_dirtyState = State::SLIDEIDLE;
		_isSliding = true;
		break;
	case State::SLIDEEXIT:
		_dirtyState = State::FALLING;
		_isSliding = false;
		break;
	}
	_isDirtyState = true;
}

void May::OnEndPush(void) noexcept
{
	switch (_state)
	{
	case State::PUSHENTER:
		_dirtyState = State::PUSHIDLE;
		break;
	case State::PUSHEND:
		_dirtyState = Actor::State::IDLE;
		break;
	}
	_isDirtyState = true;

}

void May::OnEndInteraction(void) noexcept
{
	switch (_state)
	{
	case State::INTERACTION:
		_dirtyState = State::IDLE;
		break;
	}
	_isInteraction = false;
	_isDirtyState = true;
}

void May::OnEndLever(void) noexcept
{
	switch (_state)
	{
	case State::LEVERLEFT:		FALLTHROUGH
	case State::LEVERRIGHT:
		_dirtyState = State::IDLE;
		break;
	}
	_isInteraction = false;
	_isDirtyState = true;
}

void May::OnEndLand(void) noexcept
{
	_rigidbody->SetAngularDrag(1.f);
	_rigidbody->SetDrag(1.f);
	switch (_state)
	{
	case State::JUMPLAND:
		_dirtyState = State::IDLE;
		break;
	case State::JUMPLANDJOG:
		_dirtyState = State::WALK;
		break;
	}

	_isDirtyState = true;
}


void May::OnEnterInteraction(void) noexcept
{
	switch (_state)
	{
	case State::INTERACTIONSTART:
		_dirtyState = State::INTERACTION;
		break;
	case State::INTERACTION:
		_dirtyState = State::INTERACTIONIDLE;
		break;
	}
	_isDirtyState = true;


	if (nullptr != _interactObj)
	{
		const std::string& objectName = _interactObj->GetName();

		if (objectName == "PowerSwitch")
		{
			_interactObj->GetComponent<PowerSwitch>()->SetLower(0, true);
		}
	}
}

void May::UpdateCheckEvent(float fElapsedTime) noexcept
{
	if (_interactObj == nullptr)
		return;
	const std::string& objectName = _interactObj->GetName();

	if (objectName == "PowerSwitch")
	{
		InteractPowerSwitch(fElapsedTime);
	}
	else if (objectName == "FuseCrab0" ||
		objectName == "FuseCrab1" ||
		objectName == "FuseCrab2")
	{
		InteractFuseCrab(fElapsedTime);
	}
	else if (objectName == "CircularSaw")
	{
		if (_state == Actor::State::SAWIDLE)
		{
			if (true == _interactObj->GetComponent<CircularSaw>()->GetRun())
			{
				_isDirtyState = true;
				_dirtyState = State::SAWSUCCSESS;
			}
		}
		else {
			InteractSaw(fElapsedTime);
		}
	}
	else if (objectName == "MachineGear")
		InteractMachineGear(fElapsedTime);
	else if (objectName == "WallSlider")
		InteractWallslider(fElapsedTime);
	else if (objectName == "PumpValveGrabPosition")
		InteractPumpValve(fElapsedTime);
	else if (objectName == "DumbbellMayGrab")
		InteractPushDumbbell(fElapsedTime);
	else if (objectName == "LeverPosition" || objectName == "Lever0Position" || objectName == "Lever1Position")
	{
		if(CE_MATH::equl_f( _lerpTime ,0.f) && CE_MATH::equl_f(_lerpRotate,0.f))
		InteractLever(fElapsedTime);
	}
}

void May::LateUpdateInteractStart(const std::string& objectName) noexcept
{
	if (objectName == "PowerSwitch")
	{
		_animator->Play("May.ao|May_Trav_Jump_Start"
			, false
			, nullptr
			, 0.3f
			, std::bind(&May::OnEnterInteraction, this));
	}
}

void May::LateUpdateInteract(const std::string& objectName) noexcept
{
	if (objectName == "PowerSwitch")
	{
		_animator->Play("May.ao|May_Bhv_Shed_FuseLever_Enter"
			, false
			, std::bind(&May::OnEnterInteraction, this));
	}
	else if (objectName == "FuseCrab")
	{
		_animator->Play("May.ao|May_Bhv_PickUp_FuseCrab_PutInSocket"
			, false
			, std::bind(&May::OnEndInteraction, this));
	}
}

void May::LateUpdateInteractIdle(const std::string& objectName) noexcept
{
	if (objectName == "PowerSwitch")
	{
		_animator->Play("May.ao|May_Bhv_Shed_FuseLever_MH"
			, true);
	}
}

void May::LateUpdateInteractEnd(const std::string& objectName) noexcept
{
	if (objectName == "PowerSwitch")
	{
		_animator->Play("May.ao|May_Bhv_Shed_FuseLever_Success"
			, false
			, std::bind(&May::OnEndInteraction, this));
		_interactObj = nullptr;
	}
}

void May::OnTriggerDash(void) noexcept
{
	if (true == _OnGround)
	{
		if (!static_cast<PlayerInput*>(_inputHandler)->GetDirInput())
		{
			_rigidbody->SetAngularDrag(30.f);
			_rigidbody->SetDrag(30.f);
		}
	}
	else
	{
		_rigidbody->SetFreezePositionY(false);
		_rigidbody->SetUseGravity(true);
	}
}

void May::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	GameObject* obj = other->GetGameObject();

	switch (obj->GetTag())
	{
	case GameObjectTag::LOADING_EVENT:
		{
			if (SCENEMANAGER->IsWhileLoading() == false && SCENEMANAGER->GetPreLoadingScene() == nullptr)
			{
				SCENEMANAGER->LoadSceneAsync(obj->GetName().c_str(), false);

			}
			if ("WareHouse_01" == SCENEMANAGER->GetSceneID())
				GameObject::FindObjectByName("FuseCrabUI")->SetActive(false);
			return;
		}
		break;

	case GameObjectTag::CAMERA_EVENT:
		{
			FollowingCamera* followCam = _camera->GetGameObject()->GetComponent<FollowingCamera>();
			followCam->EnterCameraEvent( obj->GetTransform()->GetWorldPosition()
										, obj->GetTransform()->GetLocalEulerAngle());
			return;
		}
		break;

	case GameObjectTag::GROUND:

		if (_state == State::FALLING
			|| _state == State::JUMP
			|| _state == State::JUMPFWD
			|| _state == State::SECOND_JUMP
			|| _state == State::DASH
			|| _state == State::WALLSLIDERENTER
			|| _state == State::WALLSLIDEIDLE
			|| _state == State::WALLSLIDEJUMP
			|| _state == State::SLIDEIDLE)
		{
			if (_isInteraction)
				_isInteraction = false;

			/*if (_state == State::DASH)
			{
				_rigidbody->SetFreezePositionY(false);
				_rigidbody->SetUseGravity(true);
			}*/

			_rigidbody->SetFreezeRotationY(false);

			_OnGround = true;
			_isSliding = false;
			_isWallSlide = false;
			_isDoubleJump = false;
			_isGroundPound = false;
			_isJump = false;
			_isDash = false;
			if (_isPickUp)
			{
				_JumpCount = 1;
			}
			else
			{
				_JumpCount = 2;
			}
			_DashCount = 1;
		}
		else if (_state == State::DASH)
		{
			OnEndDash();
			_rigidbody->SetAngularDrag(100.f);
			_rigidbody->SetDrag(100.f);

			_OnGround = true;
			_isWallSlide = false;
			_isDoubleJump = false;
			_isJump = false;
			_isDash = false;
			_JumpCount = 2;
			_DashCount = 1;
		}
		else if (_isGroundPound)
		{
			if (_state == State::GROUNDPOUNDLAND)
				return;

			_isDirtyState = true;
			_dirtyState = State::GROUNDPOUNDLAND;

			_OnGround = true;
			_isWallSlide = false;
			_isDoubleJump = false;
			_isJump = false;
			_isDash = false;
			_JumpCount = 2;
			_DashCount = 1;
			return;
		}
		else
			return;

		if (!static_cast<PlayerInput*>(_inputHandler)->GetDirInput())
		{
			_isDirtyState = true;
			_dirtyState = State::JUMPLAND;
		}
		else
		{
			_isDirtyState = true;
			_dirtyState = State::JUMPLANDJOG;
		}
		break;

	case GameObjectTag::OBJECT:
		_interactObj = obj;

		if (_interactObj->GetName() == "PumpValveGrabPosition")
		{
			_ValveInteractUseObject = GameObject::FindObjectByName("PumpValve");
		}
		else if (_interactObj->GetName() == "DumbbellMayGrab")
		{
			_ValveInteractUseObject = obj;
		}

		break;

	case GameObjectTag::OBSTACLE:
		/* 땅이 아닐때만 벽에 붙음*/
		if (!_OnGround)
		{
 			if (obj->GetName() == "MachineGear")
				_rigidbody->SetFreezeRotationY(true);


			_wallJumpPower = 2.f;
			if (_isDash)
			{
				_rigidbody->SetFreezePositionY(false);
				_rigidbody->SetUseGravity(true);
			}

			float theta = CE_MATH::CalcDotTheta(transform->GetForwardNoY(), obj->GetTransform()->GetForwardNoY(), true);

			if (XMVectorGetZ(transform->GetForward()) > 0 && theta > 30.f
				|| XMVectorGetZ(transform->GetForward()) < 0 && theta < 150.f)
			{
				return;
			}

			_interactObj = obj;
			_JumpCount = 2;
			_DashCount = 1;
			_isDoubleJump = false;
			_isJump = false;
			_isDash = false;
			_isDirtyState = true;
			_dirtyState = State::WALLSLIDERENTER;
		}
		break;

	case GameObjectTag::SLIDE:

		_isJump = false;
		_isDash = false;
		_JumpCount = 2;
		_DashCount = 1;
		_isWallSlide = false;
		_isDoubleJump = false;
		_isDirtyState = true;
		_dirtyState = State::SLIDEENTER;
		break;

	case GameObjectTag::EFFECT:

		if ("VacuumFan0" == obj->GetName() ||
			"VacuumFan1" == obj->GetName() ||
			"VacuumFan" == obj->GetName() ||
			"VacuumFanBig" == obj->GetName())
		{
			_JumpCount = 2;
			_DashCount = 1;
			_rigidbody->SetVelocity(0, 0, 0);
			_rigidbody->SetUseGravity(true);
		}
		else
		{
			if (false == obj->GetComponent<SuctionEffect>()->GetOn()) 
				return;
		}
		_isDirtyState = true;
		_dirtyState = State::SUCTION;
		_interactObj = obj;
		_lerpTime = 0.f;
		break;

	case GameObjectTag::ROT_OBSTACLE:

		if (_state == Actor::State::WALLSLIDEJUMP)
			return;

		if (_isDash)
		{
			_rigidbody->SetFreezePositionY(false);
			_rigidbody->SetUseGravity(true);
		}

		_wallJumpPower = 1.f;

		_rigidbody->SetEnable(false);
		transform->SetParent(obj->GetTransform());
		transform->SetLocalEulerAngle(0.f, CE_MATH::Deg2Rad * 90.f, 0.f);

		//Rigidbody* rigidbody = gameObject->GetComponent<Rigidbody>();
		//PxRigidActor* parent = obj->GetComponent<Rigidbody>()->GetRigidActorXXX();

		//PxTransform local = PxTransform(XMVectorGetX(other->GetOffsetPos())
		//								, XMVectorGetY(other->GetOffsetPos())
		//								, XMVectorGetZ(other->GetOffsetPos()));

		//PxD6Joint* joint = PxD6JointCreate(*PhysicsSystem::Instance()->GetPxPhysicsXXX()
		//								, parent
		//								, local
		//								, rigidbody->GetRigidActorXXX()
		//								, PxTransform(0, 0, 0));

		//joint->setMotion(PxD6Axis::eTWIST, PxD6Motion::eLOCKED);
		//joint->setMotion(PxD6Axis::eSWING1, PxD6Motion::eLOCKED);
		//joint->setMotion(PxD6Axis::eSWING2, PxD6Motion::eLOCKED);
		//rigidbody->SetWorldJointXXX(joint);


		_interactObj = obj;
		_JumpCount = 2;
		_DashCount = 1;
		_isDoubleJump = false;
		_isJump = false;
		_isDash = false;

		_isDirtyState = true;
		_dirtyState = State::WALLSLIDERENTER;

		break;

	case GameObjectTag::SAVEPOINT:
		if (_savePoint == obj)
			return;

		_savePoint = obj;

		PlayerPrefs::This().SetFloat("SavePointX", XMVectorGetX(obj->GetTransform()->GetLocalPosition()));
		PlayerPrefs::This().SetFloat("SavePointY", XMVectorGetY(obj->GetTransform()->GetLocalPosition()));
		PlayerPrefs::This().SetFloat("SavePointZ", XMVectorGetZ(obj->GetTransform()->GetLocalPosition()));
		break;

	case GameObjectTag::DEADZONE:
		_isDead = true;
		break;

	case GameObjectTag::ROT_OBSTACLE2:

		if (_state == Actor::State::WALLSLIDEJUMP)
			return;

		if (_isDash)
		{
			_rigidbody->SetFreezePositionY(false);
			_rigidbody->SetUseGravity(true);
		}

		_wallJumpPower = 1.f;

		_rigidbody->SetEnable(false);
		transform->SetParent(obj->GetTransform());
		transform->SetLocalPosition(-2.5f, 0.f, 0.f);
		transform->SetLocalEulerAngle(0.f, CE_MATH::Deg2Rad * 90.f, 0.f);

		_interactObj = obj;
		_JumpCount = 2;
		_DashCount = 1;
		_isDoubleJump = false;
		_isJump = false;
		_isDash = false;

		_isDirtyState = true;
		_dirtyState = State::WALLSLIDERENTER;


		if ("WareHouse_03" == SCENEMANAGER->GetSceneID())
		{
			FollowingCamera* followCam = _camera->GetGameObject()->GetComponent<FollowingCamera>();
			GameObject* camObject = GameObject::FindObjectByName("CameraPoint");
			followCam->EnterCameraEvent(camObject->GetTransform()->GetWorldPosition()
										, camObject->GetTransform()->GetLocalEulerAngle());
		}
		break;
	}
}

void May::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
	GameObject* obj = other->GetGameObject();
	switch (obj->GetTag())
	{
	case GameObjectTag::GROUND:
		break;

	case GameObjectTag::OBJECT:
		_interactObj = nullptr;
		break;

	case GameObjectTag::OBSTACLE:
		_interactObj = nullptr;
		_ValveInteractUseObject = nullptr;
		break;

	case GameObjectTag::EFFECT:
		if ("VacuumFan0" == obj->GetName() ||
			"VacuumFan1" == obj->GetName() ||
			"VacuumFan" == obj->GetName() ||
			"VacuumFanBig" == obj->GetName())
		{
			_rigidbody->SetUseGravity(true);
			_interactObj = nullptr;
		}
		break;

	case GameObjectTag::CAMERA_EVENT:
		{
			FollowingCamera* followCam = _camera->GetGameObject()->GetComponent<FollowingCamera>();
			followCam->ExitCameraEvent();
			return;
		}
		break;
	}
}

void May::InteractPowerSwitch(float fElapsedTime) noexcept
{
	if (_interactObj == nullptr)
		return;

	if (_state == Actor::State::INTERACTIONSTART
		|| _state == Actor::State::INTERACTION
		|| _state == Actor::State::INTERACTIONIDLE)
	{
		if (!_isInteraction)
		{
			_isInteraction = true;
			XMStoreFloat3(&_lerpVector, transform->GetWorldPosition());
		}
		else if (_isInteraction == true)
		{
			XMMATRIX mayGrabMatrix = _interactObj->GetComponent<PowerSwitch>()->GetMayGrabMatrix();

			XMMATRIX rightHandBoneMatrix = _rightHandBone->GetCombinedTransform()
				* XMMatrixScaling(0.01f, 0.01f, 0.01f)
				* XMMatrixRotationY(XMConvertToRadians(-90.f))
				* transform->GetWorldMatrix();

			float dist = XMVectorGetX(XMVector3Length(rightHandBoneMatrix.r[3] - transform->GetWorldMatrix().r[3]));

			XMVECTOR pos = XMVectorSet(XMVectorGetX(mayGrabMatrix.r[3]) - 0.1f
				, XMVectorGetY(mayGrabMatrix.r[3]) - dist + 0.35f
				, XMVectorGetZ(mayGrabMatrix.r[3]) - 0.1f
				, 1.f);

			_interactime += fElapsedTime;
			float f = CETween::Lerp(XMVectorGetY(transform->GetWorldMatrix().r[3]), XMVectorGetY(pos), fElapsedTime, CETween::EaseType::easeOutElastic);

			pos = XMVectorSetY(pos, f);

			transform->SetWorldPosition(pos);
			CalcDirection(360, fElapsedTime);

			if (_state == Actor::State::INTERACTIONIDLE)
			{
				/* 레버가 중간에 떨어질때 interaction false*/
				if (40 <= _interactObj->GetComponent<PowerSwitch>()->GetCurrentAngle())
				{
					_isInteraction = false;
					_interactime = 0.f;
					_isDirtyState = true;
					_dirtyState = Actor::State::INTERACTIONEND;
				}
			}
		}

	}
}

void May::InteractFuseCrab(float fElapsedTime) noexcept
{
	if (_interactObj == nullptr)
		return;

	if (_state == Actor::State::PICKUP)
	{
		if (!_isInteraction)
		{
			_isInteraction = true;

			_oldFuseCrab = _interactObj;

			_interactObj->GetComponent<FuseCrab>()->SetHierarcyNode(_PickUpBone);
			_interactObj->GetComponent<FuseCrab>()->SetInteractTransform(transform);
			_interactObj->GetComponent<FuseCrab>()->PickUp();
			_interactObj->GetComponent<Rigidbody>()->SetEnable(false);
			_interactObj->GetComponent<Collider>()->SetEnable(false);

			_interactObj = nullptr;
		}
	}
}

void May::InteractSaw(float fElapsedTime) noexcept
{
	if (_interactObj == nullptr)
		return;

	if (_state == Actor::State::SAWENTER)
	{

		_interactObj = GameObject::FindObjectByName("CircularSaw");

		XMMATRIX mayGrabMatrix = _interactObj->GetComponent<CircularSaw>()->GetGrabMatrix(CircularSaw::PLAYER::MAY);

		XMVECTOR pos = XMVectorSet(XMVectorGetX(mayGrabMatrix.r[3])
			, XMVectorGetY(mayGrabMatrix.r[3])
			, XMVectorGetZ(mayGrabMatrix.r[3]) - 0.35f
			, 1.f);

		if (!_isInteraction)
		{
			_interactime += fElapsedTime;

			float fx = CETween::Lerp(XMVectorGetX(transform->GetWorldMatrix().r[3]), XMVectorGetX(pos), _interactime, CETween::EaseType::easeOutSine);
			float fz = CETween::Lerp(XMVectorGetZ(transform->GetWorldMatrix().r[3]), XMVectorGetZ(pos), _interactime, CETween::EaseType::easeOutSine);

			pos = XMVectorSetX(pos, fx);
			pos = XMVectorSetZ(pos, fz);

			transform->SetWorldPosition(pos);
			CalcDirection(360, fElapsedTime);

			if (_interactime >= 1.f)
			{
				_interactime = 0.f;
				_isInteraction = true;
				_isSawEnter = false;
				_animator->SetPlaySpeed(1.f);
			}
		}
		else
		{
			transform->SetWorldPosition(pos);
		}
	}
}

void May::InteractPumpValve(float fElapsedTime) noexcept
{
	if (nullptr == _interactObj)
		return;

	XMVECTOR posiiton = _interactObj->GetTransform()->GetWorldMatrix().r[3];
	transform->SetWorldPosition(posiiton);

	if (_state == Actor::State::VALVE_ROTATE)
	{
		if (nullptr != _ValveInteractUseObject)
			_ValveInteractUseObject->GetComponent<PumpValve>()->SetRotate(true);
	}
	else
	{
		if (nullptr != _ValveInteractUseObject)
			_ValveInteractUseObject->GetComponent<PumpValve>()->SetRotate(false);
	}
}

void May::InteractPushDumbbell(float fElapsedTime) noexcept
{
	if (nullptr == _interactObj)
		return;

	if (_state == Actor::State::PUSHENTER ||
		_state == Actor::State::PUSHIDLE)
	{
		transform->SetLocalEulerAngle(0.f, XMConvertToRadians(90.f), 0.f);
		transform->SetWorldPosition(_interactObj->GetTransform()->GetWorldMatrix().r[3]);
	}
	else if (_state == Actor::State::PUSH)
	{
	}
	else if (_state == Actor::State::PUSHEND)
	{

	}
}

void May::InteractLever(float fElapsedTime) noexcept
{
	XMVECTOR pos = transform->GetWorldPosition();


	if (!_isInteraction && CE_MATH::equl_f(_interactime, 0.f))
	{
		XMStoreFloat3(&_lerpPosition, transform->GetWorldMatrix().r[3]);
	}

	if (!_isInteraction)
	{
		_interactime += fElapsedTime * 8.f;

		float fx = CETween::Lerp(_lerpPosition.x, XMVectorGetX(_interactObj->GetTransform()->GetWorldPosition()), std::max(std::min(1.f,_interactime), 0.0f), CETween::EaseType::easeOutCubic);
		float fz = CETween::Lerp(_lerpPosition.z, XMVectorGetZ(_interactObj->GetTransform()->GetWorldPosition()), std::max(std::min(1.f,_interactime), 0.0f), CETween::EaseType::easeOutCubic);

		pos = XMVectorSetX(pos, fx);
		pos = XMVectorSetZ(pos, fz);

		transform->SetWorldPosition(pos);

		if (_interactime >= 1.f)
		{
			XMVECTOR dir = XMVectorZero();
			XMVECTOR pos = XMVectorZero();
			if (_interactObj->GetName() == "LeverPosition")
			{
				pos = GameObject::FindObjectByName("Lever")->GetTransform()->GetWorldPosition();
				dir =  XMVector3Normalize(pos - _interactObj->GetTransform()->GetWorldPosition());
			}
			else if (_interactObj->GetName() == "Lever0Position")
			{
				pos = GameObject::FindObjectByName("Lever0")->GetTransform()->GetWorldPosition();
				dir = XMVector3Normalize(pos - _interactObj->GetTransform()->GetWorldPosition());
			}
			else if (_interactObj->GetName() == "Lever1Position")
			{
				pos = GameObject::FindObjectByName("Lever1")->GetTransform()->GetWorldPosition();
				dir = XMVector3Normalize(pos - _interactObj->GetTransform()->GetWorldPosition());
			}

			_lerpRotate = CE_MATH::CalcDotTheta(transform->GetForwardNoY(), XMVectorSetY(dir, 0.f),false);

			if (0 < CE_MATH::isLeft(pos, transform->GetWorldMatrix()))
			{
				_lerpRotate = -_lerpRotate;
			}


			transform->Rotate(0.f, _lerpRotate, 0.f);
			_lerpRotate = 0.f;
			ZeroMemory(&_lerpPosition, sizeof(XMFLOAT3));
			_interactime = 0;
			_isInteraction = true;
			_isSawEnter = false;
			_animator->SetPlaySpeed(1.f);
		}
	}
}

void May::InteractLeverReverse(void) noexcept
{
	if (_interactObj->GetName() == "LeverPosition")
	{
		Lever* lever = GameObject::FindObjectByName("Lever")->GetComponent<Lever>();
		lever->ReverseDirection();
		lever->PlayLeverActive();


	}
	else if (_interactObj->GetName() == "Lever0Position" || _interactObj->GetName() == "Lever1Position")
	{
		Lever* lever0 = GameObject::FindObjectByName("Lever0")->GetComponent<Lever>();
		Lever* lever1 = GameObject::FindObjectByName("Lever1")->GetComponent<Lever>();

		lever0->ReverseDirection();
		lever0->PlayLeverActive();
		lever1->ReverseDirection();
		lever1->PlayLeverActive();
	}
}

void May::InteractMachineGear(float fElapsedTime) noexcept
{
	if (!_interactObj->GetComponent<MachineGear>()->GetRunning())
		return;

	XMVECTOR mg = _interactObj->GetTransform()->GetWorldMatrix().r[3];
	XMVECTOR my = transform->GetWorldMatrix().r[3];
	XMVECTOR dd = XMVector3Normalize(my - mg);

	float dist = XMVectorGetX(XMVector3Length(my - mg));

	/* MachineGear 가 fElapsedTime 으로 돌기때문에 이렇게 넣어주어도 가능
	다른값을로 돌땐 그 값을 가져와야 할듯 */
	dd = XMVector3TransformCoord(dd, XMMatrixRotationZ(fElapsedTime));

	my = mg + (dd * dist);

	transform->SetWorldPosition(my);
}

void May::InteractWallslider(float fElapsedTime) noexcept
{
}
