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

class BaseballPlayerScore;
class BaseballScore : public Behaviour
{
public:		enum class PLAYER { CODY, MAY, END };

public:		BaseballScore(void) noexcept;
public:		virtual ~BaseballScore(void) noexcept = default;

public:		void			Start(void) noexcept override;
public:		void			Update(float fElapsedTime) noexcept override;

public:		void			GetScore(PLAYER player, int score = 0) noexcept;
public:		void			StartBanner(void) noexcept;
public:		bool			GetEndGame(void) noexcept { return _endGame; };
public:		std::string		GetWinner(void) noexcept { return _winner; };
public:		BaseballPlayerScore* Getplayer(PLAYER player) noexcept { return _playerScore[(int)player]; }


private:	GameObject*					 _playerScoreObject[(int)PLAYER::END];
private:	GameObject*					 _playerNameObject[(int)PLAYER::END];

private:	Transform*					 _playerScoreTransform[(int)PLAYER::END];
private:	BaseballPlayerScore*		 _playerScore[(int)PLAYER::END];

private:	GameObject*					_vsObject;
private:	Transform*					_vsTransform;
private:	UI::RectTransform*			_vsRectTransform;
private:	UI::Image*					_vsImage;
private:	UI::Text*					_nameText[(int)PLAYER::END];


private:	float						_lerpTime;
private:	bool						_lerpEnd;

private:	bool						_endGame;
private:	std::string					_winner;
};

