#pragma once
#include "Scene.h"

class BaseballCody;
class BaseballMay;

class BaseballScene : public Scene
{
public:		BaseballScene(void) noexcept = default;
public:		virtual ~BaseballScene(void) noexcept = default;

public:		void		SetLoadingContents(void) noexcept override;
public:		HRESULT		Init(void) noexcept override;
public:		void		FixedUpdate(float fElapsedTime) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		LateUpdate(float fElapsedTime) noexcept override;
public:		void		Render(void) noexcept override;
public:		void		LateRender(void) noexcept override;
public:		void		Release(void) noexcept override;


private:	BaseballMay*				_may;
private:	BaseballCody*				_cody;

private:	GameObject*					_save;
};

