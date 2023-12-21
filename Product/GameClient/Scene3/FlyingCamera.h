#pragma once
#include "Behaviour.h"

namespace ce
{
	class Camera;
}

class FlyingCamera : public Behaviour
{
public:		FlyingCamera(Transform* player) noexcept;
public:		~FlyingCamera(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		void		SetBoost(bool use) noexcept;

private:	Transform*	_player;

private:	bool		_usingBoost;
private:	bool		_endBoost;
private:	bool		_isBoostLerp;
private:	float		_boostTime;

private:	float		_time;

private:	bool		_isLerpY;
private:	float		_lerpAngleY;
private:	float		_startAngleY;
private:	float		_lerpTimeY;
private:	float		_timeY;


private:	float		_prevX;
private:	bool		_isLerpX;
private:	float		_lerpAngleX;
private:	float		_startAngleX;
private:	float		_lerpTimeX;
private:	float		_timeX;

private:	float		_y;
private:	float		_x;

private:	Vector3		_playerOffset;
private:	Vector3		_cameraOffset;
private:	Vector3		_boostOffset;

private:	Camera* _camera;
};

