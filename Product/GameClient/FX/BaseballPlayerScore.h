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

class BaseballPlayerScore : public Behaviour
{
public:		enum class PLAYER { CODY, MAY, END };

public:		BaseballPlayerScore(PLAYER player) noexcept;
public:		virtual ~BaseballPlayerScore(void) noexcept = default;

public:		void	Start(void) noexcept override;
public:		void	Update(float fElapsedTime) noexcept override;
public:		void	SetScore(void) noexcept;
public:		int		GetScore(void) noexcept { return _currentScore; }
public:		PLAYER		GetPlayer(void) noexcept { return _player; }

public:		void	SetAlpha(float alpha) noexcept;
public:		bool	GetStart(void) noexcept { return _start; }


private:	PLAYER						_player;

private:	GameObject*					_borderObject;
private:	GameObject*					_textObject;


private:	Transform*					_borderTransform;
private:	UI::RectTransform*			_borderRectTransform;
private:	UI::Image*					_borderImage;

private:	UI::Text*					_scoreText;


private:	int							_currentScore;
private:	bool						_start;

};

