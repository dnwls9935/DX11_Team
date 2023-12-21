#include "pch.h"
#include "WackamoleBox.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"

WackamoleBox::WackamoleBox(void) noexcept
{
}

void WackamoleBox::Start(void) noexcept
{
	_renderer = ModelRenderer::Create(false);
	gameObject->AddComponent(_renderer);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Wackamole\\WakcaMoleBox.fbx"));

	XMMATRIX OffsetMat = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	_renderer->SetOffsetMatrix(OffsetMat);
}

void WackamoleBox::Update(float fElapsedTime) noexcept
{
}
