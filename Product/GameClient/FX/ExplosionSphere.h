#pragma once

#include "Behaviour.h"

class ExplosionSphere : public Behaviour
{
public:		ExplosionSphere(void) noexcept;
public:		virtual ~ExplosionSphere(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;

public:		void	StartFX(void) noexcept;


private:	GameObject*		_lightningSphere;

private:	bool	_start;
};

