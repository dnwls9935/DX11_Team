#pragma once
#include "Behaviour.h"

static constexpr uint NUMCHESSBOARD = 64;

class ChessBoardDataManager : public Behaviour
{
public: enum class edirection
{
	FRONT,
	BACK,
	LEFT,
	RIGHT,
	END
};

public:	enum class cardinalPoint
{
	E, W, S, N,
	FOUR,
	EIGHT,
};

public: typedef struct tagchessboard
{
	GameObject*			_boarddata;
	bool				_chessboardIn;
} CHESSBOARD;

public:		explicit ChessBoardDataManager(void) noexcept ;
public:		virtual ~ChessBoardDataManager(void) noexcept = default;

public:		void			Start(void) noexcept override;
public:		void			Update(float fElapsedTime) noexcept override;

public:		FXMVECTOR		GetPosVec(const uint index)			const noexcept;
public:		class ChessBoardData*	GetBoardData(const uint index)		const noexcept;
public:		const std::vector<CHESSBOARD>& GetBoard() const noexcept { return _chessboard; };
public:		const bool		GetCheckIn(const uint index)		const noexcept;
public:		void			SetCheckIn(const uint index, bool set)	  noexcept;

public:		int				CalDirection(uint index, edirection edir) noexcept;
public:		bool			isOut(FXMVECTOR pos) noexcept;

public:		void			PlayFX(int currentIndex, cardinalPoint cardinalPoint);
public:		void			PlayFX(int currentIndex);

private:	void			EightPlayFX(int currentIndex);

private: std::vector<CHESSBOARD> _chessboard; 
private:XMFLOAT3 _rightupper;
private:XMFLOAT3 _rightunder;
private:XMFLOAT3 _leftupper ;
private:XMFLOAT3 _leftunder ;

};

