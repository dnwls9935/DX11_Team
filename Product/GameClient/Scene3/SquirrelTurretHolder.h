#pragma once
#include "MonsterActor.h"

class ExplosionFX;
class SpriteHP;
class EffectPlayer;

class SquirrelTurretHolder : public MonsterActor
{
public:		SquirrelTurretHolder(void) noexcept;
public:		virtual ~SquirrelTurretHolder(void) noexcept = default;

public:		void				Start(void) noexcept override;

public:		void				Update(float fElapsedTime) noexcept override;
public:		void				LateUpdate(float fElapsedTime) noexcept override;

public:		void				GetHit(float damage, int8 attacker = 0) noexcept override;
public:		void				Die(void) noexcept override;

public:		void				Idle(void) noexcept override { __noop; }
public:		void				Walk(void) noexcept override { __noop; }
public:		void				Attack(void) noexcept override { __noop; }
public:		static void			SetExplode(bool explode) noexcept { _isExplode = explode; }

private:	Transform*			_playerTransform = nullptr;

private:	bool				_visibleHP = false;

private:	bool				_playActiveAni;
private:	float				_aniDeltaTime;

private:	SpriteHP*			_spriteHpBar;

private:	ExplosionFX*		_fx;

private:	Vector3				_originPos;

private:	static	bool		_isExplode;

private:	EffectPlayer*		_effectPlayer;
private:	AudioAsset*			_audioAsset;
};


