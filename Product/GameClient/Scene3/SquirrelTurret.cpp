#include "pch.h"
#include "SquirrelTurret.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Animator.h"
//#include "SquirrelBullet.h"
#include "Camera.h"
#include "BoneChildObject.h"
#include "AcornShooter.h"

SquirrelTurret::SquirrelTurret(void) noexcept
{
}

SquirrelTurret::~SquirrelTurret(void) noexcept
{
}

void SquirrelTurret::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create(false);
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Tree\\Defense\\SquirrelTurret_Eye.fbx"));
	gameObject->AddComponent(renderer);

	XMMATRIX mat = XMMatrixScaling(0.01f, 0.01f, 0.01f)
									* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(0)
										, CE_MATH::ToRadian(90)
										, CE_MATH::ToRadian(0)));
	renderer->SetOffsetMatrix(mat);

	HierarcyNode* leftEye = renderer->GetModel()->GetHierarchyNode("Left");
	HierarcyNode* rightEye = renderer->GetModel()->GetHierarchyNode("Right");

	GameObject* obj = GameObject::Instantiate();
	_leftEye = obj->GetTransform();
	_leftEye->SetParent(transform);

	_left = new AcornShooter(GameObjectTag::PLAYER);

	obj->AddComponent(new BoneChildObject(leftEye, mat));
	obj->AddComponent(_left);

	obj = GameObject::Instantiate();
	_rightEye = obj->GetTransform();
	_rightEye->SetParent(transform);

	_right = new AcornShooter(GameObjectTag::PLAYER);

	obj->AddComponent(new BoneChildObject(rightEye, mat));
	obj->AddComponent(_right);
}

void SquirrelTurret::Update(float fElapsedTime) noexcept
{
	if (_playerTransform == nullptr)
	{
		GameObject* player = GameObject::FindObjectByTag(GameObjectTag::PLAYER);

		if (player == nullptr)
			return;

		_playerTransform = player->GetTransform();
	}
	else
	{
		XMVECTOR direction = XMVector3Normalize(_playerTransform->GetWorldPosition() - transform->GetWorldPosition());
		float dot = XMVectorGetX(XMVector3Dot(direction, XMVectorSet(0, 1, 0, 0)));
		float angle = acosf(dot);

		if (fabsf(angle) >= 0.2f)
		{
			XMVECTOR cross = XMVector3Cross(XMVectorSet(0, 1, 0, 0), direction);
			if (XMVectorGetY(cross) < 0)
				angle *= -1;

			transform->SetLocalEulerAngle(0, 0, (angle - CE_MATH::ToRadian(90.f)) * -1);
		}

		if (XMVectorGetX(XMVector3Length(_playerTransform->GetWorldPosition() - transform->GetWorldPosition())) < 300)
		{
			_time += fElapsedTime;

			if (_time >= 0.7f)
			{
				_left->Play(5, 100, direction);
				_time = 0.f;
			}
			else if (_time >= 0.35f && _time <= 0.365f)
			{
				_right->Play(5, 100, direction);
			}
		}
	}
}

void SquirrelTurret::FixedUpdate(float fElapsedTime) noexcept
{
}

void SquirrelTurret::LateUpdate(float fElapsedTime) noexcept
{
}
