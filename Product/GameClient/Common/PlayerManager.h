#pragma once
#include "Behaviour.h"

class Actor;
class RespawnCircle;

class PlayerManager : public Behaviour
{
public:		PlayerManager(Actor* may, Actor* cody, std::string sceneName) noexcept;
public:		virtual		~PlayerManager(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		void		PlayerRespawn(bool isMay) noexcept;

private:	bool			_allDead;
private:	bool			_grayScale;
private:	float			_time;

private:	bool			_respawn;
private:	bool			_mayDead;
private:	bool			_codyDead;

private:	std::string		_sceneName;

private:	Actor*			_may;
private:	Actor*			_cody;

private:	RespawnCircle* _respawnCircle;
};

