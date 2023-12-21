#pragma once

#include "Behaviour.h"

class BigExplosion;
class AdditionalSmoke;
class ExplosionShockWave;
class Debris;

class ExplosionFX : public Behaviour
{
public:		ExplosionFX(void) noexcept;
public:		virtual ~ExplosionFX(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	StartFX(XMVECTOR position) noexcept;

private:	BigExplosion*		    _bigExplosion;
private:	AdditionalSmoke*	    _additionalSmoke;
private:    ExplosionShockWave*     _explosionShockWave;
private:    Debris*                 _debris;
};

              