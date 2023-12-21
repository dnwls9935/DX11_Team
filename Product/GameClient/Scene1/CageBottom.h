#pragma once

#include "Behaviour.h"

namespace ce
{
	class Rigidbody;
	class Collider;
}

class CageBottom : public Behaviour
{
public:		CageBottom(GameObject* flyingMachineObject, XMMATRIX modelOffsetMatrix) noexcept;
public:		virtual ~CageBottom(void) noexcept = default;

public:		virtual void	Start(void) noexcept override;
public:		virtual void	Update(float fElapsedTime) noexcept override;

public:		virtual void	OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		virtual void	OnTriggerExit(Collider* mine, Collider* other) noexcept override;

private:	GameObject*		_flyingMachineObject;
private:	Rigidbody*		_rigid;
private:	Collider*		_trigger;
private:	Collider*		_collider;

private:	XMMATRIX		_offsetMat;
};

