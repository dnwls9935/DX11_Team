#pragma once

#include "CinematicPlayer.h"



class WareHouseCinematicPlayer : public CinematicPlayer
{
public: WareHouseCinematicPlayer(CHARACTORDESC charactordesc) noexcept;
public: virtual ~WareHouseCinematicPlayer(void) noexcept = default;

public:	void Start(void) noexcept override;
public: void Update(float fElapsedTime) noexcept override;
public: void SetColorDepth(const Vector4& vec) noexcept;
};

