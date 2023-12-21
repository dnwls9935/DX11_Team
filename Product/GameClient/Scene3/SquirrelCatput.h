#pragma once

#include "Behaviour.h"

class SquirrelCatput : public Behaviour
{
public:		explicit SquirrelCatput(GameObject* target) noexcept;
public:		virtual ~SquirrelCatput(void) noexcept = default;


public:		void	Start(void) noexcept override;
public:		void	FixedUpdate(float fElapsedTime) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;

private:	GameObject*	_target;
private:	bool		_isFire;
private: 	bool		_isFired;
private:	bool		_isShoot = false;
private:	float		_deltaTime;
};

