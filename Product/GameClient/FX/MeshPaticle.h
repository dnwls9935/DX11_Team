#pragma once

#include "Behaviour.h"

class MeshPaticle : public Behaviour
{
public:		MeshPaticle(void) noexcept;
public:		virtual ~MeshPaticle(void) noexcept;

public:		void	Start(void) noexcept;
public:		void	Update(float fElapsedTime) noexcept;
};

