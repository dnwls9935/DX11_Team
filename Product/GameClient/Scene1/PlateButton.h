#pragma once
#include "Behaviour.h"

class PlateButton: public Behaviour
{
public:					PlateButton(bool isReturn
									, std::function<void()> onEnterEvent
									, std::function<void()> onExitEvent = nullptr) noexcept;
public:		virtual		~PlateButton(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		void		OnTriggerEnter(Collider* mine, Collider* other) noexcept override;
public:		void		OnTriggerExit(Collider* mine, Collider* other) noexcept override;

private:	int			_enterCount;

private:	bool		_isReturn;
private:	bool		_isEnd;

private:	bool		_isPush;
private:	bool		_isPop;

private:	float		_time;
private:	float		_localY;
		
private:	std::function<void()>	_onEnterEvent;
private:	std::function<void()>	_onExitEvent;

private:	Transform* _buttonTrans;
};

