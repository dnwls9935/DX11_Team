#pragma once
#include "Behaviour.h"
#include "InputHandler.h"
#include "FollowingCamera.h"

namespace ce
{
	class Animator;
	class ModelRenderer;
	class Texture;
	class Rigidbody;
	class HierarcyNode;
}
class EffectPlayer;

class Actor abstract : public Behaviour
{
public:		enum class Type
			{
				PLAYER,
				MONSTER,
				END
			};

public:		enum class State
			{
				IDLE,
				WALKSTART, JUMPLANDWALKSTART, WALK, WALKEND,
				RUN,
				ATTACK,
				DASH, DASHEND, DASHENDJOG,
				JUMP, JUMPFWD, JUMPLAND, JUMPLANDJOG,
				SECOND_JUMP,
				FALLING,
				GETHIT,
				INTERACTIONSTART, INTERACTION, INTERACTIONIDLE, INTERACTIONEND,
				PICKUP, PICKENTER,
				CROUCHENTER, CROUCHSTART,  CROUCHIDLE, CROUCH, CROUCHEND, CROUCHEXIT, CROUCHEXITMOVE, 
				GROUNDPOUNDSTART, GROUNDPOUNDFALLING, GROUNDPOUNDLAND, GROUNDPOUNDEXIT, 
				WALLSLIDERENTER, WALLSLIDEIDLE, WALLSLIDEJUMP,
				SAWENTER, SAWIDLE, SAWSUCCSESS,
				SLIDEENTER, SLIDEIDLE, SLIDEEXIT, 
				SUCTION,
				VALVE_ROTATE_IDLE, VALVE_ROTATE,
				PUSHENTER, PUSHIDLE, PUSH, PUSHEND,
				LEVERLEFT, LEVERRIGHT,
				DIE,
				END
			};

public:		enum class eAudioType {
				WALK,
				DASH,
				JUMP,
				DOUBLEJUMP,
				GROUNDPOUNDSTART,
				GROUNDPOUNDFALL,
				GROUNDPOUNDLAND,
				SLIDESTART,
				SLIDELOOP,
				SLIDESTOP,
				LAND,
				CROUCH,
				VO_DASH,
				VO_AIRDASH,
				VO_DOUBLEJUMP,
				VO_GROUNDPOUNTSTART,
				VO_GROUNDPOUNDJUMP,
				VO_GROUNDPOUNDLAND,
				VO_JUMP,
				VO_WALLGRAB,
				VO_WALLJUMP,
				END
			};

public:		Actor(float maxHp =12) noexcept;
public:		virtual ~Actor(void) noexcept = default;

public:		Type				GetType(void) const noexcept { return _actorType; }
public:		State				GetState(void) const noexcept { return _state; }

public:		bool				IsDead(void) noexcept { return _isDead; }
public:		bool				GetOperationPad(void) noexcept { return _bOperationPad; }

public:		virtual void		Idle(void) noexcept PURE;
public:		virtual void		Walk(void) noexcept PURE;
public:		virtual void		Run(void) noexcept PURE;
public:		virtual void		Attack(void) noexcept PURE;
public:		virtual void		GetHit(float damage, int8 attacker = 0) noexcept PURE;
public:		virtual void		Die(void) noexcept PURE;

public:		virtual void		Dash(void) noexcept PURE;
public:		virtual void		Jump(void) noexcept PURE;
public:		virtual void		DoubleJump(void) noexcept PURE;
public:		virtual void		Falling(void) noexcept PURE;
public:		virtual	void		Respawn(void) noexcept { __noop; }

public:		virtual void		SetDirection(float angle,float deltatime) noexcept { __noop; };
public:		virtual void		Interaction(void) noexcept PURE;

public:		void				SetFollowCamera(FollowingCamera* camera) noexcept { _camera = camera; }
public:		void				GetFollowCamera(FollowingCamera** camera);
public:		const State&		GetPlayerState(void) noexcept { return _state; }

public:		float				GetMaxHP(void) noexcept { return _maxHp; }
public:		float				GetHP(void)	noexcept { return _hp; }
public:		float				GetHpPer(void) noexcept { return _hp / _maxHp; }

public:		InputHandler*		GetInputHandler(void) noexcept { return _inputHandler; }
public:		HierarcyNode*		GetAlignBone(void) noexcept { return _PickUpBone; }

protected:	Type				_actorType;

protected:	State				_state;
protected:	State				_dirtyState;

protected:	bool				_bOperationPad;
protected:	bool				_isDirtyState = false;

protected:	bool				_isDead = false;
protected:	bool				_isDeadEnd = false;

protected:  bool				_isJump = false;
protected:	bool				_isDoubleJump = false;
protected:  bool				_isDash = false;
protected:	bool				_isInteraction = false;
protected:	bool				_isRun = false;
protected:	bool				_isCrouch = false;
protected:	bool				_isGroundPound = false;
protected:	bool				_isPickUp = false;
protected:	bool				_isWallSlide = false;
protected:	bool				_isSliding = false;
protected:	bool				_OnGround = false;

protected:	bool				_isSawEnter = false;

protected:	int					_JumpCount = 2;
protected:	int					_WallJump  = 1;
protected:	int					_DashCount = 1;

protected:	float				_maxHp;
protected:	float				_hp;
protected:	float				_interactime = 0;
protected:	float				_deadLerpTime = 0.f;
protected:  float				_wallJumpPower = 1.f;

protected:	XMFLOAT3			_lerpPosition;
protected:  float				_lerpRotate;

protected:	InputHandler*		_inputHandler;

protected:	Animator*			_animator;
protected:	ModelRenderer*		_renderer;
protected:	Rigidbody*			_rigidbody;
protected:	FollowingCamera*	_camera;
protected:	XMFLOAT3			_lerpVector;
protected:	HierarcyNode*		_PickUpBone;

protected:	GameObject*			_interactObj;
protected:	GameObject*			_ValveInteractUseObject;
protected:  GameObject*			_savePoint;

protected:	EffectPlayer*		_movementEffectPlayer;
protected:	EffectPlayer*		_voiceEffectPlayer;
protected:	AudioAsset*			_audioasset[static_cast<uint>(eAudioType::END)];

protected:	float				_speed = 0.f;



};

