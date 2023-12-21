#pragma once

#include "Scene.h"

class WareHouse_03 : public Scene
{
public:		WareHouse_03(void) noexcept = default;
public:		virtual ~WareHouse_03(void) noexcept = default;


public:		void		SetLoadingContents(void) noexcept override;
public:		HRESULT		Init(void) noexcept override;
public:		void		FixedUpdate(float fElapsedTime) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;
public:		void		LateUpdate(float fElapsedTime) noexcept override;
public:		void		Render(void) noexcept override;
public:		void		LateRender(void) noexcept override;
public:		void		Release(void) noexcept override;

private:	float		_divideTime;
private:	bool		_divide;
};

