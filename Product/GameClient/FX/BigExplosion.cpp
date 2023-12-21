#include "pch.h"
#include "BigExplosion.h"
#include "GameObject.h"
#include "Transform.h"
#include "AtlasAnimationX.h"

BigExplosion::BigExplosion(void) noexcept
{
}

void BigExplosion::Start(void) noexcept
{
	for (int i = 0; i < 25; i++)
	{
		_explosionObject[i] = GameObject::Instantiate("BigExplosion_Explosion");
		_explosionTransform[i] = _explosionObject[i]->GetTransform();
		_explosionTransform[i]->SetParent(transform);
		_explosionTransform[i]->SetLocalPosition(0, 0, 0);

		float x = (CE_MATH::Random(20) - 10);
		float y = (CE_MATH::Random(20) - 10);
		float z = (CE_MATH::Random(20) - 10);
		_lerpPos[i] = XMFLOAT3(x, y, z);

		AtlasAnimationX::AtlasDesc desc;
		desc._fullPath = "Asset\\Texture\\Explosion\\Explosion.dds";
		desc._color = Vector4(0.74f, 0.45f, 0.22f, 1.f) * 6.f;
		desc._emissivePath = "Asset\\Texture\\Explosion\\ExplosionEmission.dds";
		desc._emissiveColor = Vector4(0.74f, 0.27f, 0.027f, 1.f) * 2.5f;
		desc._amount = XMFLOAT2(6, 6);
		desc._billboard = true;
		desc._fadeOut = true;
		desc._lerpSize = XMFLOAT2(15, 15);
		desc._lerpType = CETween::EaseType::easeInExpo;
		desc._renderPass = SpriteRenderer::RenderOption::ATLASANIM;
		desc._size = XMFLOAT2(15, 15);
		desc._speed = 30.f;
		desc._fadeOutSpeed = 0.95f;
		_explosionAnimation[i] = new AtlasAnimationX(desc, false);
		_explosionObject[i]->AddComponent(_explosionAnimation[i]);
		_explosionAnimation[i]->ResetUV();

		_explosionObject[i]->SetLayer(GameObjectLayer::TRANSLUCENT);
		_explosionObject[i]->SetActive(false);
	}
}

void BigExplosion::Update(float fElapsedTime) noexcept
{
	int b = 0;
	for (int i = 0; i < 25; i++)
	{
		_lerpTimeAcc[i] += fElapsedTime;

		float x = CETween::Lerp(0, _lerpPos[i].x, std::min(1.f, _lerpTimeAcc[i]), CETween::EaseType::easeOutQuint);
		float y = CETween::Lerp(0, _lerpPos[i].y, std::min(1.f, _lerpTimeAcc[i]), CETween::EaseType::easeOutQuint);
		float z = CETween::Lerp(0, _lerpPos[i].z, std::min(1.f, _lerpTimeAcc[i]), CETween::EaseType::easeOutQuint);
		_explosionTransform[i]->SetLocalPosition(x, y, z);

		if (true == _explosionAnimation[i]->GetAnimationEnd())
		{
			b++;
			_explosionAnimation[i]->SetEnable(false);
		}

		if (b == 25)
		{
			_start = false;
			GameObject::Destroy(gameObject);
		}
	}
}

void BigExplosion::OnDestroy(void) noexcept
{
	Transform::TList& childs = const_cast<Transform::TList&>(transform->GetChilds());

	for (int i = 0; i < childs.size(); ++i)
		GameObject::Destroy(childs[i]->GetGameObject());
}

void BigExplosion::StartFX(void) noexcept
{
	for (int i = 0; i < 25; i++)
	{
		_lerpTimeAcc[i] = 0;
		_explosionTransform[i]->SetLocalPosition(0, 0, 0);
		_explosionAnimation[i]->SetEnable(true);
		_explosionAnimation[i]->ResetUV();

		float x = (CE_MATH::Random(20) - 10);
		float y = (CE_MATH::Random(20) - 10);
		float z = (CE_MATH::Random(20) - 10);
		_lerpPos[i] = XMFLOAT3(x, y, z);

		int randomAngle = CE_MATH::Random(360);
		_explosionAnimation[i]->SetBillboadEularAngle(Vector3(0, 0, XMConvertToRadians(randomAngle)));
	}
}
