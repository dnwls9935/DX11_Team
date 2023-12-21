#pragma once
#include "Behaviour.h"

class ChessKing;
class ChessQueen;
class DungeonActor;
class ChessBoardDataManager;
class ChessPawn;
class ChessBishop;
class ChessKnight;
class ChessRook;

//Chess King/Queen 동시 관리 클래스 
class ChessBoss : public Behaviour
{
public:	enum class SummonType
		{
			PAWN,
			BISHOP,
			KNIGHT,
			ROOK,
			END
		};

public:		ChessBoss(ChessBoardDataManager* chessBord) noexcept;
public:		virtual ~ChessBoss(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		FixedUpdate(float fElapsedTime) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		bool		GetHit(float damage, int8 attacker) noexcept;
public:		void		SetSummonPattern(void) noexcept;
public:		void		Summon(void) noexcept;

public:		float		GetMaxHP(void) noexcept { return _maxHp; }
public:		float		GetHP(void) noexcept { return _hp; }
public:		void		visible(bool bvisible) noexcept;

public:		void		DistanceDamage(float dist,float damage, bool isKing) noexcept;

private:	std::vector<DungeonActor*>	_players;
private:	ChessBoardDataManager*		_boardMgr;

private:	bool					_playerSet;

private:	bool					_isDead;
private:	float					_maxHp;
private:	float					_hp;

private:	ChessKing*				_king;
private:	ChessQueen*				_queen;

private:	SummonType				_prevSummon;
private:	ChessPawn*				_pawns[3];
private:	ChessBishop*			_bishops[2];
private:	ChessKnight*			_knights[7];
private:	ChessRook*				_rook;
};

