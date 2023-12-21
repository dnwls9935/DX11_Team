#include "pch.h"
#include "Spark.h"
#include "GameObject.h"
#include "Transform.h"
#include "AtlasAnimationX.h"

Spark::Spark(Transform* parent) noexcept
	: _parent(parent)
{
}

Spark::~Spark(void) noexcept
{
}

void Spark::Start(void) noexcept
{
	
}

void Spark::Update(float fElapsedTime) noexcept
{
	
}

void Spark::StartFX(void) noexcept
{
	_start = true;
}
