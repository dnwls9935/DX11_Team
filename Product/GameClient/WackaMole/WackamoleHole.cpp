#include "pch.h"
#include "WackamoleHole.h"
#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"

WackamoleHole::WackamoleHole(POSITION index) noexcept
	: _index(index)
{
}

void WackamoleHole::Start(void) noexcept
{
	switch (_index)
	{
	case WackamoleHole::POSITION::UP:
		transform->SetLocalPosition(-2.f, 2.32f, 0);
		break;
	case WackamoleHole::POSITION::DOWN:
		transform->SetLocalPosition(1.f, 2.32f, 0);
		break;
	case WackamoleHole::POSITION::LEFT:
		transform->SetLocalPosition(-0.6f, 2.32f, -1.4f);
		break;
	case WackamoleHole::POSITION::RIGHT:
		transform->SetLocalPosition(-0.6f, 2.32f, 1.4f);
		break;
	default:
		break;
	}


	ModelRenderer* renderer = ModelRenderer::Create(false);
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Wackamole\\WakcaMoleHole.fbx"));
	renderer->SetOffsetMatrix(XMMatrixScaling(0.01f, 0.01f, 0.01f));
	gameObject->AddComponent(renderer);
}

void WackamoleHole::Update(float fElapsedTime) noexcept
{
}
