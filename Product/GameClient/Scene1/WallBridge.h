#pragma once
#include "Behaviour.h"
class EffectPlayer;
class WallBridge :  public Behaviour
{
public:		WallBridge(bool isShort) noexcept;
public:		virtual ~WallBridge(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		void		UpBridge(void) noexcept;
public:		void		DownBridge(void) noexcept;

private:	bool		_isShort = false;

private:	bool		_isUp;
private:	bool		_isDown;
private:	bool		_isPushButton;

private:	bool		_isEnd;

private:	float		_time;

private:	float4		_pos;

private:	EffectPlayer* _effectPlayer;
private:	AudioAsset* _upAsset;
private:	AudioAsset* _downAsset;
};

