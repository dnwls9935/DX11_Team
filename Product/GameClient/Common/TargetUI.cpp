#include "pch.h"
#include "TargetUI.h"
#include "FollowingCamera.h"
#include "Image.h"
#include "Actor.h"
#include "GameObject.h"
#include "Transform.h"
#include "RectTransform.h"
#include "Camera.h"
/* 어떤 actor의 위치를 표시 camera에다가 */
TargetUI::TargetUI(Actor* actor, FollowingCamera* camera) noexcept
	: _actor(actor)
	, _camera(camera)
{

}

void TargetUI::Start(void) noexcept
{
	GameObject* obj = GameObject::Instantiate("TargetUI");
	obj->GetTransform()->SetParent(_actor->GetTransform());

	UI::Image* targetui = UI::Image::Create();
	targetui->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Player\\PlayerMarker.dds"));
	targetui->SetColor(Vector4(0.2f, 0.6f, 1.f, 1.f));
	obj->AddComponent(targetui);
	obj->GetComponent<RectTransform>()->SetWidth(100);
	obj->GetComponent<RectTransform>()->SetHeight(100);
	obj->GetComponent<RectTransform>()->SetPivot(0, 0);
	obj->GetTransform()->SetLocalPosition(0, 0, 0);
	obj->SetSortOrder(10);


}

void TargetUI::Update(float fElapsedTime) noexcept
{
	if (_camera == nullptr) return;

	/* Set Position */
	//XMMATRIX viewProjMat =  Camera::GetMainCamera()->GetViewMatrix() * Camera::GetMainCamera()->GetProjMatrix();
	/*Vector3	pos = _actor->GetTransform()->GetWorldPosition() ;
	XMMATRIX view = Camera::GetMainCamera()->GetViewMatrix();
	ZeroMemory(&view.r[3], sizeof(XMVECTOR));
	XMVECTOR vecpos = XMVector3Project((XMVECTOR)pos, 495, 0, 1485, 1080, 0.f, 1.f, Camera::GetMainCamera()->GetProjMatrix() ,view,Camera::GetMainCamera()->GetWorldMatrix());*/
	/*Vector3 worldPos = XMVector3TransformCoord((XMVECTOR)pos, viewProjMat);
	worldPos.x = ((worldPos.x + 1.f) * 0.5) * WINCX / 2.f;
	worldPos.y = ((2.f - worldPos.y + 1.f) * 0.5f - 1.f) * WINCY / 2.f ;
	worldPos.z = 0;*/

	//gameObject->GetTransform()->SetWorldPosition((XMVECTOR)worldPos);
	//gameObject->GetTransform()->SetWorldPosition(vecpos);
	//gameObject->GetTransform()->SetLocalPosition(vecpos);
	//gameObject->GetTransform()->SetWorldPosition(vecpos);


}

void TargetUI::FixedUpdate(float fElapsedTime) noexcept
{
}

void TargetUI::LateUpdate(float fElapsedTime) noexcept
{
}
