#pragma once

#include "Behaviour.h"

class TunnelDoor : public Behaviour
{
public:	explicit TunnelDoor(GameObject* _target, int8 cardinalNESW = 0) noexcept;
public: virtual ~TunnelDoor(void) = default;


public:	void Start(void) noexcept override;
public: void Update(float fElapsedTime) noexcept override;
public: void FixedUpdate(float fElapsedTime) noexcept override;
public: void OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public: static void SetCloseNarration(bool _On) noexcept { _closeNarration = _On; }

private: GameObject* _target;
private: float		_closeTime;
private: bool		_isClosed;
private: bool		_isClose;
private: int8		_cardinalDir;

private: static bool	_closeNarration;
};

