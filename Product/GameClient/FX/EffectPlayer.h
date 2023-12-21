#pragma once

#include "Behaviour.h"

class EffectPlayer : public Behaviour
{
public: EffectPlayer(AudioSource* audiosource) noexcept ;
public: virtual ~EffectPlayer(void) noexcept = default;

public: void			Start(void)					noexcept override;
public:	void			Update(float fElapsedTime)	noexcept override;

public: void			Play(AudioAsset* asset, 
							const float volume, 
							const bool loop = false, 
							const std::function<void()> endEvent = nullptr,
							const bool othersound = false,
							const bool worldsound = false) noexcept;
public: void			Stop(void) noexcept;
public:	void			SetVolume(const float volume) noexcept;
public: AudioSource*	GetAudioSource(void) const noexcept { return _AudioSource; }

public: AudioSource* _AudioSource;
};

