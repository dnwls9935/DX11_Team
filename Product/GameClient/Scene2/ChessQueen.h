#pragma once
#include "MonsterActor.h"

class ChessBoss;
class ChessBoardDataManager;
class EffectPlayer;

class ChessQueen : public MonsterActor
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
	INTRO,
	ATTACK,
	CHARGE,
	SUMMON,
	END

};

public:		ChessQueen(ChessBoss* chessBoss, ChessBoardDataManager* boardManager) noexcept;
public:		virtual		~ChessQueen(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		LateUpdate(float fElapsedTime) noexcept override;

public:		void		Idle(void) noexcept override;
public:		void		Walk(void) noexcept override;
public:		void		Dash(void) noexcept override;
public:		void		Attack(void) noexcept override;
public:		void		GetHit(float damage, int8 attacker) noexcept override;
public:		void		Die(void) noexcept override;

public:		void		OnTriggerEnter(Collider * mine, Collider * other) noexcept override;

public:		void		SetPattern(void) noexcept;
public:		void		LookAt(void) noexcept;
private:	void		SetNextPos(void) noexcept;

public:		virtual void	SetTarget(GameObject* target, bool isHit = true) noexcept;

public:		void		Summon(void) noexcept;
public:		void		DashStart(void) noexcept;
public:		void		DashEndAnimation(void) noexcept;
public:		void		DashEnd(void) noexcept;
public:		void		DieEndAnimation(void) noexcept;

private:	int			_curIndex;
private:	int			_nextIndex;

private:	Vector3		_curPosition;
private:	Vector3		_nextPosition;
private:	float		_time;

private:	bool		_wait;
private:	float		_waitTime;

private:	bool		_isReverseAngle;
private:	float		_curRadian;
private:	float		_directionAngle;
private:	float		_rotationAngle;
private:	float		_rotTime;
private:	Direction	_curDir;

private:	bool		_isDash;
private:	bool		_dashAni;

private:	Collider*	_trigger;

private:	ChessBoss*	_bossMgr;
private:	ChessBoardDataManager* _boardMgr;

private:	GameObject*		_dieObject;
private:	ModelRenderer*	_dieRenderer;
private:	Animator*		_dieAnimator;
private:	Transform*		_dieTransform;

private:	EffectPlayer* _effectPlayer;
private:	AudioAsset* _audioAsset[static_cast<uint>(eAudioType::END)];
};

