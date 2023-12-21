#include "pch.h"
#include "MeshPaticle.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"

MeshPaticle::MeshPaticle(void) noexcept
{
}

MeshPaticle::~MeshPaticle(void) noexcept
{
}

void MeshPaticle::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create(true);
	//renderer->SetModel(ASSETMANA);
	
	
}

void MeshPaticle::Update(float fElapsedTime) noexcept
{
}
