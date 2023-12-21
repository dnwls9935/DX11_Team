#pragma once
#include "Destruction.h"
class StoneWall : public Destruction
{
public:		StoneWall(void) noexcept;
public:		virtual	~StoneWall(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Die(void) noexcept override;

};

