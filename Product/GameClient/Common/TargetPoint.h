#pragma once

#include "Behaviour.h"

/* 사용법 : AddComponenet 하기전에 
* 필수 입력 :	position
* 선택 입력 :	size
*/

class TargetPoint : public Behaviour
{
public:		enum class targetType
{
	MAY,
	CODY,
	FUSECRAB,
	MAYPOINT,
	CODYPOINT,
	END
};

public:		TargetPoint(FXMVECTOR position,targetType type) noexcept;
public:		TargetPoint(Transform* transform, targetType type) noexcept;
public:		virtual ~TargetPoint(void) noexcept;


public:		void Start(void) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		void SetPointPosition(float x, float y, float z) noexcept;
public:		void SetPointPosition(FXMVECTOR vec) noexcept;

public:		void SetTargetPoint(targetType targettype);

private:	Transform* _transform;
private:	XMFLOAT3	_position;


private:	targetType	_targettype;


};

