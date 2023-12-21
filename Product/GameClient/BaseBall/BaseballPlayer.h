#pragma once
#include "Behaviour.h"


class BaseballPlayer : public Behaviour
{
public: BaseballPlayer(bool ismay) noexcept;
public: virtual ~BaseballPlayer(void) = default;

public:	void Start(void) noexcept override;
public: void Update(float fElapsedTime) noexcept override;

private: bool			_ismay;
};

