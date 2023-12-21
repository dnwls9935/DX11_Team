#pragma once

#include "Behaviour.h"

class TinyExplosion;

class BulletFlare : public Behaviour
{
public:		BulletFlare(void) noexcept;
public:		virtual ~BulletFlare(void) noexcept = default;
public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;

public:		void	StartFX(void) noexcept;

private:	TinyExplosion*		_tinyExplosion;
private:	bool				_start;
};

