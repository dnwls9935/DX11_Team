#pragma once

#include "Behaviour.h"

class AtlasAnimationX;

class Spark : public Behaviour
{
public:		Spark(Transform* parent) noexcept;
public:		virtual ~Spark(void) noexcept;

public:		void	Start(void) noexcept;
public:		void	Update(float fElapsedTime) noexcept;
public:		void	StartFX(void) noexcept;

private:	bool	_start;

private:	Transform*			_parent;

};

