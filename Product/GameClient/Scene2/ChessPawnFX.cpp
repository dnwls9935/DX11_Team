#include "pch.h"
#include "ChessPawnFX.h"
#include "GameObject.h"
#include "Transform.h"
#include "SpriteRenderer.h"

ChessPawnFX::ChessPawnFX(Transform* pawn) noexcept
	: _parent(pawn)
{
}

void ChessPawnFX::Start(void) noexcept
{
	_color = Vector4(0.f, 0.2f, 1.f, 0.f);

	transform->SetParent(_parent);

	SetUpMarkBottomCircle();
	SetUpMarkBottomRotLarge();
	SetUpMarkBottomRotSmall();
	SetUpMarkPolar();

	_markBottomCircle->GetTransform()->SetParent(transform);
	_markBottomRotLarge->GetTransform()->SetParent(transform);
	_markBottomRotSmall0->GetTransform()->SetParent(transform);
	_markBottomRotSmall1->GetTransform()->SetParent(transform);


	_markBottomCircle->GetTransform()->SetLocalPosition(0, 0.1f, 0);
	_markBottomRotLarge->GetTransform()->SetLocalPosition(0, 0.11f, 0);
	_markBottomRotSmall0->GetTransform()->SetLocalPosition(0, 0.12f, 0);
	_markBottomRotSmall1->GetTransform()->SetLocalPosition(0, 0.13f, 0);

	_start = false;
	_lerpTime = 0.f;
	_lerpSize = 0.f;
}

void ChessPawnFX::Update(float fElapsedTime) noexcept
{
	if (false == _start)
		return;

	_lerpTime += fElapsedTime;

	float alpha = 0;
	alpha = CETween::Lerp(0, 1, std::min(1.f, _lerpTime), CETween::EaseType::easeLiner);
	_markBottomCircleRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_color.x, _color.y, _color.z, alpha));
	_markBottomCircleRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_color.x, _color.y, _color.z, alpha));

	_markBottomRotLargeRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_color.x, _color.y, _color.z, alpha));
	_markBottomRotLargeRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_color.x, _color.y, _color.z, alpha));

	_markBottomRotSmall0Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_color.x, _color.y, _color.z, alpha));
	_markBottomRotSmall0Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_color.x, _color.y, _color.z, alpha));

	_markBottomRotSmall1Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_color.x, _color.y, _color.z, alpha));
	_markBottomRotSmall1Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_color.x, _color.y, _color.z, alpha));

	for (int i = 0; i < 5; i++)
	{
		_markPolarRenderer[i]->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_color.x, _color.y, _color.z, alpha));
		_markPolarRenderer[i]->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_color.x, _color.y, _color.z, alpha));
	}

	_lerpSize += fElapsedTime * 0.05f;
	float size = 0;
	size = CETween::Lerp(12.7f, 0, std::min(2.f, _lerpSize), CETween::EaseType::easeLiner);
	_markBottomRotLargeRenderer->SetSize(size, size);
	size = CETween::Lerp(9, 0, std::min(2.f, _lerpSize), CETween::EaseType::easeLiner);
	_markBottomRotSmall0Renderer->SetSize(size, size);
	size = CETween::Lerp(7, 0, std::min(2.f, _lerpSize), CETween::EaseType::easeLiner);
	_markBottomRotSmall1Renderer->SetSize(size, size);


	size = CETween::Lerp(10, 0, std::min(2.f, _lerpSize), CETween::EaseType::easeLiner);
	_markPolarRenderer[0]->SetSize(size, size);
	_markPolarRenderer[3]->SetSize(size, size);
	size = CETween::Lerp(7, 0, std::min(2.f, _lerpSize), CETween::EaseType::easeLiner);
	_markPolarRenderer[1]->SetSize(size, size);
	_markPolarRenderer[2]->SetSize(size, size);
	size = CETween::Lerp(15, 0, std::min(2.f, _lerpSize), CETween::EaseType::easeLiner);
	_markPolarRenderer[4]->SetSize(size, size);


	float speed = 3.f;
 	_markBottomCircleTransform->Rotate(0, XMConvertToRadians(speed), 0);
	_markBottomRotLargeTransform->Rotate(0, XMConvertToRadians(speed), 0);
	_markBottomRotSmall0Transform->Rotate(0, XMConvertToRadians(speed * 3.f), 0);
	_markBottomRotSmall1Transform->Rotate(0, XMConvertToRadians(speed * -3.f), 0);

	for (int i = 0; i < 5; i++)
	{
		if(i == 3)
			_markPolarTransform[i]->Rotate(0, XMConvertToRadians(speed * -3.f), 0);
		else
			_markPolarTransform[i]->Rotate(0, XMConvertToRadians(speed * 3.f), 0);
	}
}

void ChessPawnFX::StartFX(void) noexcept
{
	gameObject->SetActive(true);
	_start = true;

 	_markBottomCircle->SetActive(true);
	_markBottomRotLarge->SetActive(true);
	_markBottomRotSmall0->SetActive(true);
	_markBottomRotSmall1->SetActive(true);

	for (int i = 0; i < 5; i++)
		_markPolar[i]->SetActive(true);

	_lerpTime = 0.f;

}

void ChessPawnFX::StopFX(void) noexcept
{
	_start = false;

	_lerpTime = 0.f;
	_lerpSize = 0.f;

	_markBottomCircle->SetActive(false);
	_markBottomRotLarge->SetActive(false);
	_markBottomRotSmall0->SetActive(false);
	_markBottomRotSmall1->SetActive(false);

	_markBottomCircleRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_color.x, _color.y, _color.z, 0));
	_markBottomCircleRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_color.x, _color.y, _color.z, 0));
	_markBottomRotLargeRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_color.x, _color.y, _color.z, 0));
	_markBottomRotLargeRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_color.x, _color.y, _color.z, 0));
	_markBottomRotSmall0Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_color.x, _color.y, _color.z, 0));
	_markBottomRotSmall0Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_color.x, _color.y, _color.z, 0));
	_markBottomRotSmall1Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_color.x, _color.y, _color.z, 0));
	_markBottomRotSmall1Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_color.x, _color.y, _color.z, 0));

	_markBottomRotLargeRenderer->SetSize(12.7f, 12.7f);
	_markBottomRotSmall0Renderer->SetSize(9, 9);
	_markBottomRotSmall1Renderer->SetSize(7, 7);

	for (int i = 0; i < 5; i++)
	{
		_markPolar[i]->SetActive(false);
		_markPolarRenderer[i]->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(_color.x, _color.y, _color.z, 0));
		_markPolarRenderer[i]->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_color.x, _color.y, _color.z, 0));
	}

	_markPolarRenderer[0]->SetSize(10, 10);
	_markPolarRenderer[1]->SetSize(7, 7);
	_markPolarRenderer[2]->SetSize(7, 7);
	_markPolarRenderer[3]->SetSize(10, 10);
	_markPolarRenderer[4]->SetSize(15, 15);

	gameObject->SetActive(false);
}

void ChessPawnFX::SetUpMarkBottomCircle(void) noexcept
{
	_markBottomCircle = GameObject::Instantiate("PawnFX_MarkBottomCircle");
	_markBottomCircle->GetTransform()->SetParent(transform);
	_markBottomCircle->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(90.f), 0, XMConvertToRadians(90.f));
	_markBottomCircleTransform = _markBottomCircle->GetTransform();
	_markBottomCircleRenderer = SpriteRenderer::Create(false);
	_markBottomCircleRenderer->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_1.dds"));
	_markBottomCircleRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, _color);
	_markBottomCircleRenderer->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_1.dds"));
	_markBottomCircleRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, _color);
	_markBottomCircleRenderer->SetSize(0.f, 0.f);
	_markBottomCircleRenderer->SetPassType(SpriteRenderer::RenderOption::EFFECTBLEND_MSAA);
	_markBottomCircle->AddComponent(_markBottomCircleRenderer);
	_markBottomCircle->SetLayer(GameObjectLayer::TRANSLUCENT);
	_markBottomCircle->SetTag(GameObjectTag::EFFECT);
}

void ChessPawnFX::SetUpMarkBottomRotLarge() noexcept
{
	_markBottomRotLarge = GameObject::Instantiate("PawnFX_MarkBottomRotLarge");
	_markBottomRotLarge->GetTransform()->SetParent(transform);
	_markBottomRotLarge->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(90.f), 0, XMConvertToRadians(90.f));
	_markBottomRotLargeTransform = _markBottomRotLarge->GetTransform();
	_markBottomRotLargeRenderer = SpriteRenderer::Create(false);
	_markBottomRotLargeRenderer->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_2.dds"));
	_markBottomRotLargeRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, _color);
	_markBottomRotLargeRenderer->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_2.dds"));
	_markBottomRotLargeRenderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, _color);
	_markBottomRotLargeRenderer->SetSize(0.f, 0.f);
	_markBottomRotLargeRenderer->SetPassType(SpriteRenderer::RenderOption::EFFECTBLEND_MSAA);
	_markBottomRotLarge->AddComponent(_markBottomRotLargeRenderer);
	_markBottomRotLarge->SetLayer(GameObjectLayer::TRANSLUCENT);
	_markBottomRotLarge->SetTag(GameObjectTag::EFFECT);
}

void ChessPawnFX::SetUpMarkBottomRotSmall() noexcept
{
	_markBottomRotSmall0 = GameObject::Instantiate("PawnFX_MarkBottomRotLarge");
	_markBottomRotSmall0->GetTransform()->SetParent(transform);
	_markBottomRotSmall0->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(90.f), 0, XMConvertToRadians(90.f));
	_markBottomRotSmall0Transform = _markBottomRotSmall0->GetTransform();
	_markBottomRotSmall0Renderer = SpriteRenderer::Create(false);
	_markBottomRotSmall0Renderer->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_2.dds"));
	_markBottomRotSmall0Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, _color);
	_markBottomRotSmall0Renderer->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_2.dds"));
	_markBottomRotSmall0Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, _color);
	_markBottomRotSmall0Renderer->SetSize(0.f, 0.f);
	_markBottomRotSmall0Renderer->SetPassType(SpriteRenderer::RenderOption::EFFECTBLEND_MSAA);
	_markBottomRotSmall0->AddComponent(_markBottomRotSmall0Renderer);
	_markBottomRotSmall0->SetLayer(GameObjectLayer::TRANSLUCENT);
	_markBottomRotSmall0->SetTag(GameObjectTag::EFFECT);

	_markBottomRotSmall1 = GameObject::Instantiate("PawnFX_MarkBottomRotLarge");
	_markBottomRotSmall1->GetTransform()->SetParent(transform);
	_markBottomRotSmall1->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(90.f), 0, XMConvertToRadians(90.f));
	_markBottomRotSmall1Transform = _markBottomRotSmall1->GetTransform();
	_markBottomRotSmall1Renderer = SpriteRenderer::Create(false);
	_markBottomRotSmall1Renderer->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_2.dds"));
	_markBottomRotSmall1Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, _color);
	_markBottomRotSmall1Renderer->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_2.dds"));
	_markBottomRotSmall1Renderer->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, _color);
	_markBottomRotSmall1Renderer->SetSize(0.f, 0.f);
	_markBottomRotSmall1Renderer->SetPassType(SpriteRenderer::RenderOption::EFFECTBLEND_MSAA);
	_markBottomRotSmall1->AddComponent(_markBottomRotSmall1Renderer);
	_markBottomRotSmall1->SetLayer(GameObjectLayer::TRANSLUCENT);
	_markBottomRotSmall1->SetTag(GameObjectTag::EFFECT);
}

void ChessPawnFX::SetUpMarkPolar(void) noexcept
{
	for (int i = 0; i < 5; i++)
	{
		std::string str = "PawnFX_MarkPolar";
		str += std::to_string(i);

		_markPolar[i] = GameObject::Instantiate(str.c_str());
		_markPolar[i]->GetTransform()->SetParent(transform);
		_markPolar[i]->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(90.f), 0, XMConvertToRadians(90.f));
		_markPolarTransform[i] = _markPolar[i]->GetTransform();
		_markPolarTransform[i]->SetLocalPosition(0, 0.4f * (i + 1), 0);
		_markPolarRenderer[i] = SpriteRenderer::Create(false);

		if (i == 0 || i == 2 || i == 4)
		{
			_markPolarRenderer[i]->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_1.dds"));
			_markPolarRenderer[i]->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_1.dds"));
		}
		else
		{
			_markPolarRenderer[i]->SetTexture(ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_2.dds"));
			_markPolarRenderer[i]->GetMaterial()->SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Mark_2.dds"));
		}
		_markPolarRenderer[i]->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, _color);
		_markPolarRenderer[i]->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(_color.x, _color.y, _color.z, 0));
		_markPolarRenderer[i]->SetSize(0.f, 0.f);
		_markPolarRenderer[i]->SetPassType(SpriteRenderer::RenderOption::EFFECTBLEND_MSAA);
		_markPolar[i]->AddComponent(_markPolarRenderer[i]);
		_markPolar[i]->SetLayer(GameObjectLayer::TRANSLUCENT);
		_markPolar[i]->SetTag(GameObjectTag::EFFECT);
	}


	_markPolarTransform[3]->SetLocalEulerAngle(0, XMConvertToRadians(180.f), 0);
}

