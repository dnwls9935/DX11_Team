#pragma once

#include "Behaviour.h"

namespace ce
{
	class ModelRenderer;
}

class Spike : public Behaviour
{
public: typedef struct tagspike
{
	GameObject* _spikeeach;
	ModelRenderer* _renderer;
} SPIKEEACH;
public: explicit Spike(void) noexcept  ;
public: virtual ~Spike(void) noexcept = default;


public:	void Start(void) noexcept override;
public:	void Update(float fElapsedTime) noexcept override;
public: void SetDirection(FXMVECTOR dir, FXMVECTOR right) noexcept;

public:	void OnEnable() noexcept override;
public:	void OnDisable() noexcept override;

private: SPIKEEACH	_each[3];
private: float _lerpTime;
private: bool _dir;

};

