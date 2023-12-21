#include "pch.h"
#include "PlateButton.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"

PlateButton::PlateButton(bool isReturn, std::function<void()> onEnterEvent, std::function<void()> onExitEvent) noexcept
	: _isReturn(isReturn)
	, _onEnterEvent(onEnterEvent)
	, _onExitEvent(onExitEvent)
	, _isPush(false)
	, _isPop(false)
	, _time(0)
	, _localY(0)
{
}

void PlateButton::Start(void) noexcept
{
	gameObject->SetTag(GameObjectTag::OBJECT);
	gameObject->SetLayer(GameObjectLayer::OBJECT);

	/* Frame Model */
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Pressure_Plate\\Pressure_Plate_Frame.fbx"));
	gameObject->AddComponent(renderer);

	/* Rigidbody */
	Rigidbody* rigidbody = Rigidbody::Create(false);
	rigidbody->SetIsKinematic(true);
	gameObject->AddComponent(rigidbody);

	/* Collider */
	Collider* collider = Collider::CreateBoxCollider(0.7f, 0.1f, 0.7f, XMVectorSet(0, 0.2f, 0, 0));
	collider->SetIsTrigger(true);
	gameObject->AddComponent(collider);

	/* Button Model */
	GameObject* button = GameObject::Instantiate("Button");
	_buttonTrans = button->GetTransform();
	_buttonTrans->SetParent(transform);
	_buttonTrans->SetLocalPosition(0, 0, 0);

	renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Pressure_Plate\\Pressure_Plate.fbx"));
	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	renderer->SetOffsetMatrix(offsetMat);
	button->AddComponent(renderer);
}

void PlateButton::Update(float fElapsedTime) noexcept
{

	if (_enterCount> 0)
	{
		if (_isPush == false) return;

		_time += fElapsedTime;
		_buttonTrans->SetLocalPosition(0, CETween::Lerp(_localY, -0.3f, std::min(1.f,_time)), 0);

		if (_time >= 1)
		{
			_localY = -0.3f;
			_isPush = false;
			_isEnd = true;
		}
	}
	else if(true == _isReturn)
	{
		_time += fElapsedTime;
		_buttonTrans->SetLocalPosition(0, CETween::Lerp(_localY, 0, std::min(1.f,_time)), 0);

		if (_time >= 1)
		{
			_localY = 0;
			_isPop = false;
		}
	}
}

void PlateButton::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	if (_isReturn == false && _isEnd == true)
		return;

	GameObject* obj = other->GetGameObject();
	if (obj->GetTag() == GameObjectTag::PLAYER)
	{
		_enterCount++;

		if (_enterCount > 1) return;

		_time = 0;
		_isPop = false;
		_isPush = true;

		if (_onEnterEvent != nullptr)
			_onEnterEvent();

		_localY = XMVectorGetY(transform->GetLocalPosition());
	}
}

void PlateButton::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
	if (false == _isReturn) return;

	GameObject* obj = other->GetGameObject();
	if (obj->GetTag() == GameObjectTag::PLAYER)
	{
		_enterCount--;

		if (_enterCount > 0) return;

		_time = 0;
		_isPush = false;
		_isPop = true;

		if (_onExitEvent != nullptr)
			_onExitEvent();

		_localY = XMVectorGetY(_buttonTrans->GetLocalPosition());
	}
}
