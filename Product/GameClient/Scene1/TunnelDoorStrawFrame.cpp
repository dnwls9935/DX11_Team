#include "pch.h"
#include "TunnelDoorStrawFrame.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Model.h"
#include "Transform.h"


TunnelDoorStrawFrame::TunnelDoorStrawFrame(void) noexcept
{
}

void TunnelDoorStrawFrame::Start(void) noexcept
{
	_burn = false;
	_burnTime = 0.f;

	_renderer = gameObject->GetComponent<ModelRenderer>();
	_renderer->SetUseInstance(false);
	_renderer->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DISSOLVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\T_Fire_Tiled_D.tga"));
}

void TunnelDoorStrawFrame::Update(float fElapsedTime) noexcept
{
	if (INPUT->GetKeyDown(VK_NUMPAD0))
	{
		_burnTime = 0.f;
		_burn = true;
	}
	if (INPUT->GetKeyDown(VK_UP))
	{
		transform->Translate(transform->GetForward() * 0.5f);
	}

	if (false == _burn)
		return;

	if (nullptr == _renderer)
		return;

	_burnTime += fElapsedTime * 1.5f;

	if (_burnTime >= 1.f)
		_burnTime = 1.f;

	_renderer->GetMaterial().SetValue(ShaderSemantic::Type::FLOAT_DISSOLVE, &_burnTime, sizeof(float));
}

void TunnelDoorStrawFrame::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{

}

void TunnelDoorStrawFrame::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
}
