#include "pch.h"
#include "FlyingMachineTurret.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Animator.h"
#include "BoneChildObject.h"
#include "AcornShooter.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Crosshair.h"
#include "Raycast.h"
#include "OptionPopup.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

FlyingMachineTurret::FlyingMachineTurret( const bool bOperationPad
										, Transform* cameraTrans) noexcept
	: _bOperationPad(bOperationPad)
	, _cameraTrans(cameraTrans)
{
}

void FlyingMachineTurret::Start(void) noexcept
{
	/* Turret */
	/* Model */
	ModelRenderer* renderer = ModelRenderer::Create(false);
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Item\\FlyingMachineTurretTop.fbx"));
	XMMATRIX offset = XMMatrixScaling(0.01f, 0.01f, 0.01f) *XMMatrixRotationY(CE_MATH::ToRadian(-90));
	renderer->SetOffsetMatrix(offset);
	gameObject->AddComponent(renderer);

	/* May */
	GameObject* may = GameObject::Instantiate("Turret May");
	may->GetTransform()->SetParent(transform);

	// BoneChild 
	HierarcyNode* bone = renderer->GetModel()->GetHierarchyNode("Turret");
	BoneChildObject* boneChild = new BoneChildObject(bone, offset);
	may->AddComponent(boneChild);

	// Model 
	ModelRenderer* mayRenderer = ModelRenderer::Create(false);
	mayRenderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\May\\May_Turret.fbx"));
	XMMATRIX mayOffset = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationX(CE_MATH::ToRadian(-90)) * XMMatrixRotationZ(CE_MATH::ToRadian(180)) * XMMatrixRotationZ(CE_MATH::ToRadian(-90));//// 
	mayRenderer->SetOffsetMatrix(mayOffset);
	may->AddComponent(mayRenderer);

	// Animator 
	Animator* animator = Animator::Create();
	may->AddComponent(animator);
	animator->Play("May.ao|May_Bhv_Tree_FlyingMachine_Mh", true);

	/* Left Muzzle */
	GameObject* left = GameObject::Instantiate("Left Muzzle");
	left->GetTransform()->SetParent(transform);

	bone = renderer->GetModel()->GetHierarchyNode("LeftMuzzle");
	boneChild = new BoneChildObject(bone, offset);
	left->AddComponent(boneChild);
	left->GetTransform()->SetLocalPosition(0, 0, 0);

	Rigidbody* rigid = Rigidbody::Create(true);
	rigid->SetUseGravity(false);
	left->AddComponent(rigid);

	Collider* coll = Collider::CreateSphereCollider(0.1f);
	coll->SetIsTrigger(true);
	coll->SetQueryEnable(false);
	left->AddComponent(coll);

	_leftShooter = new AcornShooter(GameObjectTag::MONSTER);
	left->AddComponent(_leftShooter);

	/* Right Muzzle */
	GameObject* right = GameObject::Instantiate("Right Muzzle");
	right->GetTransform()->SetParent(transform);

	bone = renderer->GetModel()->GetHierarchyNode("RightMuzzle");
	boneChild = new BoneChildObject(bone, offset);
	right->AddComponent(boneChild);
	right->GetTransform()->SetLocalPosition(0, 0, 0);

	rigid = Rigidbody::Create(true);
	rigid->SetUseGravity(false);
	right->AddComponent(rigid);

	coll = Collider::CreateSphereCollider(0.1f);
	coll->SetIsTrigger(true);
	coll->SetQueryEnable(false);
	right->AddComponent(coll);

	_rightShooter = new AcornShooter(GameObjectTag::MONSTER);
	right->AddComponent(_rightShooter);

	_machineTrans = transform->GetParent()->GetParent()->GetParent();

	_move = false;

	_moveAngle = 0;
	_startAngle = 0;

	_leftFire = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_leftFire);

	_rightFire = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_rightFire);

	_audioAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage3\\SquirrelMachineGun_ShotFired.ogg");
}

void FlyingMachineTurret::Update(float fElapsedTime) noexcept
{
	if (OptionPopup::This()->IsShow())
		return;

	transform->SetLocalPosition(0, 1.517f, 0);

	if (_bOperationPad)
	{
		if (PADINPUT->GetButtonDown(PadInput::PadButton::BUTTON_B))
		{
			//크로스 헤어 위치로 레이캐스트
			Vector3 pos = _cross->ShootFromCross();

			//적중된 것이 없음
			if (pos == Vector3::zero)
			{
				if (!_isRight)
					_leftShooter->Play(3, 600);
				else
					_rightShooter->Play(3, 600);
			}
			else
			{
				if (!_isRight)
				{
					XMVECTOR dir = (XMVECTOR)pos - _leftShooter->GetTransform()->GetWorldPosition();
					_leftShooter->Play(3, 600, XMVector3Normalize(dir));
				}
				else
				{
					XMVECTOR dir = (XMVECTOR)pos - _rightShooter->GetTransform()->GetWorldPosition();
					_rightShooter->Play(3, 600, XMVector3Normalize(dir));
				}
			}

			if (!_isRight)
			{
				_leftFire->Play(_audioAsset, 0.1f);
			}
			else
			{
				_rightFire->Play(_audioAsset, 0.1f);
			}

			_isRight = !_isRight;
		}
	}
	else
	{
		if (INPUT->GetKeyDown(VK_LBUTTON))
		{
			//크로스 헤어 위치로 레이캐스트
			Vector3 pos = _cross->ShootFromCross();
			
			//적중된 것이 없음
			if (pos == Vector3::zero)
			{
				if (!_isRight)
					_leftShooter->Play(3, 600);
				else
					_rightShooter->Play(3, 600);
			}
			else
			{
				if (!_isRight)
				{
					XMVECTOR dir = (XMVECTOR)pos - _leftShooter->GetTransform()->GetWorldPosition();
					_leftShooter->Play(3, 600, XMVector3Normalize(dir));
				}
				else
				{
					XMVECTOR dir = (XMVECTOR)pos - _rightShooter->GetTransform()->GetWorldPosition();
					_rightShooter->Play(3, 600, XMVector3Normalize(dir));
				}
			}
		
			if (!_isRight)
			{
				_leftFire->Play(_audioAsset, 0.1f);
			}
			else
			{
				_rightFire->Play(_audioAsset, 0.1f);
			}

			_isRight = !_isRight;
		}
	}
}

void FlyingMachineTurret::LateUpdate(float fElapsedTime) noexcept
{
	if (OptionPopup::This()->IsShow())
		return;

	float angle = 0;
	if (!_bOperationPad)
	{
		Vector3 mousePos = INPUT->GetMouseMoveDelta();
		angle = mousePos.y * 0.5f * fElapsedTime;
	}
	else
	{
		angle = PADINPUT->GetLeftStickY() *0.5f * fElapsedTime;
	}

	if (angle != 0)
	{
		float value = _moveAngle + angle;
		if (fabs(value) != 0)
		{
			//이미 회전 중이었음 
			if (_move)
			{
				float f = CETween::Lerp(0, abs(_moveAngle), 0.2f);
				if (_moveAngle < 0)
					f *= -1;

				_startAngle += f;
				_moveAngle += angle - f;
			}
			else
				_moveAngle = angle;

			_move = true;
		}
		else
			_move = false;
	}

	float3 rot;
	XMStoreFloat3(&rot, transform->GetLocalEulerAngle());

	float machineAngleX = XMVectorGetX(_machineTrans->GetWorldEulerAngle());

	if (_move)
	{
		float angle = CETween::Lerp(0, abs(_moveAngle), 0.2f);

		if (_moveAngle < 0)
			angle *= -1;

		float f = (_startAngle + angle);
		float degree = XMConvertToDegrees(f);
		if (degree > 40.f)
		{
			f = XMConvertToRadians(40.f);

			if(_moveAngle > 0)
				_moveAngle = 0;
		}
		else if (degree < -25.f)
		{
			f = XMConvertToRadians(-25.f);

			if(_moveAngle <0)
				_moveAngle = 0;
		}

		transform->SetLocalEulerAngle(f - machineAngleX, rot.y, rot.z);

		_move = false;
		_startAngle = f;
		_moveAngle = 0;

	}
	else
	{
		if (machineAngleX != 0)
		{
			transform->SetLocalEulerAngle((_startAngle - machineAngleX), rot.y, rot.z);
		}
	}

}

void FlyingMachineTurret::SetCrosshair(Crosshair* crosshair) noexcept
{
	_cross = crosshair;
}
