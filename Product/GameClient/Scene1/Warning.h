#pragma once

#include "Behaviour.h"

namespace ce
{
	namespace UI
	{
		class Image;
		class RectTransform;
	}
}

class Warning : public Behaviour
{
public:		Warning(void) noexcept;
public:		virtual ~Warning(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	Show(void) noexcept;

private:	UI::Image*			_img;
private:	UI::RectTransform*		_rectTransform;

private:	bool				_show;
private:	float				_timeAcc;
};