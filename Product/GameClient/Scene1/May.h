#pragma once

#include "Actor.h"

namespace ce
{
	class HierarcyNode;
	class AudioSource;
}

class May final : public Actor
{


public:		May(bool bOperationPad) noexcept;
public:		virtual ~May(void) noexcept;

public:		void		Start(void) noexcept override;
public:		void		FixedUpdate(float fElapsedTime) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		LateUpdate(float fElapsedTime) noexcept override;

public:		void		Idle(void) noexcept override;
public:		void		Walk(void) noexcept override;
public:		void		Run(void) noexcept override;
public:		void		Die(void) noexcept override;
public:		void		Jump(void) noexcept override ;
public:		void		DoubleJump(void) noexcept override;
public:		void		Falling(void) noexcept override;
public:		void		Dash(void) noexcept override;
public:		void		Attack(void) noexcept override;
public:		void		GetHit(float damage, int8 attacker) noexcept override;
public:		void		SetDirection(float deltatime) noexcept ;
public:		void		CalcDirection(float angle, float deltatime) noexcept;

public:		void		Interaction(void) noexcept override;
public:		void		Crouch(void) noexcept;

public:		void		Suction(float fElapsedTime) noexcept;
public:		void		WallJump(void) noexcept;


public:		void		OnEndJump(void) noexcept;
public:		void		OnEndLand(void) noexcept;
public:		void		OnEndDash(void) noexcept;
public:		void		OnEndDashEnd(void) noexcept;
public:		void		OnEndWalk(void) noexcept;
public:		void		OnEndCrouch(void) noexcept;
public:		void		OnEndGroundPound(void) noexcept;
public:		void		OnEndPickUp(void) noexcept;
public:		void		OnEndWallSlide(void) noexcept;
public:		void		OnEndSawJump(void) noexcept;
public:		void		OnEndSlide(void) noexcept;
public:		void		OnEndPush(void) noexcept;
public:		void		OnEndInteraction(void) noexcept;
public:		void		OnEndLever(void) noexcept;
public:		void		OnEnterInteraction(void) noexcept;

public:		void		UpdateCheckEvent(float fElapsedTime) noexcept;

public:		void		LateUpdateInteractStart(const std::string& objectName) noexcept;
public:		void		LateUpdateInteract(const std::string& objectName) noexcept;
public:		void		LateUpdateInteractIdle(const std::string& objectName) noexcept;
public:		void		LateUpdateInteractEnd(const std::string& objectName) noexcept;

public:		void		OnTriggerDash(void) noexcept;
public:		void		OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void		OnTriggerExit(Collider* mine, Collider* other) noexcept override;

private:	void		InteractPowerSwitch(float fElapsedTime) noexcept;
private:	void		InteractFuseCrab(float fElapsedTime) noexcept;
private:	void		InteractSaw(float fElapsedTime) noexcept;

private:	void		InteractPumpValve(float fElapsedTime) noexcept;
private:	void		InteractPushDumbbell(float fElapsedTime) noexcept;
private:	void		InteractLever(float fElapsedTime) noexcept;
private:	void		InteractLeverReverse(void) noexcept;

	   /* Phyxs 조인트 나오면 고쳐야 할덧 */
private:	void		InteractMachineGear(float fElapsedTime) noexcept;
private:	void		InteractWallslider(float fElapsedTime) noexcept;

private:	HierarcyNode*	_rightHandBone;
private:	GameObject*		_oldFuseCrab;

private:	GameObject*		_leftFootDustFX;
private:	GameObject* 	_rightFootDustFX;

private:	Collider*		_colliderTriggerBody;
private:	Collider*		_collider;

	   /* Test Code */
private:	float			_lerpTime;
};

