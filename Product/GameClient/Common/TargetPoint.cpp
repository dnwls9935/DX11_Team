#include "pch.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Transform.h"

#include "TargetPoint.h"


TargetPoint::TargetPoint(FXMVECTOR position, targetType type) noexcept
	:	_targettype(type)
	,	_transform(nullptr)
{
	XMStoreFloat3(&_position, position);
}

TargetPoint::TargetPoint(Transform* transform, targetType type) noexcept
	:	_transform(transform)
	,	_targettype(type)
{

}

TargetPoint::~TargetPoint(void) noexcept
{
}

void TargetPoint::Start(void) noexcept
{
	SetTargetPoint(_targettype);
}

void TargetPoint::FixedUpdate(float fElapsedTime) noexcept
{
}

void TargetPoint::Update(float fElapsedTime) noexcept
{
}

void TargetPoint::LateUpdate(float fElapsedTime) noexcept
{
}

void TargetPoint::SetPointPosition(float x, float y, float z) noexcept
{
	_position.x = x;
	_position.y = y;
	_position.z = z;
}

void TargetPoint::SetPointPosition(FXMVECTOR vec) noexcept
{
	_position.x = XMVectorGetX(vec);
	_position.y = XMVectorGetY(vec);
	_position.z = XMVectorGetZ(vec);
}

void TargetPoint::SetTargetPoint(targetType targettype)
{
	


	switch (_targettype)
	{
	case TargetPoint::targetType::MAY:
	{
		GameObject* mayUI = GameObject::Instantiate("mayUI");
		mayUI->GetTransform()->SetLocalPosition(XMLoadFloat3(&_position));

		SpriteRenderer* InLine = SpriteRenderer::Create(true);
		InLine->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Player\\PlayerMarker.dds"));
		InLine->SetSize(0.75f, 0.75f);
		InLine->SetPassType(SpriteRenderer::RenderOption::UI);
		InLine->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.2f, 0.6f, 1.f, 1.f));
		mayUI->AddComponent(InLine);
		mayUI->SetLayer(GameObjectLayer::TRANSLUCENT);

		break;
	}
	case TargetPoint::targetType::CODY:
	{
		GameObject* codyUI = GameObject::Instantiate("codyUI");
		codyUI->GetTransform()->SetLocalPosition(XMLoadFloat3(&_position));

		SpriteRenderer* InLine = SpriteRenderer::Create(true);
		InLine->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Player\\PlayerMarker.dds"));
		InLine->SetSize(0.75f, 0.75f);
		InLine->SetPassType(SpriteRenderer::RenderOption::UI);
		InLine->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.2f, 1.f, 0.2f, 1.f));
		codyUI->AddComponent(InLine);
		codyUI->SetLayer(GameObjectLayer::TRANSLUCENT);

		break;
	}
	case TargetPoint::targetType::FUSECRAB:
	{
		GameObject* FuseCrabUI = GameObject::Instantiate("FuseCrabUI");
		FuseCrabUI->GetTransform()->SetLocalPosition(XMLoadFloat3(&_position));

		SpriteRenderer* InLine = SpriteRenderer::Create(true);
		InLine->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Stage1\\FuseCrab_Marker.dds"));
		InLine->SetSize(3.f, 3.f);
		InLine->SetPassType(SpriteRenderer::RenderOption::UI);
		InLine->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1.f, 1.f, 1.f, 1.f));
		FuseCrabUI->AddComponent(InLine);
		FuseCrabUI->SetLayer(GameObjectLayer::TRANSLUCENT);
		FuseCrabUI->SetActive(false);

		break;
	}
	case TargetPoint::targetType::MAYPOINT:
	{
		GameObject* mayTargetUI = GameObject::Instantiate("mayTargetUI");
		mayTargetUI->GetTransform()->SetLocalPosition(XMLoadFloat3(&_position));

		SpriteRenderer* InLine = SpriteRenderer::Create(true);
		InLine->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Stage1\\GrabPoint\\Dot.dds"));
		InLine->SetSize(0.5f, 0.5f);
		InLine->SetPassType(SpriteRenderer::RenderOption::UI);
		InLine->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1.f, 1.f, 1.f, 1.f));
		mayTargetUI->AddComponent(InLine);
		mayTargetUI->SetLayer(GameObjectLayer::TRANSLUCENT);


		GameObject* mayTargetOutLineUI = GameObject::Instantiate("mayTargetOutLineUI");
		mayTargetOutLineUI->GetTransform()->SetLocalPosition(XMLoadFloat3(&_position));

		SpriteRenderer* OutLine = SpriteRenderer::Create(true);
		OutLine->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Stage1\\GrabPoint\\Circle.dds"));
		OutLine->SetSize(0.75f, 0.75f);
		OutLine->SetPassType(SpriteRenderer::RenderOption::UI);
		OutLine->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.2f, 0.6f, 1.f, 1.f));
		mayTargetOutLineUI->AddComponent(OutLine);
		mayTargetOutLineUI->SetLayer(GameObjectLayer::TRANSLUCENT);

		
		break;
	}
	case TargetPoint::targetType::CODYPOINT:
	{
		GameObject* codyTargetUI = GameObject::Instantiate("codyTargetUI");
		codyTargetUI->GetTransform()->SetLocalPosition(XMLoadFloat3(&_position));

		SpriteRenderer* InLine = SpriteRenderer::Create(true);
		InLine->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Stage1\\GrabPoint\\Dot.dds"));
		InLine->SetSize(0.5f, 0.5f);
		InLine->SetPassType(SpriteRenderer::RenderOption::UI);
		InLine->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1.f, 1.f, 1.f, 1.f));
		codyTargetUI->AddComponent(InLine);
		codyTargetUI->SetLayer(GameObjectLayer::TRANSLUCENT);


		GameObject* codyTargetOutLineUI = GameObject::Instantiate("codyTargetOutLineUI");
		codyTargetOutLineUI->GetTransform()->SetLocalPosition(XMLoadFloat3(&_position));

		SpriteRenderer* OutLine = SpriteRenderer::Create(true);
		OutLine->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Stage1\\GrabPoint\\Circle.dds"));
		OutLine->SetSize(0.75f, 0.75f);
		OutLine->SetPassType(SpriteRenderer::RenderOption::UI);
		OutLine->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.2f, 1.f, 0.2f, 1.f));
		codyTargetOutLineUI->AddComponent(OutLine);
		codyTargetOutLineUI->SetLayer(GameObjectLayer::TRANSLUCENT);

		break;
	}
	case TargetPoint::targetType::END:
		CE_ASSERT("ckswns", "TargetPoint의 type이 지정되지않았습니다!");
		break;
	}
}
