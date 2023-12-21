#pragma once
#include "Behaviour.h"


class BaseballPlayer;
class EffectPlayer;

class BaseballLever : public Behaviour
{
private:	enum STATE{IDLE, DOWN, UP, END};

public: BaseballLever(bool ismay) noexcept;
public: virtual ~BaseballLever(void) = default;

public:	void Start(void) noexcept override;
public: void Update(float fElapsedTime) noexcept override;

public: void TurnLeverDown(float fElapsedTime) noexcept;
public: void TurnLeverUp(float fElapsedTime) noexcept;

public: void SetLeverDown(GameObject* obj) noexcept;


private: bool						_down;
private: bool						_up;
private: STATE						_state;

private: int						_reverse;

private: BaseballPlayer*			_player;
private: GameObject*				_playerobj;
private: EffectPlayer*				_effectPlayer;
};

