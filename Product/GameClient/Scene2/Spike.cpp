#include "pch.h"
#include "Spike.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "Actor.h"

Spike::Spike(void) noexcept
	: _dir(false)
	, _lerpTime(0.f)
{

}

void Spike::Start(void) noexcept
{
	for (int i = 0; i < 3; ++i)
	{
		_each[i]._spikeeach = GameObject::Instantiate();
		_each[i]._renderer = ModelRenderer::Create(true);
		_each[i]._renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Effect\\Spike\\Spike.fbx"));
		_each[i]._spikeeach->AddComponent(_each[i]._renderer);


		_each[i]._spikeeach->SetLayer(GameObjectLayer::OBJECT);
		_each[i]._spikeeach->GetTransform()->SetLocalScale(0, 0, 0);

	}

}

void Spike::Update(float fElapsedTime) noexcept
{
	if (!_dir)
	{
		_lerpTime += fElapsedTime * 2;
		if (_lerpTime >= 1.f)
		{
			_dir = !_dir;
			_lerpTime = 0.f;
			return;
		}
		float xzlerp = CETween::Lerp(0.f, 0.15f, std::min(1.f, _lerpTime), CETween::EaseType::easeOutQuart);
		float ylerp = CETween::Lerp(0.f, 0.075f, std::min(1.f, _lerpTime), CETween::EaseType::easeOutQuart);
		for (int i = 0; i < 3; ++i)
		{
			_each[i]._spikeeach->GetTransform()->SetLocalScale(xzlerp, ylerp, xzlerp);
		}

	}
	else
	{
		_lerpTime += fElapsedTime * 1.5f;
		if (_lerpTime >= 1.f)
		{
			_dir = !_dir;
			_lerpTime = 0.f;
			gameObject->SetActive(false);
			return;
		}
		float xzlerp = CETween::Lerp(0.15f, 0.f, std::min(1.f, _lerpTime), CETween::EaseType::easeOutQuart);
		float ylerp = CETween::Lerp(0.075f, 0.f, std::min(1.f, _lerpTime), CETween::EaseType::easeOutQuart);
		for (int i = 0; i < 3; ++i)
		{
			_each[i]._spikeeach->GetTransform()->SetLocalScale(xzlerp, ylerp, xzlerp);
		}

	}
}

void Spike::SetDirection(FXMVECTOR pos, FXMVECTOR right) noexcept
{
	_each[0]._spikeeach->GetTransform()->SetLocalPosition(pos);
	_each[1]._spikeeach->GetTransform()->SetLocalPosition(pos + right);
	_each[2]._spikeeach->GetTransform()->SetLocalPosition(pos - right);
}

void Spike::OnEnable() noexcept
{
	for (int i = 0; i < 3; ++i)
	{
		_each[i]._spikeeach->SetActive(true);
	}
	
	_dir = false;
	_lerpTime = 0.f;
}

void Spike::OnDisable() noexcept
{
	for (int i = 0; i < 3; ++i)
	{
		_each[i]._spikeeach->GetTransform()->SetLocalScale(0, 0, 0);
		_each[i]._spikeeach->SetActive(false);
	}
}

