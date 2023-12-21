#pragma once
#include "Actor.h"

namespace ce
{
	class HierarcyNode;
	class SpriteRenderer;
}
class PlayerInput;
class PlayerStatus;
class TurretCamera;
class FlyingCamera;
class FlyingMachineTurretHolder;
class ParticleSystemAnimation;
class EffectPlayer;

class FlyingMachine : public Actor
{
public:		enum class Direction
			{
				UP,
				LEFT_UP,
				RIGHT_UP,
				DOWN,
				LEFT_DOWN,
				RIGHT_DOWN,
				LEFT,
				RIGHT,
				END
			};

public:		enum class AudioID
			{
				PROPELLER,
				BOOST,
				DAMAGE,
				END
			};

public:		FlyingMachine(bool bOperationPad) noexcept;
public:		virtual			~FlyingMachine(void) noexcept;

public:		virtual	void	Start(void) noexcept override;
public:		virtual void	FixedUpdate(float fElapsedTime) noexcept override;
public:		virtual void	Update(float fElapsedTime) noexcept override;
public:		virtual void	LateUpdate(float fElapsedTime) noexcept override;

public:		virtual void	GetHit(float damage, int8 attacker) noexcept override;
public:		virtual void	Die(void) noexcept override;
	
public:		virtual void	OnTriggerEnter(Collider* mine, Collider* other) noexcept override;

private:	void			CreateAfterBurnerFX(void) noexcept;

private:	void			OnEndBoost(void) noexcept;

private:	bool						_isDeath;
private:	float						_deathTime;

private:	ParticleSystemAnimation*	_particle[2];
private:	PlayerInput*				_playerInput;

private:	bool						_usingBoost;
private:	bool						_propellerOn;
private:	float						_boostTime;
private:	float						_boostEndTime;

private:	bool						_boostCool;
private:	float						_boostCoolTime;
private:	float						_boostCoolEndTime;

private:	FlyingMachineTurretHolder*	_turret;
private:	TurretCamera*				_turretCam;
private:	FlyingCamera*				_flyingCam;

private:	bool						_isDirtyDirection;
private:	Direction					_dirtyDirection;
private:	Direction					_direction;

private:	bool						_isLerp;
private:	float						_lerpTime;
private:	float						_lerpEndTime;

private:	CETween::EaseType			_easeType;
private:	Vector3						_moveDirection;
private:	Vector3						_curRadian;
private:	Vector3						_goalRadian;

private:	HierarcyNode*				_propeller;
private:	float						_propellerAngle;

private:	PlayerStatus*				_statusUI;

private:	EffectPlayer*				_propellerPlayer;
private:	EffectPlayer*				_boostPlayer;
private:	EffectPlayer*				_damagePlayer;
private:	AudioAsset*					_audioAsset[static_cast<uint>(AudioID::END)];

public:		virtual void	Idle(void) noexcept override		{ __noop; }
public:		virtual void	Walk(void) noexcept override		{ __noop; }
public:		virtual void	Dash(void) noexcept override		{ __noop; }
public:		virtual void	Run(void) noexcept override			{ __noop; }
public:		virtual void	Attack(void) noexcept override		{ __noop; }
public:		virtual void	Jump(void) noexcept override		{ __noop; }
public:		virtual void	DoubleJump(void) noexcept override	{ __noop; }
public:		virtual void	Falling(void) noexcept override		{ __noop; }
public:		virtual void	Interaction(void) noexcept			{ __noop; }

private:	static	constexpr	uint8 INPUT_FRONT		= 1 << 0;
private:	static	constexpr	uint8 INPUT_LEFT		= 1 << 1;
private:	static	constexpr	uint8 INPUT_RIGHT		= 1 << 2;
private:	static	constexpr	uint8 INPUT_BACKWARD	= 1 << 3;


};

