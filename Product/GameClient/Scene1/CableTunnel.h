#pragma once

#include "Behaviour.h"

namespace ce
{
	class Rigidbody;
	class Collider;
}

class CableTunnel : public Behaviour
{
public:		CableTunnel(void) noexcept;
public:		virtual ~CableTunnel(void) noexcept;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;

private:	Rigidbody* _rigid;
private:	Collider* _collider;
};

