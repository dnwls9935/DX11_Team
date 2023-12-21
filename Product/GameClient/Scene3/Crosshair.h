#pragma once
#include "Behaviour.h"

namespace ce
{
	class Camera;
}

class Crosshair :public Behaviour
{
public:		Crosshair(bool bOperationPad) noexcept;
public:		virtual		~Crosshair(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		void		Invisible(void) noexcept;

public:		float		GetEndTime(void) noexcept { return _endTime; }
public:		float		GetLength(void) noexcept { return _length; }

public:		Vector3		ShootFromCross(void) noexcept;

private:	bool		_move;
private:	float		_time;
private:	float		_endTime;
private:	float		_length;
private:	Vector3		_movePos;
private:	Vector3		_mousePos;

private:	bool		_bOperationPad;
private:	Transform*	_crossTrans;
private:	Camera*		_camera;
private:	Transform*	_cameraTrans;
};

