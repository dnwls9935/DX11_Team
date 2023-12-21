#include "pch.h"
#include "CycloneFx.h"

#include "GameObject.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "ShaderManager.h"
#include "SpriteRenderer.h"
#include "AtlasAnimation.h"

CycloneFx::CycloneFx(void) noexcept
{
}

CycloneFx::~CycloneFx(void) noexcept
{
}

void CycloneFx::Start(void) noexcept
{

	for (int i = 0; i < 3; i++)
	{
		_flame[i] = GameObject::Instantiate("CycloneFlame");
		_flame[i]->GetTransform()->SetParent(transform);
		_flame[i]->GetTransform()->SetLocalPosition(0, 0, 0);
		
		_rendererFlame[i] = ModelRenderer::Create(false);
		_rendererFlame[i]->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Effect\\Ring\\Ring_2.fbx"));

		_flame[i]->AddComponent(_rendererFlame[i]);

		Material::TList& mats = _rendererFlame[i]->GetMaterials();

		for (int i = 0; i < mats.size(); ++i)
		{
			mats[i].SetShader(ShaderManager::This().GetShader(Shader::Type::MODEL_FORWARD_EFFECT));
			/*mats[i].SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.74f, 0.04f, 0.f, 1));
			mats[i].SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1, 0.7f, 0.f, 1));*/

			mats[i].SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1, 0.04f, 0.f, 1));
			mats[i].SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1, 0.7f, 0.f, 1));
		}

		_rendererFlame[i]->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\PerlinMap_4.dds"));
		_rendererFlame[i]->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\PerlinMap_4.dds"));
		//_rendererFlame->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_NOISE, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\PerlinMap_4.dds"));
		_rendererFlame[i]->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_ALPHA, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\mask_3.dds"));
		_rendererFlame[i]->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_NORMAL, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\Normal_4.dds"));


		_flame[i]->SetTag(GameObjectTag::EFFECT);
		_flame[i]->SetLayer(GameObjectLayer::TRANSLUCENT);

	}
	_flame[0]->GetTransform()->SetLocalScale(0.15f, 1.5f, 0.15f);
	_flame[1]->GetTransform()->SetLocalScale(0.25f, 1.f, 0.25f);
	_flame[2]->GetTransform()->SetLocalScale(0.3f, 0.5f, 0.3f);

	_frameTime = 0.f;
	_scrollSpeeds = XMFLOAT3(1.1f, 2.1f, 3.1f);
	_scales = XMFLOAT3(1, 2, 3);

	_distortion[0] = XMFLOAT2();

	_distortion[0] = DirectX::XMFLOAT2(0.1f, 0.4f);
	_distortion[1] = DirectX::XMFLOAT2(0.1f, 0.3f);
	_distortion[2] = DirectX::XMFLOAT2(0.1f, 0.2f);

	_distortionScale = 0.5f;
	_distortionBias = 0.8f;


	std::string str[4];
	str[0] = "Asset\\Texture\\Dungeon\\FlameRing\\Slash_4.dds";
	str[1] = "Asset\\Texture\\Dungeon\\FlameRing\\Slash_3.dds";
	str[2] = "Asset\\Texture\\Dungeon\\FlameRing\\Slash_4.dds";
	str[3] = "Asset\\Texture\\Dungeon\\FlameRing\\Slash_3.dds";

	for (int i = 0; i < 4; i++)
	{
		_ring[i] = GameObject::Instantiate("RingFlame");
		_ring[i]->GetTransform()->SetParent(transform);
		_ring[i]->GetTransform()->SetLocalPosition(0, 0, 0);


		AtlasAnimation::AtlasDesc desc;
		desc._fullPath = str[i];
		desc._emissivePath = str[i];
		desc._emissiveColor = XMFLOAT4(1, 0, 0.f, 1);
		desc._amount = XMFLOAT2(10, 10);
		desc._color = Vector4(1.f, 0.4f, 0.f, 1);
		desc._size = XMFLOAT2(2 + (i * i), 2 + (i * i));
		desc._offsetVector = XMVectorSet(CE_MATH::ToRadian(90), 0, 0, 0);
		desc._speed = 40.f;
		desc._duration = 3.0f;
		desc._renderPass = SpriteRenderer::RenderOption::ATLASANIM_ONE;
		desc._billboard = false;

		_rendererRing[i] = new AtlasAnimation(desc, true);
		_ring[i]->AddComponent(_rendererRing[i]);
		_ring[i]->GetTransform()->SetLocalPosition(0, 0.2f + (i * 0.1f), 0);
		_ring[i]->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(90.f), 0, XMConvertToRadians(90.f));
		_ring[i]->GetTransform()->SetLocalEulerAngle(XMConvertToRadians(90.f), XMConvertToRadians(i * 90.f), XMConvertToRadians(90.f));
	}

	for (int i = 0; i < 3; i++)
	{
		_flame[i]->SetActive(false);
	}
	for (int i = 0; i < 4; i++)
	{
		_ring[i]->SetActive(false);
	}

	_use = false;
}

void CycloneFx::Update(float fElapsedTime) noexcept
{
	_frameTime += fElapsedTime;
	_uv.x += fElapsedTime * 3.5f;
	_uv.y -= fElapsedTime * 2.f;

	for (int i = 0; i < 3; i++)
	{
		Material::TList& mats = _rendererFlame[i]->GetMaterials();
		for (int j = 0; j < mats.size(); ++j)
		{
			mats[j].SetValue(ShaderSemantic::Type::FLOAT2_TEXCOORD, &_uv, sizeof(float2));


			mats[j].GetShader().SetRawValue("frameTime", &_frameTime, sizeof(float));

			mats[j].GetShader().SetRawValue("scrollSpeeds", &_scrollSpeeds, sizeof(float) * 3);
			mats[j].GetShader().SetRawValue("scales", &_scales, sizeof(float) * 3);

			mats[j].GetShader().SetRawValue("distortion1", &_distortion[0], sizeof(float) * 2);
			mats[j].GetShader().SetRawValue("distortion2", &_distortion[1], sizeof(float) * 2);
			mats[j].GetShader().SetRawValue("distortion3", &_distortion[2], sizeof(float) * 2);

			mats[j].GetShader().SetRawValue("distortionScale", &_distortionScale, sizeof(float));
			mats[j].GetShader().SetRawValue("distortionBias", &_distortionBias, sizeof(float));

		}
	}

	if (_frameTime <= 3.f)
		_frameTime = 0.f;




	for (int i = 0; i < 4; i++)
	{
		_ring[i]->GetTransform()->Rotate(0, fElapsedTime * -60.f , 0);
	}


}

void CycloneFx::StartFx(void) noexcept
{
	for (int i = 0; i < 3; i++)
	{
		_flame[i]->SetActive(true);
	}
	for (int i = 0; i < 4; i++)
	{
		//_ring[i]->SetActive(true);
	}
	
	_use = true;
	gameObject->SetActive(_use);
}

void CycloneFx::StopFx(void) noexcept
{
	for (int i = 0; i < 3; i++)
	{
		_flame[i]->SetActive(false);
	}
	for (int i = 0; i < 4; i++)
	{
		//_ring[i]->SetActive(false);
	}

	_use = false;
	gameObject->SetActive(_use);
}
