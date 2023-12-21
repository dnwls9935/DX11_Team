#pragma once

#include "Behaviour.h"

namespace ce
{
	class Animator;
}

class VacuumBoss : public Behaviour
{
public:		VacuumBoss(void) noexcept;
public:		virtual ~VacuumBoss(void) noexcept;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;


private:	Animator*	_animator;
};

