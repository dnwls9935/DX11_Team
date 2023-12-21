#pragma once

#include "Summons.h"

class ChessRookFX;

class ChessRook : public Summons
{
private:	enum class ROOK_STATE { ANTICIPATION_IDLE, ATTACK, ROOK_STATE_END };

public:		ChessRook(void) noexcept;
public:		virtual ~ChessRook(void) noexcept;

public:		void	Start(void) noexcept override;
public:		void	FixedUpdate(float fElapsedTime) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	LateUpdate(float fElapsedTime) noexcept override;
public:		void	Die(void) noexcept override;

public:		void	Attack(float fElapsedTime) noexcept;

private:	void	OnEndAnimation(void) noexcept;

private:	ROOK_STATE		_rookState;
private:	ChessRookFX*	_fx;


private:	AudioAsset* _attackAsset;
private:	AudioAsset* _vanishAsset;
private:	AudioAsset* _spawnAsset;

};

