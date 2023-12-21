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

class PlayerScore : public Behaviour
{
public:		enum class PLAYER { CODY, MAY, END };

public:		PlayerScore(PLAYER player) noexcept;
public:		virtual ~PlayerScore(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	SetScore(int score) noexcept;
public:		int		GetScore(void) noexcept { return _currentScore; }

public:		void	SetAlpha(float alpha) noexcept;
	  
private:	PLAYER						_player;

private:	GameObject*					_borderObject;
private:	Transform*					_borderTransform;
private:	UI::RectTransform*			_borderRectTransform;
private:	UI::Image*					_borderImage;

private:	GameObject*					_scoreObject[16];
private:	Transform*					_scoreTransform[16];
private:	UI::RectTransform*			_scoreRectTransform[16];
private:	UI::Image*					_scoreImage[16];

private:	int							_currentScore;


};

