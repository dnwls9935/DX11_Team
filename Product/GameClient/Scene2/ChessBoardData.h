#pragma once
#include "Behaviour.h"


namespace ce
{
	class ParticleSystem;
}

class AtlasAnimation;

class ChessBoardData : public Behaviour
{
public:	explicit ChessBoardData(int index) noexcept;
public: virtual ~ChessBoardData(void) noexcept = default;


public:		void Start(void) noexcept override;
public:		void OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;

public:		const int GetIndex(void) const noexcept { return _index; };

public:		void Play(void) noexcept;
private:	void Stop(void) noexcept;
public:		bool isPlaying(void) noexcept;

private:	int					_index;
private:	AtlasAnimation*		_animator;
private:	ParticleSystem*		_particle;

private:	GameObject*			_smoke;
private:	GameObject*			_particleObject;
private:	float				_lifeTime;
private:	bool				_play;

private:	float				_lerpTime;
private:	float				_lerpAlpha;
private:	bool				_reverse;
};

