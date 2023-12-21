#include "pch.h"
#include "WareHouseCinematicPlayer.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Animator.h"

WareHouseCinematicPlayer::WareHouseCinematicPlayer(CHARACTORDESC charactordesc) noexcept
	: CinematicPlayer(charactordesc)
{
}

void WareHouseCinematicPlayer::Start(void) noexcept
{
	_modelrenderer = ModelRenderer::Create(false);
	_modelrenderer->SetModel(ASSETMANAGER->GetModelData(_charactordesc.modelname.c_str()));
	gameObject->AddComponent(_modelrenderer);

	XMMATRIX mat = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(0.f
		, CE_MATH::ToRadian(180.f)
		, CE_MATH::ToRadian(0.f)));

	_modelrenderer->SetOffsetMatrix(  mat * XMLoadFloat4x4(&_charactordesc.offsetmatrix));

	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play(_charactordesc.animationindex,false);
	_animator->SetPlaySpeed(_charactordesc.animationspeed);

	gameObject->SetTag(GameObjectTag::PLAYER);
	gameObject->SetLayer(GameObjectLayer::UNIT);
}

void WareHouseCinematicPlayer::Update(float fElapsedTime) noexcept
{
}

void WareHouseCinematicPlayer::SetColorDepth(const Vector4& vec) noexcept
{
	Material::TList& mats = _modelrenderer->GetMaterials();
	for (auto& mat : mats)
	{
		mat.SetColor(ShaderSemantic::Type::COLOR_DEPTHEFFECT, Vector4(vec));
	}
}
