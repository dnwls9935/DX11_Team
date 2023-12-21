#pragma once

#include "Behaviour.h"

namespace ce::UI {
	class Image;
	class Text;
}

class MainBanner;
class CountBanner;
class ScoreBanner;
class WinLoseBanner;

class MiniGameUI : public Behaviour
{
public:		typedef struct tagvectext
							{
								std::wstring maintext;
								std::wstring infotext;
								std::wstring infotextin;
							} MINIGAMETEXT;

public:		enum class STEP { INTRO, COUNT, INGAME, WIN_AND_LOSE, END };

public:	MiniGameUI(const MINIGAMETEXT& vectext) noexcept;
public: virtual ~MiniGameUI(void) noexcept = default;

public:	void	Start(void) noexcept override;
public:	void	Update(float fElapsedTime) noexcept override;
public: void	OnMainBanner(void) noexcept;
public:	STEP	GetStep(void) noexcept { return _step; }

private:	MINIGAMETEXT	_textInfo;
private:	UI::Text*		_readyText;
private:	UI::Image*		_countBackGround;
private:	UI::Image*		_countNumber;

private:	MainBanner*		_mainBanner;
private:	CountBanner*	_countBanner;
private:	ScoreBanner*	_scoreBanner;
private:	WinLoseBanner*	_winloseBanner;

private:	STEP			_step;

private:	bool			_loading;

};

