#pragma once

#include "Behaviour.h"

class WackamoleHole : public Behaviour
{
public:		enum class POSITION { UP, DOWN, LEFT, RIGHT };

public:		WackamoleHole(POSITION index) noexcept;
public:		virtual ~WackamoleHole(void) noexcept = default;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;

private:	POSITION		_index;
};

