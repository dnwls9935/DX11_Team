#pragma once
#include "Behaviour.h"

namespace ce
{
	class ParticleSystem;
}

class Spike;
class ChessBoardDataManager;

static constexpr uint numspike = 200;

class ChessRookFX : public Behaviour
{
public:	typedef struct tagrookfx
{
	GameObject* _moveObj;
	Collider* _ColliderObj;
} ROOKFX;

public:	explicit ChessRookFX(void) noexcept;
public:	virtual ~ChessRookFX(void) noexcept = default;

public:	void	Start(void) noexcept override;
public:	void	FixedUpdate(float fElapsedTime) noexcept override;
public:	void	Update(float fElapsedTime) noexcept override;
public:	void	LateUpdate(float fElapsedTime) noexcept override;
public:	void	SetBoardManager(ChessBoardDataManager* board) noexcept { _boardmanager = board; }
public:	void	DieReset(void) noexcept;

public:	void	StartFX(void) noexcept;
public:	void	Explosion(void) noexcept;

private:	ROOKFX				_rookFX[4];
private:	Spike*				_spike[numspike];
private:	ChessBoardDataManager* _boardmanager;

private:	float				_lerpTime;
private:	float				_oldpos;
private:	uint				_count;
private:	bool				_start;
private:	bool				_explosion;
private:	bool				_colliderWidth;


};

