#include "pch.h"
#include "AdditionalSmoke.h"
#include "GameObject.h"
#include "Transform.h"
#include "AtlasAnimationX.h"

AdditionalSmoke::AdditionalSmoke(void) noexcept
{
}

void AdditionalSmoke::Start(void) noexcept
{
	for (int i = 0; i < 10; i++)
	{
		_smokeObject[i] = GameObject::Instantiate("BigExplosion_AdditionalSmoke");
		_smokeTransform[i] = _smokeObject[i]->GetTransform();
		_smokeTransform[i]->SetParent(transform);
		_smokeTransform[i]->SetLocalPosition(0, 0, 0);

		float x = (CE_MATH::Random(30) - 15);
		float y = (CE_MATH::Random(30) - 15);
		float z = (CE_MATH::Random(30) - 15);
		_lerpPos[i] = XMFLOAT3(x, y, z);

		AtlasAnimationX::AtlasDesc desc;
		desc._fullPath = "Asset\\Texture\\Explosion\\SmokePuff01.dds";
		desc._color = Vector4(0.2f, 0.2f, 0.2f, 0.1f);
		desc._amount = XMFLOAT2(5, 5);
		desc._billboard = true;
		desc._fadeOut = true;
		desc._fadeOutSpeed = 0.5f;
		desc._lerpType = CETween::EaseType::easeOutQuint;
		desc._renderPass = SpriteRenderer::RenderOption::ATLASANIM;
		desc._size = XMFLOAT2(20, 20);
		desc._lerpSize = XMFLOAT2(20, 20);
		desc._speed = 25.f;
		_smokeAnimation[i] = new AtlasAnimationX(desc, false);
		_smokeObject[i]->AddComponent(_smokeAnimation[i]);
		_smokeAnimation[i]->ResetUV();

		_smokeObject[i]->SetLayer(GameObjectLayer::TRANSLUCENT);
		_smokeObject[i]->SetActive(false);

	}
}

void AdditionalSmoke::Update(float fElapsedTime) noexcept
{
	for (int i = 0; i < 10; i++)
	{
		_lerpTimeAcc[i] += fElapsedTime;

		float x = CETween::Lerp(0, _lerpPos[i].x, std::min(1.f, _lerpTimeAcc[i]), CETween::EaseType::easeOutQuint);
		float y = CETween::Lerp(0, _lerpPos[i].y, std::min(1.f, _lerpTimeAcc[i]), CETween::EaseType::easeOutQuint);
		float z = CETween::Lerp(0, _lerpPos[i].z, std::min(1.f, _lerpTimeAcc[i]), CETween::EaseType::easeOutQuint);
		_smokeTransform[i]->SetLocalPosition(x, y, z);

		_smokeAnimation[i]->SetSpeed(10.f);
		_smokeAnimation[i]->SetColor(Vector4(0.2f, 0.2f, 0.2f, 0.1f));
		_smokeAnimation[i]->SetFadeSpeed(0.0f);
		_smokeAnimation[i]->SetSize(XMFLOAT2(10,10));

		if (true == _smokeAnimation[i]->GetAnimationEnd())
		{
			_smokeAnimation[i]->SetEnable(false);
		}
	}
}

void AdditionalSmoke::StartFX(void) noexcept
{
	for (int i = 0; i < 10; i++)
	{
		_lerpTimeAcc[i] = 0;
		_smokeTransform[i]->SetLocalPosition(0, 0, 0);
		_smokeAnimation[i]->SetEnable(true);
		_smokeAnimation[i]->ResetUV();

		float x = (CE_MATH::Random(20) - 10);
		float y = (CE_MATH::Random(20) - 10);
		float z = (CE_MATH::Random(20) - 10);
		_lerpPos[i] = XMFLOAT3(x, y, z);

		int randomAngle = CE_MATH::Random(360);
		_smokeAnimation[i]->SetBillboadEularAngle(Vector3(0, 0, XMConvertToRadians(randomAngle)));
	}
}
