#pragma once

#include "MonsterActor.h"

namespace ce
{
	class Rigidbody;
	class Collider;
	class ModelRenderer;
	class Animator;
}

class SpinningTrap : public MonsterActor
{
public:		SpinningTrap(void) noexcept;
public:		virtual ~SpinningTrap(void) noexcept;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
//public:		void Die(void) noexcept override;


 // Don't use
public:		void Idle(void) noexcept override;
public:		void Walk(void) noexcept override;
public:		void Attack(void) noexcept override { __noop; };
public:		void Die(void) noexcept override;
protected:	void OnEndAnimation(void) noexcept { __noop; };

public:		void		 OnTriggerEnter(Collider* mine, Collider* other) noexcept;
public:		void		 OnTriggerExit(Collider* mine, Collider* other) noexcept;

public:		void SetRotate(bool b) { _rotate = b; }
public:		void SetStartEnd(FXMVECTOR start, FXMVECTOR end) noexcept;


private:	Rigidbody*	_rigid;
private:	Collider*	_triggerCollider;
private:	Collider*	_collider;

private:	ModelRenderer*	_renderer;
private:	Animator*		_animator;;

private:	bool	_rotate;
private:	float	_speed;
private:	float	_damage;

private:	XMFLOAT3 _start;
private:	XMFLOAT3 _end;
private:	XMFLOAT3 _to;
private:	float	 _length;
private:	bool	_smashed;
private:	bool	_die;
};

