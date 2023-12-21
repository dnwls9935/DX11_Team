#pragma once

#include "Behaviour.h"

class VacuumFan : public Behaviour
{
public:		VacuumFan(bool up) noexcept;
public:		virtual ~VacuumFan(void) noexcept;

public:		void Start(void) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;
	  
public:		void SetDirection(bool b) { _up = b; };
public:		bool GetDirection() {return _up; };

private:	bool _on;
private:	bool _up;

};

