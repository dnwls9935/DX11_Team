#include "pch.h"
#include "WackamoleDoor.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"

WackamoleDoor::WackamoleDoor(void) noexcept
{
}

void WackamoleDoor::Start(void) noexcept
{
	transform->SetLocalPosition(2.5f, 0, -0.7f);

	ModelRenderer* renderer = ModelRenderer::Create(false);
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Wackamole\\WakcaMoleDoor.fbx"));
	renderer->SetOffsetMatrix(XMMatrixScaling(0.01f, 0.01f, 0.01f));
	gameObject->AddComponent(renderer);
}

void WackamoleDoor::Update(float fElapsedTime) noexcept {}
