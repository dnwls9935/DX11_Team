#pragma once
#include "Behaviour.h"
#include "AcornBullet.h"

class AcornShooter : public Behaviour
{
public:		AcornShooter(GameObjectTag tag) noexcept;
public:		virtual ~AcornShooter(void) noexcept = default;

public:		void			Awake(void) noexcept override;
public:		void			Start(void) noexcept override;
public:		void			FixedUpdate(float fElapsedTime) noexcept override;
public:		void			Update(float fElapsedTime) noexcept override;
public:		void			LateUpdate(float fElapsedTime) noexcept override;
		
public:		void			OnEnable(void) noexcept override;
public:		void			OnDisable(void) noexcept override;

public:		void			Play( const float damage
								, const float speed
								, const bool loop = false
								, const float reFireTime = 10) noexcept;

public:		void			Play( const float damage
								, const float speed
								, const FXMVECTOR dir) noexcept;

public:		void			Stop(void) noexcept;
private:	void			Fire(FXMVECTOR direction) noexcept;

private:	bool			_isPlaying;

private:	float			_shootingTime;
private:	float			_deltaTime;

private:	float			_bulletSpeed;
private:	float			_bulletDamage;

private:	GameObjectTag	_enemyTag;
};

