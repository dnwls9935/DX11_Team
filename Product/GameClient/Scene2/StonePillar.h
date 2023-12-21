#pragma once
#include "Destruction.h"

class StonePillar : public Destruction
{
public:		StonePillar(void) noexcept;
public:		virtual	~StonePillar(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Die(void) noexcept override;
};

