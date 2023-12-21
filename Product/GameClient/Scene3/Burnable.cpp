#include "pch.h"
#include "Burnable.h"
#include "GameObject.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

bool Burnable::_burnsound = false;

Burnable::Burnable(void) noexcept
{
}

Burnable::~Burnable(void) noexcept
{
}

void Burnable::Start(void) noexcept
{
	_burn = false;
	_burnTime = 0.f;

	_renderer = gameObject->GetComponent<ModelRenderer>();
	_renderer->SetUseInstance(false);

	_rigid = gameObject->GetComponent<Rigidbody>();
	_collider = gameObject->GetComponent<Collider>();

	gameObject->SetTag(GameObjectTag::BURNABLE);

	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);

	_audioAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage3\\Play_World_Tree_Escape_Destructible_BurnableStraw_BurnOnShot.ogg");
}

void Burnable::Update(float fElapsedTime) noexcept
{
	if (false == _burn)
		return;

	if (nullptr == _renderer)
		return;

	_burnTime += fElapsedTime;

	float burn = CETween::Lerp(0, 1, std::min(1.f, _burnTime), CETween::EaseType::easeInExpo);

	if (_burnTime >= 1.f)
	{
		_burnTime = 1.f;
		_burn = false;
		//GameObject::Destroy(gameObject);
	}

	_renderer->GetMaterial().SetValue(ShaderSemantic::Type::FLOAT_DISSOLVE, &_burnTime, sizeof(float));

}

void Burnable::Burn(void) noexcept
{
	if (_burn == true)
		return;

	if (!_burnsound)
	{
		_burnsound = true;
		_effectPlayer->Play(_audioAsset, 0.1f, false, [&]() {
			_burnsound = false;
			}
		);
	}
	_burn = true;
	_renderer->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DISSOLVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\T_Fire_Tiled_D.dds"));

	_rigid->SetEnable(false);
	_collider->SetEnable(false);
}

void Burnable::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
}

void Burnable::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
}
