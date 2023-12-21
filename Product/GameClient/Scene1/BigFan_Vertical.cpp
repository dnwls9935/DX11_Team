#include "pch.h"
#include "BigFan_Vertical.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "GameObject.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

BigFan_Vertical::BigFan_Vertical(void) noexcept
	: _rotate(false)
	, _sound(false)
	, _rotatesound(false)
{

}

BigFan_Vertical::~BigFan_Vertical(void) noexcept
{

}

void BigFan_Vertical::Start(void) noexcept
{

	_loop = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\WallSlideFan\\Play_World_Shed_Vacuum_Platform_WallSlideFan_Valve.ogg");

	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);
}

void BigFan_Vertical::Update(float fElapsedTime) noexcept
{

	if (_rotate)
	{
		transform->Rotate(0, 0, fElapsedTime * CE_MATH::ToRadian(10));

		if (!_sound)
		{
			_effectPlayer->Play(_loop, 0.5f);
			_sound = true;
		}
	}
	else
	{
		if (_sound)
		{
			_effectPlayer->Stop();
			_sound = false;
		}
	}
}
