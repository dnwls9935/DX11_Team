#pragma once

#include "Cinematic.h"

class WareHouseCinematic : public Cinematic
{

public:	WareHouseCinematic(const std::vector<CINEMADESC>& cinemadesc ,GameObject* CameraObj[2]) noexcept;
public: virtual ~WareHouseCinematic(void) noexcept = default;

public: void Start(void) noexcept override;
public: void Update(float fElapsedTime) noexcept override;
public:	void Play(void) noexcept;
public: void SetOn(uint index) noexcept { _On[index] = true; };
private: virtual void OnEndCinema(void) noexcept override;

private: GameObject* _cameraobj[2];
private: bool		_On[2];
};

