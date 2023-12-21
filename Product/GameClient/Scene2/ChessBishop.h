#pragma once

#include "Summons.h"

class ChessBishopFX;

namespace ce
{
	class ParticleSystem;
	class SpriteRenderer;
}
class AtlasAnimation;

class ChessBishop : public Summons
{
private:	enum class BISHOP_STAT {ANTICIPATION_IDLE, ATTACK, BISHOP_STATE_END};

public:		ChessBishop(void) noexcept;
public:		virtual ~ChessBishop(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	FixedUpdate(float fElapsedTime) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	LateUpdate(float fElapsedTime) noexcept override;
public:		void	Die(void) noexcept override;
public:		void	Attack(float fElapsedTime) noexcept;

private:	void	OnEndAnimation(void) noexcept;
private:	void	OnTriggerAnimation(void) noexcept;

private:	BISHOP_STAT				_bishopState;
private:	ChessBishopFX*			_fx;

private:	AudioAsset*				_attackAsset;
private:	AudioAsset*				_vanishAsset;
private:	AudioAsset*				_spawnAsset;


private:	SpriteRenderer*			_shockWave;
private:	float					_shockWaveTime;
private:	bool					_isShockWave;

private:	GameObject*				_shockAtalsObject;
private:	AtlasAnimation*			_shockAtlas;
};

