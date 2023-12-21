#pragma once

#include "Behaviour.h"

namespace ce
{
	class Rigidbody;
	class Collider;
}

class PumpValve : public Behaviour
{
public:		PumpValve(void) noexcept;
public:		virtual ~PumpValve(void) noexcept;

public:		void Start(void) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		void SetDirection(bool b);
public:		void SetRotate(bool b);
public:		void SetController(GameObject* obj1, GameObject* obj2);

public:		void OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void OnTriggerExit(Collider* mine, Collider* other) noexcept override;

public:		void SetBtn(GameObject* btn);

private:	bool	_direction;
private:	bool	_rotate;

private:	GameObject* _bigFan[2];
private:	GameObject* _grabPosition;

private:	GameObject* _btn;

private:	Rigidbody*	_rigid;
private:	Collider*	_collider;


};