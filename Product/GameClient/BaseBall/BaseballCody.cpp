#include "pch.h"
#include "BaseballCody.h"
#include "GameObject.h"
#include "Animator.h"
#include "Rigidbody.h"
#include "PlayerInput.h"
#include "ModelRenderer.h"
#include "Transform.h"

#include "BaseballLever.h"

BaseballCody::BaseballCody(bool bOperationPad) noexcept
{
	_lever = GameObject::FindLoadingObjectByName("Toy01Crank")->GetComponent<BaseballLever>();
}

BaseballCody::~BaseballCody(void) noexcept
{
}

void BaseballCody::Start(void) noexcept
{
	_renderer = ModelRenderer::Create();
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\BattingTeam\\Players\\BaseBallCody.fbx"));
	gameObject->AddComponent(_renderer);

	XMMATRIX offsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(-96.919f));
	_renderer->SetOffsetMatrix(offsetMat);

	Material::TList& mats = _renderer->GetMaterials();
	for (Material& mat : mats)
	{
		mat.SetColor(ShaderSemantic::Type::COLOR_DEPTHEFFECT, Vector4(0, 1.f, 0, 0.5f));
	}

	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play("Cody.ao|Cody_Bhv_PlayRoom_BaseBallToy_mh", true);
	_animator->SetPlaySpeed(1);

	gameObject->SetTag(GameObjectTag::PLAYER);
	gameObject->SetLayer(GameObjectLayer::UNIT);
}

void BaseballCody::FixedUpdate(float fElapsedTime) noexcept
{
}

void BaseballCody::Update(float fElapsedTime) noexcept
{
	if (PADINPUT->GetButtonStay(PadInput::PadButton::BUTTON_X))
	{
		_animator->Play("Cody.ao|BaseBallCody", false);
		_animator->SetPlaySpeed(0.8);
		_lever->SetLeverDown(gameObject);
	}


	
}

void BaseballCody::LateUpdate(float fElapsedTime) noexcept
{
}
