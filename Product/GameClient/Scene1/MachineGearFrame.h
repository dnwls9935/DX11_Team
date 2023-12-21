#pragma once

#include "Behaviour.h"
#include "GameObject.h"

class MachineGearFrame : public Behaviour
{
public:		enum class POSITION{UP, DOWN, END};

public:		MachineGearFrame(POSITION posType) noexcept;
public:		virtual ~MachineGearFrame(void) noexcept;

public:		void Start(void) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		void SetBtn(GameObject* btnA, GameObject* btnB) { _btnA = btnA; _btnB = btnB; };

private:	bool _rotate;
private:	POSITION _posType;
private:	GameObject* _btnA;
private:	GameObject* _btnB;
};
