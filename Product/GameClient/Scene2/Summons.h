#pragma once

#include "MonsterActor.h"

namespace ce
{
	class ParticleSystem;
}

class Summons : public MonsterActor
{
public:	Summons(bool isPreInstance) noexcept;
public:	virtual ~Summons(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	FixedUpdate(float fElapsedTime) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	LateUpdate(float fElapsedTime) noexcept override;

public:		void	Idle(void) noexcept override { __noop; };
public:		void	Walk(void) noexcept override { __noop; };
public:		void	Attack(void) noexcept override { __noop; };
public:		void	Die(void) noexcept override;

public:		void	OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void	OnTriggerExit(Collider* mine, Collider* other) noexcept override;
public:		void	SetBoard(class ChessBoardDataManager* board) noexcept { _board = board; }
public:		void	Spawn(int index);

public:		void	OnDead(void) noexcept;
protected:	void	virtual OnEndAnimation(void) noexcept PURE;

protected:	bool			_isSpawn;
protected:	bool			_isSpawnSound;
protected:	int				_index;
protected:	float			_lerpTime;
protected:	float			_lerpSize;

protected:	ModelRenderer*	_replaceRenderer;
protected:	Animator*		_replaceAnimator;

protected:	class ChessBoardDataManager* _board;
protected:	ParticleSystem* _particle;
};


