#include "pch.h"
#include "FountainParticle.h"

FountainParticle::FountainParticle(void) noexcept
	: ParticleSystem(Type::MESH, Shape::CONE)
{
}

FountainParticle::~FountainParticle(void) noexcept
{
}

void FountainParticle::UpdateXXX(float fElapsedTime) noexcept
{
	_lifeTime += fElapsedTime;

	float velocity = _particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * _particleVelocityVariation;

	for (int i = 0; i < 30; i++)
	{
		_particleList[i].velocity = velocity;
		_particleList[i].deltaTime += fElapsedTime;
		_particleList[i].position = _particleList[i].position + ((_particleList[i].direction + _offsetVector) * _particleList[i].velocity * fElapsedTime);
	}

	UpdateBuffers();
}

void FountainParticle::SetGravity(bool use, float value)
{
	if (true == use)
		_gravity = value;
	else
		_gravity = 0.f;
}

void FountainParticle::SetDirection(FXMVECTOR dir)
{
	for (int i = 0; i < 30; i++)
	{
		XMVECTOR direction = dir * (((float)rand() - (float)rand()) / RAND_MAX);
		direction = XMVector3Normalize(direction);

		direction = XMVectorSetY(direction, 1.f);

		_particleList[i].direction = direction;
	}
}
