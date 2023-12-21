#include "pch.h"
#include "PlayerManager.h"
#include "GameObject.h"
#include "Actor.h"
#include "RespawnCircle.h"
#include "FadeController.h"
#include "RestartScene.h"
#include "ChessScene.h"
#include "NarrationManager.h"

PlayerManager::PlayerManager(Actor* may, Actor* cody, std::string sceneName) noexcept
	: _may(may)
	, _cody(cody)
	, _sceneName(sceneName)
	, _respawn(false)
{
}

void PlayerManager::Start(void) noexcept
{
	_respawnCircle = new RespawnCircle(this, _may, _cody);
	gameObject->AddComponent(_respawnCircle);
}

void PlayerManager::Update(float fElapsedTime) noexcept
{
	if (_allDead)
	{
		if (_grayScale)
		{
			_time = std::min(1.f, _time + fElapsedTime);
			PostEffect::This().setUseGrey(true, _time);

			if (_time == 1.f)
				_grayScale = false;
		}
		return;
	}

	if (_may->IsDead())
	{
		if (!_mayDead)
			_mayDead = true;
	}
	else if(_mayDead)
		_mayDead = false;

	if (_cody->IsDead())
	{
		if(!_codyDead)
			_codyDead = true;
	}
	else if (_codyDead)
		_codyDead = false;

	if (_mayDead && _codyDead)
	{
		if (true == _respawn)
		{
			_respawnCircle->StopRespawn();
		}
		
		_respawn = false;

		_allDead = true;
		_grayScale = true;
		_time = 0;

		NarrationManager::This().Stop();

		FadeController::This()->StartFadeOut(3.0f
			, true
			, false
			, [&]() {
				RestartScene::SetNextSceneName(_sceneName.c_str());
				SCENEMANAGER->LoadScene("Restart");
			});

	}
	else if (false == _respawn)
	{
		if (_mayDead)
		{
			_respawnCircle->RespawnStart(true, _may->GetOperationPad());
			_respawn = true;
		}
		else if (_codyDead)
		{
			_respawnCircle->RespawnStart(false, _cody->GetOperationPad());
			_respawn = true;
		}
	}

}

void PlayerManager::PlayerRespawn(bool isMay) noexcept
{
	_respawn = false;

	if (true == isMay)
	{
		if (false == _mayDead)
			return;

		_may->Respawn();
	}
	else
	{
		if (false == _codyDead)
			return;

		_cody->Respawn();
	}
}
