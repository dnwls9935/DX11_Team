#pragma once
#include "Behaviour.h"

class CrusherDrawBridge;
class EffectPlayer;
class Lock : public Behaviour
{
public:					Lock(CrusherDrawBridge* bridge) noexcept;
public:		virtual		~Lock(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		void		Unlock(void) noexcept;
public:		void		FollowBridge(void) noexcept;

private:	bool		_isUnlock;
private:	bool		_isEnd;

private:	float		_time;
private:	Transform* _lockTrans;

private:	bool		_isFollow;
private:	CrusherDrawBridge* _bridge;

private:	EffectPlayer*	_OpenEffectPlayer;
private:	AudioAsset*		_OpenAsset;
};

