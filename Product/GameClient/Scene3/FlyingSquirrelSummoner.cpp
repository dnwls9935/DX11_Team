#include "pch.h"
#include "FlyingSquirrelSummoner.h"
#include "Collider.h"
#include "GameObject.h"
#include "NarrationManager.h"
#include "Warning.h"

void FlyingSquirrelSummoner::Start(void) noexcept
{
	GameObject* warning = GameObject::Instantiate("Warning");
	warning->AddComponent(_warning = new Warning());
}

void FlyingSquirrelSummoner::Update(float fElapsedTime) noexcept
{
}//

void FlyingSquirrelSummoner::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	if (_summon == false)
	{
		if (other->GetGameObject()->GetTag() == GameObjectTag::PLAYER)
		{
			for (int i = 0; i < _squirrels.size(); ++i)
			{
				_squirrels[i]->GetGameObject()->SetActive(true);

			}
			if (!NarrationManager::This().SetCsvData("Asset\\Narrative\\Tree\\SquirrelFighter_Spawn.csv"))
			{
				CE_ASSERT("rldnr", "NarrationData가 없습니다.");
			}
			NarrationManager::This().SetNarrationVolume(2.f);
			NarrationManager::This().Play();

			_summon = true;
			_warning->Show();
		}
	}
}

void FlyingSquirrelSummoner::AddSquirrel(FlyingSquirrel* squirrel) noexcept
{
	_squirrels.push_back(squirrel);
}
