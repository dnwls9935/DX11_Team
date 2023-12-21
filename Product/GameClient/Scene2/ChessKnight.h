#pragma once

#include "Summons.h"

namespace ce
{
	class ParticleSystem;
}

class ChessKnight : public Summons
{
public:		ChessKnight(void) noexcept;
public:		virtual ~ChessKnight(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	FixedUpdate(float fElapsedTime) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	LateUpdate(float fElapsedTime) noexcept override;
public:		void	Attack(void) noexcept override;
public:		void	Die(void) noexcept override;
public:		void	Spawn(int index) noexcept;
public:		void	OnTriggerEnter(Collider* mine, Collider* other) noexcept override;

private:	void	OnEndAnimation(void) noexcept;

private:	bool	_go;
private:	int		_finalPointIndex;

private:	ChessBoardDataManager* _mgr;

private:	AudioAsset* _attackAsset;
private:	AudioAsset* _vanishAsset;
private:	AudioAsset* _spawnAsset;

};