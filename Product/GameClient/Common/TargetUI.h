#pragma once

#include "Behaviour.h"

class Actor;
class FollowingCamera;
namespace ce
{
	namespace UI
	{
		class Image;
	}
}

class TargetUI : public Behaviour
{
public:		TargetUI(Actor* actor, FollowingCamera* camera) noexcept;
public:		virtual ~TargetUI(void) noexcept = default;

public:	void Start(void) noexcept override;
public: void Update(float fElapsedTime) noexcept override;
public:	void FixedUpdate(float fElapsedTime) noexcept override;
public:	void LateUpdate(float fElapsedTime) noexcept override;

private:	Actor* _actor;
private:	UI::Image* _targetimage;
private:	FollowingCamera* _camera;
};

