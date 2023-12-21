#pragma once

#include "Behaviour.h"

namespace ce
{
	class Rigidbody;
	class Collider;
}

class BigFan : public Behaviour
{
public:		BigFan(bool direction) noexcept;
public:		virtual ~BigFan(void) noexcept;

public:		void Start(void) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		void SetRotate(bool b) { _rotate = b; }
public:		void SetDirection(bool b) { _direction = b; }
	
private:	bool _direction; /* true == left */
private:	bool _rotate;

private:	Rigidbody* _rigid;
private:	Collider* _colliderBlade0;
private:	Collider* _colliderBlade1;
private:	Collider* _colliderBlade2;

};

