#pragma once

#include "Scene.h"

class May;
class Cody;

class WareHouse_01 : public Scene
{
public:		WareHouse_01(void) noexcept = default;
public:		virtual ~WareHouse_01(void) noexcept = default;

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
private:	bool		_cinema;

private:	May* _may;
private:	Cody* _cody;
};

