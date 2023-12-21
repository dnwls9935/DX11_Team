#include "pch.h"
#include "Crosshair.h"
#include "GameObject.h"
#include "Transform.h"
#include "Image.h"
#include "RectTransform.h"
#include "Camera.h"
#include "Raycast.h"

Crosshair::Crosshair(bool bOperationPad) noexcept
	:_bOperationPad(bOperationPad)
	, _camera(nullptr)
{
}

void Crosshair::Start(void) noexcept
{
	/* Circle */
	/* Image */
	UI::Image* circle = UI::Image::Create();
	circle->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Stage3\\Crosshair_Circle.dds"));
	gameObject->AddComponent(circle);
	gameObject->GetComponent<RectTransform>()->SetPivot(0.5f, 0.5f);
	gameObject->SetSortOrder(15);

	transform->SetLocalScale(0.5f, 0.5f, 1);
	transform->SetLocalPosition(WINCX * 0.25f + 25, WINCY * 0.5f + 25, 0);

	/* Crosshair */
	GameObject* obj = GameObject::Instantiate("Cross");
	_crossTrans = obj->GetTransform();
	_crossTrans->SetParent(transform);

	UI::Image* cross = UI::Image::Create();
	cross->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Stage3\\Crosshair_Cross.dds"));
	obj->AddComponent(cross);
	obj->GetComponent<RectTransform>()->SetPivot(0.5f, 0.5f);
	obj->SetSortOrder(14);

	_crossTrans->SetLocalScale(0.5f, 0.5f, 1);
	_crossTrans->SetLocalPosition(17, 17, 0);

	_mousePos = INPUT->GetMousePosition();

}

void Crosshair::Update(float fElapsedTime) noexcept
{
	if (_camera == nullptr)
	{
		_camera = Camera::GetMainCamera();
		_cameraTrans = _camera->GetTransform();
	}

	if (!_bOperationPad)
	{
		Vector3 mousePos = INPUT->GetMouseMoveDelta(); /* 마우스 이동값 */

		mousePos *= 100 * fElapsedTime;
		_crossTrans->Translate(-mousePos.x, -mousePos.y, 0);	/* 마우스 이동량 만큼 cross 이동 */

		_movePos = _crossTrans->GetLocalPosition();

		Vector3 dist = Vector3(17, 17, 0) - _movePos;
		_length = dist.Length();

	}
	else
	{
		Vector3 padPos;
		padPos.x = PADINPUT->GetLeftStickX();
		padPos.y = PADINPUT->GetLeftStickY();

		padPos *= 100 * fElapsedTime;
		_crossTrans->Translate(-padPos.x, -padPos.y, 0);	/* 마우스 이동량 만큼 cross 이동 */

		_movePos = _crossTrans->GetLocalPosition();
	}

	Vector3 pos = _crossTrans->GetLocalPosition();
	float x = pos.x;
	float y = pos.y;

	x = CETween::Lerp(_movePos.x, 17, 0.2f);
	y = CETween::Lerp(_movePos.y, 17, 0.2f);

	_crossTrans->SetLocalPosition(x, y, 0);


}

void Crosshair::Invisible(void) noexcept
{
	gameObject->SetActive(false);
}

Vector3 Crosshair::ShootFromCross(void) noexcept
{
	if (_camera == nullptr)
		return Vector3();
	//crosshair 2D > 3D
	Vector3 crossPos = _crossTrans->GetWorldPosition();
	// crosshair를 중앙으로 맞추기 위했던 보정 값 추가 계산
	crossPos.x = (crossPos.x * 2) - 84; 
	crossPos.y -= 84;

	/* 뷰스페이스로 변환 */
	XMMATRIX projMat = _camera->GetProjMatrix();
	Vector3 direction;
	direction.x = (((2.f * crossPos.x) / WINCX - 1.f))/ XMVectorGetX(projMat.r[0]);
	direction.y = (((-2.f * crossPos.y) / WINCY + 1.f)) / XMVectorGetY(projMat.r[1]);
	direction.z = 1.f;

	/* 월드 스페이스로 변환 */
	XMVECTOR world = (XMVECTOR)direction;
	XMMATRIX invViewMat = _camera->GetTransposedViewMatrix();
	XMVECTOR dir = XMVector3TransformNormal(world, invViewMat);
	dir = XMVector3Normalize(dir);


	RaycastHit hit;

	if (Physics::Raycast( hit
						, _cameraTrans->GetWorldPosition()
						, dir
						, 1000.f))
	{
		return hit.position;
	}

	return Vector3();
}
