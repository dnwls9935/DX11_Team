#pragma once
#include "Behaviour.h"

class Crosshair;

class FlyingMachineTurretHolder : public Behaviour
{
public:		FlyingMachineTurretHolder(const bool bOperationPad) noexcept;
public:		virtual ~FlyingMachineTurretHolder(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		void		Die(void) noexcept;

private:	bool				_bOperationPad;

private:	bool				_move;
private:	float				_moveAngle;
private:	float				_startAngle;

private:	Crosshair*			_crosshair;

private:	Transform*			_cameraTrans;
private:	Transform*			_parentTrans;
};

