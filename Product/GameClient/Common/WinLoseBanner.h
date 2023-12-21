#pragma once

#include "Behaviour.h"

namespace ce
{
	namespace UI
	{
		class Image;
		class Text;
		class RectTransform;
	}
}
class EffectPlayer;

class WinLoseBanner : public Behaviour
{
public:		WinLoseBanner(void) noexcept;
public:		virtual ~WinLoseBanner(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	SetText(std::string str) noexcept;
public:		bool	End(void) noexcept { return _end; };
public:		void	PlaySound(void) noexcept;

private:	float					_lerpTime;

private:	float					_endTime;
private:	bool					_end;

private:	GameObject*				_backGroundObject;
private:	Transform*				_backGroundTransform;
private:	UI::RectTransform*		_backGroundRectTransform;
private:	UI::Image*				_backGroundImage;

private:	GameObject*				_textObject;
private:	Transform*				_textTransform;
private:	UI::RectTransform*		_textRectTransform;
private:	UI::Text*				_text;
private:	EffectPlayer*			_effectPlayer;
};
