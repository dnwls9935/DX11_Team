#pragma once

#include "Behaviour.h"

namespace ce
{
	class Animator;
}

class SuctionHoze : public Behaviour
{
/* VacuumHoseHead.ao| */
private:	enum class ANIMATION
{
	VacuumHoseHead_Blow_MH
	, VacuumHoseHead_BlowOut
	, VacuumHoseHead_Boss_MH
	, VacuumHoseHead_Boss_Smash_Hit
	, VacuumHoseHead_Riding_Blow_MH
	, VacuumHoseHead_Riding_Suck_MH	
	, VacuumHoseHead_Stunned_Enter
	, VacuumHoseHead_Stunned_MH
	, VacuumHoseHead_Suck_MH
	, VacuumHoseHead_SuckIn 
};
public:		SuctionHoze(void) noexcept;
public:		virtual ~SuctionHoze(void) noexcept;

public:		void Start(void) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		void SetFuseLight(int index, GameObject* fuseLight) { _fuseLights[index] = fuseLight; };

public:		void SetSuction(bool b);
public:		bool GetSuction() { return _suction; }

private:	Animator* _animator;

private:	GameObject* _fuseLights[3] = {nullptr};
private:	bool		_suction;
};

