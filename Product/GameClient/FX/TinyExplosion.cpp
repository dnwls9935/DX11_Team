#include "pch.h"
#include "TinyExplosion.h"
#include "GameObject.h"
#include "Transform.h"
#include "AtlasAnimationX.h"

TinyExplosion::TinyExplosion(void) noexcept
{
}

void TinyExplosion::Start(void) noexcept
{
	for (int i = 0; i < 6; i++)
	{
		_tinyExplosionObject[i] = GameObject::Instantiate("TinyExplosion");
		_tinyExplosionTransform[i] = _tinyExplosionObject[i]->GetTransform();
		_tinyExplosionTransform[i]->SetParent(transform);

		AtlasAnimationX::AtlasDesc desc;
		desc._fullPath = "Asset\\Texture\\Explosion\\Explosion.dds";
		desc._color = Vector4(0.74f, 0.45f, 0.22f, 1.f) * 6.f;
		desc._emissivePath = "Asset\\Texture\\Explosion\\ExplosionEmission.dds";
		desc._emissiveColor = Vector4(0.74f, 0.27f, 0.027f, 1.f) * 2.5f;
		desc._amount = XMFLOAT2(6, 6);
		desc._billboard = true;
		desc._fadeOut = true;
		desc._lerpSize = XMFLOAT2(5, 5);
		desc._lerpType = CETween::EaseType::easeInExpo;
		desc._renderPass = SpriteRenderer::RenderOption::ATLASANIM;
		desc._size = XMFLOAT2(5, 5);
		desc._speed = 70.f;
		desc._fadeOutSpeed = 1.7f;
		_tinyExplosionAnimation[i] = new AtlasAnimationX(desc, false);
		_tinyExplosionObject[i]->AddComponent(_tinyExplosionAnimation[i]);
		_tinyExplosionAnimation[i]->ResetUV();

		_tinyExplosionObject[i]->SetLayer(GameObjectLayer::TRANSLUCENT);
		_tinyExplosionObject[i]->SetActive(false);
	}

	_lerpTimeAcc = 0.f;
	_lerpType = 0;
}

void TinyExplosion::Update(float fElapsedTime) noexcept
{
	if (false == _start)
		return;

	_lerpTimeAcc += fElapsedTime * 1.5f;

	int b = 0;
	for (int i = 0; i < 6; i++)
	{
		float size = 0;
		if (0 == _lerpType)
		{
			size = CETween::Lerp(0, 1.0f, std::min(0.5f, _lerpTimeAcc), CETween::EaseType::easeOutBack);

			if (_lerpTimeAcc >= 0.5f)
			{
				_lerpTimeAcc = 0;
				_lerpType++;
			}
		}
		else
			size = CETween::Lerp(1.0f, 4.0f, std::min(0.5f, _lerpTimeAcc), CETween::EaseType::easeInExpo);

		_tinyExplosionTransform[i]->SetLocalScale(size, size, size);

		if (true == _tinyExplosionAnimation[i]->GetAnimationEnd())
		{
			b++;
			_tinyExplosionAnimation[i]->SetEnable(false);
		}

		if (b == 6)
		{
			_start = false;
			GameObject::Destroy(gameObject);
		}
	}
}

void TinyExplosion::OnDestroy(void) noexcept
{
	Transform::TList& childs = const_cast<Transform::TList&>(transform->GetChilds());

	for (int i = 0; i < childs.size(); ++i)
		GameObject::Destroy(childs[i]->GetGameObject());
}

void TinyExplosion::StartFX(void) noexcept
{
	_start = true;

	_lerpTimeAcc = 0.f;
	_lerpType = 0;

	for (int i = 0; i < 6; i++)
	{
		_tinyExplosionAnimation[i]->SetSpeed(30.f);

		_tinyExplosionAnimation[i]->SetEnable(true);
		_tinyExplosionAnimation[i]->ResetUV();

		float x = (CE_MATH::Random(3) - 1.5f);
		float y = (CE_MATH::Random(3) - 1.5f);
		float z = (CE_MATH::Random(3) - 1.5f);
		_tinyExplosionTransform[i]->SetLocalPosition(x, y, z);

		int randomAngle = CE_MATH::Random(360);
		_tinyExplosionAnimation[i]->SetBillboadEularAngle(Vector3(0, 0, XMConvertToRadians(randomAngle)));
	}
}
