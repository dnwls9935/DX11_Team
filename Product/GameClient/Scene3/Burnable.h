#pragma once

#include "Behaviour.h"

namespace ce
{
	class ModelRenderer;
	class Rigidbody;
	class Collider;
}

class EffectPlayer;

class Burnable : public Behaviour
{
public:		Burnable(void) noexcept;
public:		virtual ~Burnable(void) noexcept;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;

public:		void Burn(void) noexcept;

public:		void OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void OnTriggerExit(Collider* mine, Collider* other) noexcept override;

private:	ModelRenderer*	_renderer;
private:	bool			_burn;
private:	float			_burnTime;

private:	Rigidbody*		_rigid;
private:	Collider*		_collider;
private:	EffectPlayer*	_effectPlayer;
private:	AudioAsset*		_audioAsset;
private:	static bool		_burnsound;
};

