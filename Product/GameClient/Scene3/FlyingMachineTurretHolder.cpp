#include "pch.h"
#include "FlyingMachineTurretHolder.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "TurretCamera.h"
#include "FlyingMachineTurret.h"
#include "Crosshair.h"
#include "BoneChildObject.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "OptionPopup.h"

FlyingMachineTurretHolder::FlyingMachineTurretHolder(const bool bOperationPad) noexcept
	: _bOperationPad(bOperationPad)
{
}

void FlyingMachineTurretHolder::Start(void) noexcept
{
	_parentTrans = transform->GetParent()->GetParent();
	
	transform->SetLocalEulerAngle(0, CE_MATH::ToRadian(90), 0);
	/* Model */
	ModelRenderer* renderer = ModelRenderer::Create(false);
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Item\\FlyingMachineTurretBottom.fbx"));
	XMMATRIX offset = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(CE_MATH::ToRadian(-90));
	renderer->SetOffsetMatrix(offset);
	gameObject->AddComponent(renderer);

	/* Camera */
	GameObject* camera = GameObject::Instantiate("May Camera");
	_cameraTrans = camera->GetTransform();

	GameObject* turretObj = GameObject::Instantiate("Turret");
	turretObj->GetTransform()->SetParent(transform);
	turretObj->GetTransform()->SetLocalEulerAngle(0, 0, 0);
	turretObj->GetTransform()->SetLocalPosition(0, 1.517f, 0);
	FlyingMachineTurret* turret = new FlyingMachineTurret(_bOperationPad, _cameraTrans);
	turretObj->AddComponent(turret);

	camera->AddComponent(new TurretCamera(_bOperationPad, turretObj->GetTransform()));

	/* Crosshair - 카메라가 필요하므로 카메라 보다 하위에 생성되어야함 */ 
	GameObject* obj = GameObject::Instantiate("Crosshair");
	_crosshair = new Crosshair(_bOperationPad);
	obj->AddComponent(_crosshair);

	turret->SetCrosshair(_crosshair);

	_moveAngle = 0;
	_startAngle = CE_MATH::ToRadian(90);
}

void FlyingMachineTurretHolder::Update(float fElapsedTime) noexcept
{
	if (OptionPopup::This()->IsShow())
		return;

	transform->SetLocalPosition(0, 0, 0);

	float angle = 0;
	if (!_bOperationPad)
	{
		Vector3 mousePos = INPUT->GetMouseMoveDelta();
		angle = mousePos.x * 0.5f * fElapsedTime;
	}
	else
	{
		angle = PADINPUT->GetLeftStickX() * 0.5f * fElapsedTime;
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

	float machineAngleY = XMVectorGetY(_parentTrans->GetWorldEulerAngle());

	if (_move)
	{

		float angle = CETween::Lerp(0, abs(_moveAngle), 0.2f);

		if (_moveAngle< 0)
			angle *= -1;

		transform->SetLocalEulerAngle(rot.x, (_startAngle + angle)- machineAngleY, rot.z);

			_move = false;
			_startAngle += _moveAngle;
			_moveAngle = 0;
	}
	else
	{
		if (machineAngleY != 0)
		{
			transform->SetLocalEulerAngle(rot.x, (_startAngle - machineAngleY), rot.z);
		}
	}

	transform->ImmediateUpdate();
}

void FlyingMachineTurretHolder::Die(void) noexcept
{
	_crosshair->Invisible();
}