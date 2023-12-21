#pragma once

#include "Behaviour.h"

class FuseLight : public Behaviour
{
public:		FuseLight(int sequence) noexcept;
public:		virtual ~FuseLight(void) noexcept;


public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		bool	GetLight() { return _light; };
public:		void	SetLight(bool onlight) { _light = onlight; }

private:	bool	_light;
private:	int		_sequence;

};

