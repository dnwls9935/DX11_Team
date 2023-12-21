#include "pch.h"
#include "FuseLight.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"

FuseLight::FuseLight(int sequence) noexcept
	: _sequence(sequence)
{

}

FuseLight::~FuseLight(void) noexcept
{

}

void FuseLight::Start(void) noexcept
{
	/* instancing true / false */
	/* Model renderer ���� ���� �ϳ����� ���� �ν��Ͻ����� 1���� �����ϴ°ɷ� �ٲ� 03-15*/
	ModelRenderer* renderer = ModelRenderer::Create(false);
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Utilty\\FuseBox_Light.fbx"));
	renderer->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1, 0, 0, 1));


	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
							* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(270)
																						, CE_MATH::ToRadian(90)
																						, CE_MATH::ToRadian(90)));
	renderer->SetOffsetMatrix(offsetmatrix);


	XMVECTOR position{};
	XMVECTOR rotaion = CE_MATH::Deg2Rad * XMVectorSet(0,-90.f,0,1);
	switch (_sequence)
	{
	case 0:
		position = XMVectorSet(-27.4f, -0.881f, 36.373, 1);
		break;
	case 1:
		position = XMVectorSet(-25.81f, -0.881f, 36.373, 1);
		break;
	case 2:
		position = XMVectorSet(-24.242f, -0.881f, 36.373, 1);
		break;
	}

	transform->SetWorldPosition(position);
	transform->Rotate(rotaion);

	_light = false;
}

void FuseLight::FixedUpdate(float fElapsedTime) noexcept
{
	/* �׽�Ʈ �ڵ� */
	if (INPUT->GetKeyDown('Z') && "FuseLight0" == gameObject->GetName())
	{
		_light = !_light;
	}
	if (INPUT->GetKeyDown('X') && "FuseLight1" == gameObject->GetName())
	{
		_light = !_light;
	}
	if (INPUT->GetKeyDown('C') && "FuseLight2" == gameObject->GetName())
	{
		_light = !_light;
	}
}

void FuseLight::Update(float fElapsedTime) noexcept
{
	if (true == _light)
	{
		gameObject->GetComponent<ModelRenderer>()->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.5f, 1, 0.5f, 1));
		gameObject->GetComponent<ModelRenderer>()->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(0, 1, 0, 1));
	}
	else
	{
		gameObject->GetComponent<ModelRenderer>()->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1, 0.5f, 0.5f, 1));
		gameObject->GetComponent<ModelRenderer>()->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1, 0, 0, 1));
	}
}

void FuseLight::LateUpdate(float fElapsedTime) noexcept
{
}
