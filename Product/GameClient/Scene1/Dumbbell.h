#pragma once

#include "Behaviour.h"

namespace ce
{
	class Rigidbody;
	class Collider;
}

class Dumbbell : public Behaviour
{
public:		enum class PLAYER {MAY, CODY, END};

public:		Dumbbell(void) noexcept;
public:		virtual ~Dumbbell(void) noexcept;

public:		void Start(void) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

private:	GameObject* _dumbbellMayGrab;
private:	GameObject* _dumbbellCodyGrab;

private:	Rigidbody*	_rigid;
private:	Collider*	_collider;

private:	GameObject* _may;
private:	GameObject* _cody;

};

