#pragma once

#include "Behaviour.h"
namespace ce
{
	class HierarcyNode;
}

class AcornShooter;
class BoneChildObject;
class AcornShooter;

class SquirrelTurret : public Behaviour
{
public:		SquirrelTurret(void) noexcept;
public:		virtual ~SquirrelTurret(void) noexcept;

public:		void				Start(void) noexcept override;
public:		void				Update(float fElapsedTime) noexcept override;
public:		void				FixedUpdate(float fElapsedTime) noexcept override;
public:		void				LateUpdate(float fElapsedTime) noexcept override;

private:	float				_time;
private:	Transform*			_leftEye;
private:	Transform*			_rightEye;

private:	AcornShooter*		_left;
private:	AcornShooter*		_right;

private:	BoneChildObject*	_1;
private:	BoneChildObject*	_2;

private:	Transform*			_playerTransform = nullptr;
};


