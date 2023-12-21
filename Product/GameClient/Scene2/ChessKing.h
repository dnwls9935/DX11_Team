#pragma once
#include "MonsterActor.h"

namespace ce
{
	class SpriteRenderer; 
}
class ChessBoss;
class ChessBoardDataManager;
class DungeonCamera;

class ChessKing : public MonsterActor
{
public:		enum class Direction
{
	FORWARD,
	FORWARD_RIGHT,
	RIGHT,
	BACK_RIGHT,
	BACK,
	BACK_LEFT,
	LEFT,
	FORWARD_LEFT
};

public:		enum class eAudioType
{
	INTROVO,
	JUMPVO,
	SUMMONVO,
	DEADVO,
	MOVE,
	LAND,
	END
};

public:		ChessKing(ChessBoss* chessBoss, ChessBoardDataManager* boardManager) noexcept;
public:		virtual		~ChessKing(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		LateUpdate(float fElapsedTime) noexcept override;

public:		void		Idle(void) noexcept override;
public:		void		Walk(void) noexcept override;
public:		void		Jump(void) noexcept override;
public:		void		Attack(void) noexcept override;
public:		void		GetHit(float damage, int8  attacker) noexcept override;
public:		void		Die(void) noexcept override;

public:		void		LookAt(void) noexcept;
private:	void		SetNextPos(void) noexcept;

public:		virtual void	SetTarget(GameObject* target, bool isHit = true) noexcept;

public:		void		EndAttack(void) noexcept;
public:		void		EndJump(void) noexcept;
public:		void		DieEndAnimation(void) noexcept;

private:	int			_curIndex;
private:	int			_nextIndex;

private:	bool		_isFlying;

private:	bool		_same;
private:	Vector3		_curPosition;
private:	Vector3		_nextPosition;
private:	float		_time;

private:	bool		_isReverseAngle;
private:	float		_curRadian;
private:	float		_directionAngle;
private:	float		_rotationAngle;
private:	float		_rotTime;
private:	Direction	_curDir;

private:	Actor::State _animState;
private:	Actor::State _dirtyAnimState;

private:	bool			_visibleEffect;
private:	float			_effectTime;
private:	Transform*		_effectTrans;
private:	SpriteRenderer* _effect;

private:	ChessBoss*	_bossMgr;
private:	ChessBoardDataManager* _boardMgr;

private:	Transform*		_shockWaveTrans;
private:	SpriteRenderer* _shockWave;

private:	GameObject*		_dieObject;
private:	ModelRenderer*	_dieRenderer;
private:	Animator*		_dieAnimator;
private:	Transform*		_dieTransform;

private:	AudioAsset*		_audioAsset[static_cast<uint>(eAudioType::END)];

private:	DungeonCamera*	_dungeonCam;
};

