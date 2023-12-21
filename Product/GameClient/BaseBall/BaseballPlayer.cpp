#include "pch.h"
#include "BaseballPlayer.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ModelRenderer.h"

#include "BaseballBall.h"

BaseballPlayer::BaseballPlayer(bool ismay) noexcept
	: _ismay(ismay)
{


}

void BaseballPlayer::Start(void) noexcept
{
	//ModelRenderer* renderer = ModelRenderer::Create(false);
	//
	//if (_ismay)
	//{
	//	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\BattingTeam\\ToyBody01.fbx"));
	//	gameObject->AddComponent(renderer);
	//	transform->SetLocalPosition(-0.921, 2.42, -9.019);
	//	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(-90)
	//																															, 0
	//																															, CE_MATH::ToRadian(124.752)));
	//	renderer->SetOffsetMatrix(offsetMat);
	//}
	//else
	//{
	//	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\BattingTeam\\ToyBody02.fbx"));
	//	gameObject->AddComponent(renderer);
	//		transform->SetLocalPosition(10.5, 2.42, -9.11);
	//	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(-90)
	//																															, 0
	//																															, CE_MATH::ToRadian(-318.59)));
	//
	//	renderer->SetOffsetMatrix(offsetMat);
	//}
}

void BaseballPlayer::Update(float fElapsedTime) noexcept
{

}


