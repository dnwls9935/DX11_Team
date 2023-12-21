#include "pch.h"
#include "BaseballBall.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "BaseballPlayerScore.h"
#include "BaseballScore.h"
#include "Damaged.h"
#include "EffectPlayer.h"
#include "AudioSource.h"

BaseballBall::BaseballBall(void) noexcept
{

}

void BaseballBall::Start(void) noexcept
{
	_effectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_effectPlayer);
}

void BaseballBall::Update(float fElapsedTime) noexcept
{
	if (_playerScore == nullptr)
	{
		GameObject* obj = GameObject::FindObjectByName("BaseballScoreObject");

		if (gameObject->GetName() == "Player02")
			_playerScore = obj->GetComponent<BaseballScore>()->Getplayer(BaseballScore::PLAYER::MAY);
		if (gameObject->GetName() == "Player01")
			_playerScore = obj->GetComponent<BaseballScore>()->Getplayer(BaseballScore::PLAYER::CODY);
	}

}

void BaseballBall::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	GameObject* obj = other->GetGameObject(); 
	
	if (_playerScore->GetStart())
	{
		if (obj->GetName() == "ToyBody01")
		{
			gameObject->GetComponent<Rigidbody>()->AddForce(XMVectorSet(0, 0, 1, 0) * 2000.f);
			_playerScore->SetScore();
			GameObject* score = GameObject::Instantiate("Score");
			GameObject* newtransform = GameObject::Instantiate();
			newtransform->GetTransform()->SetWorldPosition(obj->GetTransform()->GetWorldPosition());
			newtransform->GetTransform()->Translate(+1.5f, +6.62f, 0, true);

			score->AddComponent(new Damaged(newtransform->GetTransform(), 1, 1));
		}

		if (obj->GetName() == "ToyBody02")
		{
			gameObject->GetComponent<Rigidbody>()->AddForce(XMVectorSet(0, 0, 1, 0) * 2000.f);
			_playerScore->SetScore();
			GameObject* score = GameObject::Instantiate("Score");
			GameObject* newtransform = GameObject::Instantiate();
			newtransform->GetTransform()->SetWorldPosition(obj->GetTransform()->GetWorldPosition());
			newtransform->GetTransform()->Translate(-1.5f, +6.62f, 0, true);

			score->AddComponent(new Damaged(newtransform->GetTransform(), 1, 2));
		}

		_effectPlayer->Play(ASSETMANAGER->GetAudioAsset("Asset\\Audio\\MiniGame\\baseball\\Play_World_SideContent_Playroom_Minigames_Baseball_Hit.ogg"), 0.1f);

	}
}
