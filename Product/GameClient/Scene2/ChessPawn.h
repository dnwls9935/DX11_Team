#pragma once

#include "Summons.h"

namespace ce
{
	class ParticleSystem;
	class SpriteRenderer;
}

class ChessPawnFX;

class ChessPawn : public Summons
{
private:	enum class PAWN_STATE { ANTICIPATION_IDLE, ATTACK, PAWN_STATE_END };

public:		ChessPawn(void) noexcept;
public:		virtual ~ChessPawn(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	FixedUpdate(float fElapsedTime) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	LateUpdate(float fElapsedTime) noexcept override;
public:		void	Attack(float fElapsedTime) noexcept;
public:		void	Die(void) noexcept override;
public:		void	SearchTile(void) noexcept;
public:		void	OnTriggerEnter(Collider* mine, Collider* other) noexcept override;

private:	void	OnEndAnimation(void) noexcept;
private:	void	OnTriggerAnimation(void) noexcept;

private:	PAWN_STATE			_pawnState;

private:	ChessPawnFX*		_fx;

private:	Collider*			_triggerCollider;


private:	AudioAsset*			_attackAsset;
private:	AudioAsset*			_vanishAsset;
private:	AudioAsset*			_spawnAsset;

private:	SpriteRenderer*		_shockWave;
private:	float				_shockWaveTime;
private:	bool				_isShockWave;

private:	SpriteRenderer*		_shockWavePilar;
private:	float				_shockwavePilarTime;
private:	bool				_isShockWavePilar;
};