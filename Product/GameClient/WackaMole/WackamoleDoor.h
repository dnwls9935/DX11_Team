#pragma once

#include "Behaviour.h"

class WackamoleDoor : public Behaviour
{
public:		WackamoleDoor(void) noexcept;
public:		virtual ~WackamoleDoor(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
};