#include "pch.h"
#include "BaseballMay.h"
#include "GameObject.h"
#include "Animator.h"
#include "Rigidbody.h"
#include "PlayerInput.h"
#include "ModelRenderer.h"
#include "Transform.h"

#include "BaseballLever.h"


BaseballMay::BaseballMay(bool bOperationPad) noexcept
{
	_lever = GameObject::FindLoadingObjectByName("Toy02Crank")->GetComponent<BaseballLever>();
}

BaseballMay::~BaseballMay(void) noexcept
{
}

void BaseballMay::Start(void) noexcept
{
	_renderer = ModelRenderer::Create(false);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\BattingTeam\\Players\\BaseBallMay.fbx"));
	gameObject->AddComponent(_renderer);

	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-96.919f));
	_renderer->SetOffsetMatrix(offsetMat);

	Material::TList& mats = _renderer->GetMaterials();
	for (Material& mat : mats)
	{
		mat.SetColor(ShaderSemantic::Type::COLOR_DEPTHEFFECT, Vector4(0, 0.f, 1.f, 0.5f));
	}

	
	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play("May.ao|May_Bhv_PlayRoom_BaseBallToy_mh", true);
	_animator->SetPlaySpeed(1);


	gameObject->SetTag(GameObjectTag::PLAYER);
	gameObject->SetLayer(GameObjectLayer::UNIT);
}

void BaseballMay::FixedUpdate(float fElapsedTime) noexcept
{
}

void BaseballMay::Update(float fElapsedTime) noexcept
{
	if (INPUT->GetKeyDown(VK_SPACE))
	{
		if (_animator->GetCurrentAnimationName() != "May.ao|BaseBallMay")
		{ 
			_animator->Play("May.ao|BaseBallMay", false);
			_animator->SetPlaySpeed(0.8);
			_lever->SetLeverDown(gameObject);
		}
	}

}

void BaseballMay::LateUpdate(float fElapsedTime) noexcept
{
}

