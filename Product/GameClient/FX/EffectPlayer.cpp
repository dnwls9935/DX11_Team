#include "pch.h"
#include "EffectPlayer.h"
#include "GameObject.h"
#include "AudioSource.h"
#include "AudioAsset.h"

EffectPlayer::EffectPlayer(AudioSource* audiosource) noexcept
{
	if (!audiosource)
	{
		CE_ASSERT("rldnr", "EffectPlayer는 audiosource가 필요합니다. ");
		return;
	}
	_AudioSource = audiosource;
}

void EffectPlayer::Start(void) noexcept
{
	gameObject->AddComponent(_AudioSource);

}

void EffectPlayer::Update(float fElapsedTime) noexcept
{

}

void EffectPlayer::Play(AudioAsset* asset, const float volume, const bool loop /* = false */, const std::function<void()> endEvent /* = nullptr */, const bool othersound /* = false */ , const bool worldsound /*= false*/) noexcept
{
	float optionVolume = 0.0f;

	if (othersound)
	{
		optionVolume = PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("MusicVolume", 5);
	}
	else
	{
		optionVolume = PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("EffectVolume", 5);
	}

	_AudioSource->LoadAudio(asset);
	_AudioSource->SetSoundWorld(worldsound);
	_AudioSource->SetLoop(loop);
	_AudioSource->SetVolume(optionVolume * volume);
	_AudioSource->Play(endEvent);
}

void EffectPlayer::Stop(void) noexcept
{
	_AudioSource->Stop();
}

void EffectPlayer::SetVolume(const float volume) noexcept
{
	const float effectVolume = PlayerPrefs::This().GetInt("TotalVolume", 10) * 0.1f * PlayerPrefs::This().GetInt("EffectVolume", 5);
	_AudioSource->SetVolume(effectVolume * volume);
}
