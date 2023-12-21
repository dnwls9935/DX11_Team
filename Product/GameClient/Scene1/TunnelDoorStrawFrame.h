#pragma once

#include "Behaviour.h"

namespace ce
{
	class ModelRenderer;
}

class TunnelDoorStrawFrame : public Behaviour
{
public:		TunnelDoorStrawFrame(void) noexcept;
public:		virtual ~TunnelDoorStrawFrame(void) noexcept = default;

public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;

public:		void OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void OnTriggerExit(Collider* mine, Collider* other) noexcept override;

private:	ModelRenderer*		_renderer;
private:	bool				_burn;
private:	float				_burnTime;
};

