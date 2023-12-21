#include "pch.h"
#include "FlyingMachine.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Animator.h"
#include "PlayerInput.h"
#include "HierarcyNode.h"
#include "TrailRenderer.h"
#include "PlayerStatus.h"
#include "FlyingMachineTurretHolder.h"
#include "BoneChildObject.h"
#include "PostEffect.h"
#include "PlayerScreen.h"
#include "ExplosionFX.h"
#include "TurretCamera.h"
#include "FlyingCamera.h"
#include "FadeController.h"
#include "RestartScene.h"
#include "Burnable.h"
#include "ParticleSystemAnimation.h"
#include "SquirrelTurretHolder.h"
#include "OptionPopup.h"
#include "AudioSource.h"
#include "TunnelDoor.h"
#include "NarrationManager.h"
#include "BGMPlayer.h"
#include "EffectPlayer.h"
#include "AudioListener.h"

FlyingMachine::FlyingMachine(bool bOperationPad) noexcept
	: _usingBoost(false)
	, _boostTime(0)
	, _boostEndTime(3.f)
	, _boostCool(false)
	, _boostCoolTime(0)
	, _boostCoolEndTime(5.f)
	, _turretCam(nullptr)
	, _propellerOn(false)
{
	_bOperationPad = bOperationPad;

	_playerInput = new PlayerInput(bOperationPad);
	_playerInput->SetActor(this);

	_inputHandler = _playerInput;

	_speed = 45.f;
}

FlyingMachine::~FlyingMachine(void) noexcept
{
	if (_playerInput != nullptr)
		delete _playerInput;
}

void FlyingMachine::Start(void) noexcept
{
	/* audio Listener*/
	gameObject->AddComponent(AudioListener::Create());

	//Setting
	gameObject->SetTag(GameObjectTag::PLAYER);

	_direction = Direction::END;
	_isDirtyDirection = false;
	_dirtyDirection = Direction::END;

	/* Status UI */
	_statusUI = new PlayerStatus(this, PlayerStatus::Type::TOGETHER);
	_statusUI->SetTexture("Asset\\UI\\Player\\Health\\Portrait_Flyingmachine.dds");
	gameObject->AddComponent(_statusUI);
	_statusUI->GetTransform()->Translate(0.f, -30.f, 0.f);

	/* ModelRenderer */
	_renderer = ModelRenderer::Create(false);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Item\\Flying.fbx"));
	XMMATRIX offset = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(CE_MATH::ToRadian(-90));
	_renderer->SetOffsetMatrix(offset);
	gameObject->AddComponent(_renderer);

	_propeller = _renderer->GetModel()->GetHierarchyNode("Propeller");

	/* Animator */
	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play("FlyingMachine.ao|FlyingMachine_Bhv_Tree_FlyingMachine_Mh", true);

	/* Rigidbody */
	_rigidbody = Rigidbody::Create(false);
	_rigidbody->SetUseGravity(false);
	_rigidbody->SetAngularDrag(0);
	gameObject->AddComponent(_rigidbody);
	_rigidbody->SetAngularDrag(100);
	_rigidbody->SetDrag(100);

	/* Collider */
	Collider* collider = Collider::CreateCapsuleCollider(0.7f, 4.8f, XMVectorSet(0, 0, -0.74f, 0), XMVectorSet(0, CE_MATH::ToRadian(90), 0, 0));
	gameObject->AddComponent(collider);
	collider = Collider::CreateCapsuleCollider(0.6f, 8.f, XMVectorSet(0, 3.16f, -0.7f, 0), XMVectorSet(CE_MATH::ToRadian(90), 0, 0, 0));
	gameObject->AddComponent(collider);
	/*collider = Collider::CreateCapsuleCollider(0.6f, 1.2f, XMVectorSet(0, 4.75f, 0.2f, 0), XMVectorSet( 0, 0,CE_MATH::ToRadian(90), 0));
	gameObject->AddComponent(collider);	*/

	/* Trigger */
	collider = Collider::CreateCapsuleCollider(0.7f, 4.8f, XMVectorSet(0, 0, -0.74f, 0), XMVectorSet(0, CE_MATH::ToRadian(90), 0, 0));
	collider->SetIsTrigger(true);
	gameObject->AddComponent(collider);
	collider = Collider::CreateCapsuleCollider(0.6f, 8.f, XMVectorSet(0, 3.16f, -0.7f, 0), XMVectorSet(CE_MATH::ToRadian(90), 0, 0, 0));
	collider->SetIsTrigger(true);
	gameObject->AddComponent(collider);

	/* Trail */
	GameObject* leftTrail = GameObject::Instantiate("Flying Left Trail");
	leftTrail->GetTransform()->SetParent(transform);
	leftTrail->GetTransform()->SetLocalPosition(0, 0, 0);
	TrailRenderer* trailRenderer = TrailRenderer::Create();
	leftTrail->AddComponent(trailRenderer);
	trailRenderer->SetOffsetMat(offset);
	trailRenderer->SetAddTime(0.01f);
	trailRenderer->SetLifeTime(0.5f);
	trailRenderer->SetSmoothLevel(12);
	trailRenderer->SetNode1(_renderer->GetModel()->GetHierarchyNode("Left_1"));
	trailRenderer->SetNode2(_renderer->GetModel()->GetHierarchyNode("Left_2"));
	trailRenderer->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
											, ASSETMANAGER->GetTextureData("Asset\\Texture\\trail_cut.dds"));
	trailRenderer->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1, 1, 1, 1.f));
	trailRenderer->SetPassType(TrailRenderer::TrailRenderOption::ALPHABLEND);

	leftTrail->SetLayer(GameObjectLayer::TRANSLUCENT);
	trailRenderer->SetActive(true);

	GameObject* rightTrail = GameObject::Instantiate("Flying Right Trail");
	rightTrail->GetTransform()->SetParent(transform);
	rightTrail->GetTransform()->SetLocalPosition(0, 0, 0);
	trailRenderer = TrailRenderer::Create();
	rightTrail->AddComponent(trailRenderer);
	trailRenderer->SetOffsetMat(offset);
	trailRenderer->SetAddTime(0.01f);
	trailRenderer->SetLifeTime(0.5f);
	trailRenderer->SetSmoothLevel(12);
	trailRenderer->SetNode1(_renderer->GetModel()->GetHierarchyNode("Right_1"));
	trailRenderer->SetNode2(_renderer->GetModel()->GetHierarchyNode("Right_2"));
	trailRenderer->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
											, ASSETMANAGER->GetTextureData("Asset\\Texture\\trail_cut.dds"));
	trailRenderer->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1, 1, 1, 1.f));
	trailRenderer->SetPassType(TrailRenderer::TrailRenderOption::ALPHABLEND);

	rightTrail->SetLayer(GameObjectLayer::TRANSLUCENT);
	trailRenderer->SetActive(true);

	/* Cody */
	GameObject* cody = GameObject::Instantiate("Flying Cody");
	cody->GetTransform()->SetParent(transform);

	// Model 
	ModelRenderer* renderer = ModelRenderer::Create(false);
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Cody\\Cody_FlyingMachine.fbx"));
	XMMATRIX codyOffset = XMMatrixScaling(0.01f, 0.01f, 0.01f)* XMMatrixRotationY(CE_MATH::ToRadian(-90))* XMMatrixRotationZ(CE_MATH::ToRadian(-90));
	renderer->SetOffsetMatrix(codyOffset);
	cody->AddComponent(renderer);

	// Bone Child
	HierarcyNode* bone = _renderer->GetModel()->GetHierarchyNode("Align");
	BoneChildObject* boneChild = new BoneChildObject(bone, offset);
	cody->AddComponent(boneChild);

	// Animator 
	Animator* animator = Animator::Create();
	cody->AddComponent(animator);
	animator->Play("Cody.ao|Cody_Bhv_Tree_FlyingMachine_Mh", true);

	/* Camera */
	GameObject* cam = GameObject::Instantiate("FlyingCamera");
	_flyingCam = new FlyingCamera(transform);
	cam->AddComponent(_flyingCam);

	/* Turret */
	GameObject* turretBoneObj = GameObject::Instantiate("Flying Machine Turret Base Bone");
	turretBoneObj->GetTransform()->SetParent(transform);

	bone = _renderer->GetModel()->GetHierarchyNode("TurretBase");
	XMMATRIX localOffset = XMMatrixRotationY(CE_MATH::ToRadian(90));
	boneChild = new BoneChildObject(bone, offset);//, localOffset);
	turretBoneObj->AddComponent(boneChild);

	GameObject* turretObj = GameObject::Instantiate("Flying Machine Turret Base");
	turretObj->GetTransform()->SetParent(turretBoneObj->GetTransform());
	turretObj->GetTransform()->SetLocalEulerAngle(0, 0, 0);
	turretObj->GetTransform()->SetLocalPosition(0, 0, 0);

	_turret = new FlyingMachineTurretHolder(false);
	//_turret = new FlyingMachineTurretHolder(!_bOperationPad); //패드
	turretObj->AddComponent(_turret);

	_goalRadian = Vector3(0, 0, 0);
	_moveDirection = Vector3(0, 0, 0);

	CreateAfterBurnerFX();

	_propellerPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_propellerPlayer);

	_boostPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_boostPlayer);

	_damagePlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_damagePlayer);

	_audioAsset[static_cast<uint>(AudioID::PROPELLER)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage3\\Play_Gameplay_Vehicles_FlyingMachine_Propeller.ogg");
	_audioAsset[static_cast<uint>(AudioID::BOOST)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage3\\Play_Gameplay_Vehicles_FlyingMachine_Boost.ogg");
	_audioAsset[static_cast<uint>(AudioID::DAMAGE)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage3\\Play_Gameplay_Vehicles_FlyingMachine_DamageTaken.ogg");

}

void FlyingMachine::Update(float fElapsedTime) noexcept
{
	if (OptionPopup::This()->IsShow())
		return;

	if (_turretCam == nullptr)
	{
		_turretCam = Camera::GetMainCamera()->GetGameObject()->GetComponent<TurretCamera>();
	}

	if (_isDeath)
	{
		_deathTime = std::min(1.f, _deathTime + fElapsedTime);

		PostEffect::This().setUseGrey(true, _deathTime);

		if (_deathTime == 1.f)
			_isDeath = false;
	}

	if (_isDead)
		return;

	//프로펠러 회전
	_propellerAngle += 1000 * fElapsedTime;
	if (_propellerAngle > 360.f)
		_propellerAngle = 0;
	_propeller->SetOffsetMatrix(XMMatrixRotationY(CE_MATH::ToRadian(_propellerAngle)));

	//부스트 사용 중?
	if (_usingBoost)
	{
		//end Time까지 부스트 사용 
		_boostTime = std::min(_boostEndTime, _boostTime + fElapsedTime);
		
		if (_boostTime >= _boostEndTime - 1)
		{
			_turretCam->SetBoost(false);
			_flyingCam->SetBoost(false);
		};

		if (_boostTime == _boostEndTime)
		{
			OnEndBoost();
		}
	}
	else
	{	
		//부스트 쿨타임
		if (_boostCool)
		{
			_boostCoolTime = std::min(_boostCoolEndTime, _boostCoolTime + fElapsedTime);
			if (_boostCoolTime == _boostCoolEndTime)
				_boostCool = false;
		}
		else {
			//부스트 사용
			if (_bOperationPad)
			{
				if(PADINPUT->GetButtonDown(PadInput::BUTTON_Y))
				{
					_usingBoost = true;

					_turretCam->SetBoost(true);
					_flyingCam->SetBoost(true);

					_speed *= 2;
					_animator->Play("FlyingMachine.ao|FlyingMachine_Bhv_Tree_FlyingMachine_Boost", true);
					_particle[0]->Play();
					_particle[1]->Play();
				}
			}
			else
			{
				if (INPUT->GetKeyDown(VK_SPACE))
				{
					_usingBoost = true;

					_turretCam->SetBoost(true);
					_flyingCam->SetBoost(true);

					_speed *= 2;
					_animator->Play("FlyingMachine.ao|FlyingMachine_Bhv_Tree_FlyingMachine_Boost", true);
					_particle[0]->Play();
					_particle[1]->Play();
				}
			}

			if (_usingBoost)
			{
				_boostPlayer->Play(_audioAsset[static_cast<uint>(AudioID::BOOST)], 0.05f);
			}
		}
	}

	//기본으로 전진 중 + 좌,우로 가는 중엔 좌우로 추가 이동 
	XMVECTOR foward = XMVector3Normalize(transform->GetForward()) + (XMVECTOR)_moveDirection;
	transform->Translate(foward * _speed * fElapsedTime);
	//transform->SetWorldPosition(0, 0, 20);

	_playerInput->FixedUpdate(fElapsedTime);
	
	//이동
	uint8 dirInput = _playerInput->GetDirInput();
	if (dirInput != 0)
	{
		_isDirtyDirection = true;
		
		//상승
		if (dirInput & INPUT_FRONT)
		{
			if (dirInput & INPUT_RIGHT)
				_dirtyDirection = Direction::RIGHT_UP;
			else if (dirInput & INPUT_LEFT)
				_dirtyDirection = Direction::LEFT_UP;
			else
				_dirtyDirection = Direction::UP;
		}
		//하강
		else if (dirInput & INPUT_BACKWARD)
		{
			if (dirInput & INPUT_RIGHT)
				_dirtyDirection = Direction::RIGHT_DOWN;
			else if (dirInput & INPUT_LEFT)
				_dirtyDirection = Direction::LEFT_DOWN;
			else
				_dirtyDirection = Direction::DOWN;
		}	
		//좌회전
		else if (dirInput & INPUT_LEFT)
		{
			_dirtyDirection = Direction::LEFT;
		}
		//우회전
		else if (dirInput & INPUT_RIGHT)
		{
			_dirtyDirection = Direction::RIGHT;
		}
	}
	else
	{
		_isDirtyDirection = true;
		_dirtyDirection = Direction::END;
	}

	if (_isLerp)
	{
		if (_lerpEndTime == 0)
		{
			_isLerp = false;
			return;
		}

		_lerpTime = std::min(_lerpEndTime, _lerpTime + fElapsedTime);
		float time = _lerpTime/ _lerpEndTime;

		float x = _curRadian.x;
		float y = _curRadian.y;
		float z = _curRadian.z;

		if (_goalRadian.x != _curRadian.x)
			x = CETween::Lerp(_curRadian.x, _goalRadian.x, time, _easeType);// , CETween::EaseType::easeInSine);
	
		if (_goalRadian.y != _curRadian.y)
			y = CETween::Lerp(_curRadian.y, _goalRadian.y, time);
		
		if (_goalRadian.z != _curRadian.z)
				z = CETween::Lerp(_curRadian.z, _goalRadian.z, time);

		transform->SetWorldEulerAngle(x, y, z);

		if (_lerpTime == _lerpEndTime)
		{
			_isLerp = false;
			_goalRadian = Vector3(0, 0, 0);
		}
	}

}

void FlyingMachine::LateUpdate(float fElapsedTime) noexcept
{
	if (_dirtyDirection == _direction)
		return;

	_isDirtyDirection = false;
	
	Direction prevDir = _direction;
	_direction = _dirtyDirection;
	_dirtyDirection = Direction::END;

	_isLerp = true;
	_lerpTime = 0;

	_curRadian = transform->GetWorldEulerAngle();
	_moveDirection = Vector3(0, 0, 0);
	_easeType = CETween::EaseType::easeLiner;
	switch (_direction)
	{
	case Direction::UP:
	{
		_goalRadian = Vector3(CE_MATH::ToRadian(-60), _curRadian.y, 0);
		_animator->Play("FlyingMachine.ao|FlyingMachine_Bhv_Tree_FlyingMachine_Mh", true);

		float angle = CE_MATH::Rad2Deg * fabs(_curRadian.x - _goalRadian.x);
		_lerpEndTime = angle * 0.01f;
	}
	break;

	case Direction::LEFT_UP:
	{
		_animator->Play("FlyingMachine_ao|FlyingMachine_Bhv_Tree_FlyingMachine_SteerLeft", true);

		_goalRadian = Vector3(CE_MATH::ToRadian(-20), _curRadian.y + CE_MATH::ToRadian(-70), CE_MATH::ToRadian(30));

		float angle = CE_MATH::Rad2Deg * fabs(_curRadian.y - _goalRadian.y);
		_lerpEndTime = angle * 0.01f;

		_moveDirection = -XMVector3Normalize(transform->GetRight()) * 0.5f;
	}
	break;

	case Direction::RIGHT_UP:
	{
		_animator->Play("FlyingMachine_ao|FlyingMachine_Bhv_Tree_FlyingMachine_SteerLeft", true);

		_goalRadian = Vector3(CE_MATH::ToRadian(-20), _curRadian.y + CE_MATH::ToRadian(70), CE_MATH::ToRadian(-30));

		float angle = CE_MATH::Rad2Deg * fabs(_curRadian.y - _goalRadian.y);
		_lerpEndTime = angle * 0.01f;

		_moveDirection = -XMVector3Normalize(transform->GetRight()) * 0.5f;
	}
	break;

	case Direction::DOWN:
	{
		_goalRadian = Vector3(CE_MATH::ToRadian(60), _curRadian.y, 0);
		_animator->Play("FlyingMachine.ao|FlyingMachine_Bhv_Tree_FlyingMachine_Mh", true);

		float angle = CE_MATH::Rad2Deg * fabs(_curRadian.x - _goalRadian.x);
		_lerpEndTime = angle * 0.01f;
	}
	break;

	case Direction::LEFT_DOWN:
	{
		_animator->Play("FlyingMachine_ao|FlyingMachine_Bhv_Tree_FlyingMachine_SteerLeft", true);

		_goalRadian = Vector3(CE_MATH::ToRadian(20), _curRadian.y + CE_MATH::ToRadian(-90), CE_MATH::ToRadian(30));

		float angle = CE_MATH::Rad2Deg * fabs(_curRadian.y - _goalRadian.y);
		_lerpEndTime = angle * 0.01f;

		_moveDirection = -XMVector3Normalize(transform->GetRight()) * 0.5f;
	}
	break;

	case Direction::RIGHT_DOWN:
	{
		_animator->Play("FlyingMachine_ao|FlyingMachine_Bhv_Tree_FlyingMachine_SteerLeft", true);

		_goalRadian = Vector3(CE_MATH::ToRadian(20), _curRadian.y + CE_MATH::ToRadian(90), CE_MATH::ToRadian(-30));

		float angle = CE_MATH::Rad2Deg * fabs(_curRadian.y - _goalRadian.y);
		_lerpEndTime = angle * 0.01f;

		_moveDirection = -XMVector3Normalize(transform->GetRight()) * 0.5f;
	}
	break;

	case Direction::LEFT:
	{
		_animator->Play("FlyingMachine_ao|FlyingMachine_Bhv_Tree_FlyingMachine_SteerLeft", true);

		_goalRadian = Vector3(_curRadian.x, _curRadian.y + CE_MATH::ToRadian(-90), CE_MATH::ToRadian(30));

		float angle = CE_MATH::Rad2Deg * fabs(_curRadian.y - _goalRadian.y);
		_lerpEndTime = angle * 0.01f;

		_moveDirection = -XMVector3Normalize(transform->GetRight());
	}
	break;

	case Direction::RIGHT:
	{	_animator->Play("FlyingMachine_ao|FlyingMachine_Bhv_Tree_FlyingMachine_SteerRight", true);

		_goalRadian = Vector3(_curRadian.x, _curRadian.y + CE_MATH::ToRadian(90), CE_MATH::ToRadian(-30));

		float angle = CE_MATH::Rad2Deg * fabs(_curRadian.y - _goalRadian.y);
		_lerpEndTime = angle * 0.01f;

		_moveDirection = XMVector3Normalize(transform->GetRight());
	}
	break;

	case Direction::END:
		_isLerp = false;
		_animator->Play("FlyingMachine.ao|FlyingMachine_Bhv_Tree_FlyingMachine_Mh", true);

		_goalRadian = Vector3(_curRadian.x, _curRadian.y, 0);

		//if (prevDir != Direction::LEFT && prevDir != Direction::RIGHT)
		//{
		//	_easeType = CETween::EaseType::easeInOutCubic;
		//	_isLerp = true;
		//	_lerpEndTime = CE_MATH::Rad2Deg * 5 * 0.01f;

		//	//이전 방향 상승 
		//	if ((int)prevDir >= 0 && (int)prevDir < 3)
		//	{
		//		_goalRadian.x = _curRadian.x + CE_MATH::ToRadian(5);
		//	}
		//	//이전 방향 하강
		//	else
		//	{
		//		_goalRadian.x = _curRadian.x - CE_MATH::ToRadian(5);
		//	}
		//}

		if (_curRadian.z != 0)
		{
			_isLerp = true;
			_lerpEndTime = CE_MATH::Rad2Deg * fabs(_curRadian.z) * 0.01f;
		}

		break;
	}

	//회전 제한
	//if (CE_MATH::ToRadian(80) < fabs(_goalRadian.y))
	//{
	//	if(_goalRadian.y >0)
	//		_goalRadian.y = CE_MATH::ToRadian(80);
	//	else
	//		_goalRadian.y = -CE_MATH::ToRadian(80);
	//}
	if (CE_MATH::ToRadian(80) < fabs(_goalRadian.z))
	{
		if (_goalRadian.z > 0)
			_goalRadian.z = CE_MATH::ToRadian(80);
		else
			_goalRadian.z = -CE_MATH::ToRadian(80);
	}

	if (_usingBoost)
		_lerpEndTime *= 0.5f;
}

void FlyingMachine::GetHit(float damage, int8 attacker) noexcept
{
	_hp = std::max(0.f, _hp - damage);
	if (_hp == 0)
	{
		Die();
	}
}

void FlyingMachine::Die(void) noexcept
{
	if (_state == Actor::State::DIE)
		return;

	_state = Actor::State::DIE;

	_isDead = true;
	_isDeath = true;

	NarrationManager::This().Stop();
	BGMPlayer::This()->Stop();
	_propellerPlayer->Stop();
	_damagePlayer->Play(_audioAsset[static_cast<uint>(AudioID::DAMAGE)], 0.5f);

	_turret->Die();

	_rigidbody->SetDrag(1);
	_rigidbody->SetAngularDrag(1);
	_rigidbody->SetUseGravity(true);

	PlayerScreen::This().ShowOnlyL();
	_statusUI->SetVisibleHP(false);

	// fx
	GameObject* fxObj = GameObject::Instantiate("Player Dead Explosion Fx");
	ExplosionFX* fx = new ExplosionFX();
	fxObj->AddComponent(fx);
	fx->StartFX(transform->GetWorldPosition());

	SquirrelTurretHolder::SetExplode(false);
	TunnelDoor::SetCloseNarration(false);

	FadeController::This()->StartFadeOut( 3.0f
										, true
										, false
										, [&]() {
											RestartScene::SetNextSceneName("Tree");
											SCENEMANAGER->LoadScene("Restart");
										});
}

void FlyingMachine::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	if (_isDead)
		return;

	GameObject* obj = other->GetGameObject();
	if (obj->GetTag() == GameObjectTag::WALL)
	{
		Die();
	}
	if (obj->GetTag() == GameObjectTag::BURNABLE)
	{
		GetHit(2,0);
		obj->GetComponent<Burnable>()->Burn();
	}

}

void FlyingMachine::CreateAfterBurnerFX(void) noexcept
{
	HierarcyNode* afterburnerBone = _renderer->GetModel()->GetHierarchyNode("Afterburner");
	XMMATRIX offset = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(CE_MATH::ToRadian(-90));

	GameObject* t = GameObject::Instantiate("AfterBurnerFX_0");
	t->GetTransform()->SetParent(transform);
	t->GetTransform()->SetLocalPosition(0, 0, 0);

	t->AddComponent(new BoneChildObject(afterburnerBone, offset, XMMatrixTranslation(0, 0, -0.5f)));

	_particle[0] = ParticleSystemAnimation::Create();
	_particle[0]->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\FlyingMachineEngine\\Dragon_Flames_TextureSheet.dds"));
	_particle[0]->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.1686275f, 0.7607843f, 1, 1));
	_particle[0]->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\FlyingMachineEngine\\Dragon_Flames_TextureSheet.dds"));
	_particle[0]->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(0.1686275f, 0.7607843f, 1, 1));
			 
	_particle[0]->SetStartColor(Vector4(0.1686275f, 0.7607843f, 1, 1));
	_particle[0]->SetEndColor(Vector4(1, 0.7f, 0, 1));
			 
	_particle[0]->SetStartEmissiveColor(Vector4(0.1686275f, 0.7607843f, 1, 1));
	_particle[0]->SetEndEmissiveColor(Vector4(1, 0.7f, 0, 1));
			 
			 
	_particle[0]->SetColorLerpType(CETween::EaseType::easeOutExpo);
	_particle[0]->SetSizeLerpType(CETween::EaseType::easeOutExpo);
			 
	_particle[0]->SetRange(Vector3::zero);
	_particle[0]->SetVelocity(0);
	_particle[0]->SetParticlePerSecond(0.001f);
	_particle[0]->SetLifeTime(0.05f);
	_particle[0]->SetRange(Vector3(1.f, 1.f, 0));
	_particle[0]->SetPass(ParticleSystemAnimation::PASS::ONEBLEDING_MSAA);
	_particle[0]->SetAmount(XMFLOAT2(8, 8));


	t->AddComponent(_particle[0]);
	t->SetLayer(GameObjectLayer::TRANSLUCENT);

	/*  ======================================================================================================================== */

	t = GameObject::Instantiate("AfterBurnerFX_1");
	t->GetTransform()->SetParent(transform);
	t->GetTransform()->SetLocalPosition(0, 0, 0);

	t->AddComponent(new BoneChildObject(afterburnerBone, offset, XMMatrixTranslation(0, 0, -3.f)));

	_particle[1] = ParticleSystemAnimation::Create();
	_particle[1]->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\FlyingMachineEngine\\Dragon_Flames_TextureSheet.dds"));
	_particle[1]->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.1686275f, 0.7607843f, 1, 1));
	
	_particle[1]->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE
										, ASSETMANAGER->GetTextureData("Asset\\Texture\\FlyingMachineEngine\\Dragon_Flames_TextureSheet.dds"));
	_particle[1]->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1, 0.3f, 0, 0.3f));

	_particle[1]->SetStartColor(Vector4(0.1686275f, 0.7607843f, 1, 1));
	_particle[1]->SetEndColor(Vector4(0, 0, 0, 0));

	_particle[1]->SetStartEmissiveColor(Vector4(0.1686275f, 0.7607843f, 1, 1));
	_particle[1]->SetEndEmissiveColor(Vector4(0, 0, 0, 0));


	_particle[1]->SetColorLerpType(CETween::EaseType::easeInExpo);
	_particle[1]->SetSizeLerpType(CETween::EaseType::easeOutExpo);

	_particle[1]->SetRange(Vector3::zero);
	_particle[1]->SetVelocity(0);
	_particle[1]->SetParticlePerSecond(0.001f);
	_particle[1]->SetLifeTime(0.2f);
	_particle[1]->SetRange(Vector3(1, 1, 0));
	_particle[1]->SetPass(ParticleSystemAnimation::PASS::ONEBLEDING_MSAA);
	_particle[1]->SetAmount(XMFLOAT2(8, 8));

	t->AddComponent(_particle[1]);
	t->SetLayer(GameObjectLayer::TRANSLUCENT);

	/*  ======================================================================================================================== */

}

void FlyingMachine::OnEndBoost(void) noexcept
{
	if (!_usingBoost)
		return;

	_usingBoost = false;
	_boostCool = true;

	_speed *= 0.5f;

	_boostTime = 0;
	_boostCoolTime = 0;

	_animator->Play("FlyingMachine.ao|FlyingMachine_Bhv_Tree_FlyingMachine_Mh", true);

	_particle[0]->Stop();
	_particle[1]->Stop();
}

void FlyingMachine::FixedUpdate(float fElapsedTime) noexcept
{
	if (!_propellerOn)
	{
		_propellerOn = true;
		_propellerPlayer->Play(_audioAsset[static_cast<uint>(AudioID::PROPELLER)], 0.05f, true);
	}

}
