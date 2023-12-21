#pragma once
#include "Behaviour.h"

namespace ce
{
	class SpriteRenderer;
	class ModelRenderer;
	class ParticleSystem;
	class Rigidbody;
}
class EffectPlayer;

class Stone : public Behaviour
{
public:		Stone(int turn) noexcept;
public:		virtual		~Stone(void) noexcept = default;
	 
public:		void		Start(void) noexcept override;
public:  	void		Update(float fElapsedTime) noexcept override;

public:		void		OnTriggerEnter(Collider* mine, Collider* other) noexcept override;

private:	void		Collision(void) noexcept;
private:	void		BrokenStone(void) noexcept;

private:	bool			_collision;
private:	bool			_explosion;
private:	float			_time;

private:	Vector3			_startPos;

private:	bool			_fall;
private:	float			_fallTime;

private:	int				_stoneTurn;
private:	std::string		_breakPath;

private:	ModelRenderer*	_renderer;
private:	SpriteRenderer*	_shadow;
//private:	ParticleSystem* _particle;

private:	EffectPlayer* _effectPlayer;
private:	AudioAsset* _audioAsset;
};

