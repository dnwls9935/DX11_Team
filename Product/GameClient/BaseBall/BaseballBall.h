#pragma once
#include "Behaviour.h"

class BaseballPlayerScore;
class EffectPlayer;

class BaseballBall : public Behaviour
{
public: BaseballBall(void) noexcept;
public: virtual ~BaseballBall(void) = default;

public:	void Start(void) noexcept override;
public: void Update(float fElapsedTime) noexcept override;
public: void OnTriggerEnter(Collider* mine, Collider* other) noexcept override;

private: BaseballPlayerScore*		_playerScore;
private: EffectPlayer*				_effectPlayer;
};

