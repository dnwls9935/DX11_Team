#pragma once

#include "Behaviour.h"

namespace ce
{
	class Animator;
}
class VacuumHub : public Behaviour
{
public:		VacuumHub() noexcept;
public:		virtual ~VacuumHub(void) noexcept;

public:		void Start(void) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;


private:	Animator*	_animator;
private:	bool		_direction;
private:	float		_range;
	   
private:	Collider* collider;

};

