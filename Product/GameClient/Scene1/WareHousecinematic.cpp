#include "pch.h"
#include "WareHouseCinematic.h"
#include "WareHouseCinematicPlayer.h"
#include "GameObject.h"
#include "NarrationManager.h"
#include "PathFollower.h"
#include "FollowingCamera.h"
#include "Transform.h"
#include "PlayerScreen.h"
#include "Animator.h"
#include "ChinemaScope.h"
#include "BGMPlayer.h"
#include "ModelRenderer.h"
#include "EffectPlayer.h"
#include <atlstr.h>


WareHouseCinematic::WareHouseCinematic(const std::vector<CINEMADESC>& cinemadesc, GameObject* CameraObj[2]) noexcept
	: Cinematic(cinemadesc)
	, _On{}
{
	memcpy(_cameraobj, CameraObj, sizeof(GameObject*) * 2);
}

void WareHouseCinematic::Start(void) noexcept
{

	AssetManager::CONST_PTR_CSVDATA csvdata = ASSETMANAGER->GetCSVData(_cinemaDesc[0].csvfilename.c_str());
	if (csvdata->empty())
	{
		CE_ASSERT("rldnr", "csvdata가 empty 입니다.")
		return;
	}

	
	XMMATRIX offsetmatrix = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&_cinemaDesc[0].offsetrotation[0])));
	_cinematicMay = GameObject::Instantiate("CinemaMay");

	CinematicPlayer::CHARACTORDESC chardesc{};
	chardesc.modelname = (*csvdata)[1][static_cast<uint>(columID::MAYMODEL)];
	chardesc.animationindex = _cinemaDesc[0].animationindex[0];
	chardesc.animationspeed = _cinemaDesc[0].animationspeed;
	XMStoreFloat4x4(&chardesc.offsetmatrix, offsetmatrix);
	_cinematicMay->GetTransform()->SetLocalPosition(XMLoadFloat3(&_cinemaDesc[0].worldposition[0]));

	_cinematicMay->AddComponent(new WareHouseCinematicPlayer(chardesc));
	_cinematicMay->GetComponent<WareHouseCinematicPlayer>()->SetColorDepth(Vector4(0.f, 0.f, 1.f, 0.5f));
	_cinematicMay->SetActive(false);

	
	/* ========================================= */

	offsetmatrix = XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&_cinemaDesc[0].offsetrotation[1])));

	_cinematicCody = GameObject::Instantiate("CinemaCody");

	chardesc.modelname = (*csvdata)[1][static_cast<uint>(columID::CODYMODEL)];
	chardesc.animationindex = _cinemaDesc[0].animationindex[1];
	chardesc.animationspeed = _cinemaDesc[0].animationspeed;
	XMStoreFloat4x4(&chardesc.offsetmatrix, offsetmatrix);
	_cinematicCody->GetTransform()->SetLocalPosition(XMLoadFloat3(&_cinemaDesc[0].worldposition[1]));


	_cinematicCody->AddComponent(new WareHouseCinematicPlayer(chardesc));
	_cinematicCody->GetComponent<WareHouseCinematicPlayer>()->SetColorDepth(Vector4(0.f, 1.f, 0.f, 0.5f));

	_cinematicCody->SetActive(false);
	


	
	/* ========================================== */

	_narrationname.emplace_back((*csvdata)[1][static_cast<uint>(columID::NARRATION)].c_str());
	
	_follower = PathFollower::Create(ASSETMANAGER->GetPathData((*csvdata)[1][static_cast<uint>(columID::PATH)].c_str()));
	if (!_follower)
	{
		CE_ASSERT("rldnr", "PathFollower 가 nullptr 입니다.");
		return;
	}
	_cameraobj[0]->AddComponent(_follower);
	_follower->SetEnable(false);

	_musEffectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_musEffectPlayer);

	_sfxEffectPlayer = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_sfxEffectPlayer);

}

void WareHouseCinematic::Update(float fElapsedTime) noexcept
{
	if (_On[0] && _On[1])
	{
		Play();
		_On[0] = false;
		_On[0] = false;
	}
}

void WareHouseCinematic::Play(void) noexcept
{
	PlayerScreen::This().ShowOnlyL([&]() {
		ChinemaScope::This()->Play(0.3f, 0.5f);
		});
	BGMPlayer::This()->ChangeVolume(0.2f);


	_cameraobj[0]->GetComponent<FollowingCamera>()->SetEnable(false);
	_follower->SetEnable(true);
	_follower->Play(_cinemaDesc[0].followtime, false,std::bind(&WareHouseCinematic::OnEndCinema,this));


	auto players = GameObject::FindObjectsByTag(GameObjectTag::PLAYER);
	for (auto& player : players)
	{
		player->SetActive(false);
	}

	_cinematicMay->SetActive(true);
	_cinematicCody->SetActive(true);

	if (!NarrationManager::This().SetCsvData(_narrationname[0].c_str()))
	{
		CE_ASSERT("rldnr", "NarrationData가 없습니다.");
	}
	//NarrationManager::This().SetNarrationVolume();
	NarrationManager::This().Play();

}

void WareHouseCinematic::OnEndCinema(void) noexcept
{
	ChinemaScope::This()->Stop(1.f,[&]() {

		PlayerScreen::This().Show();

		});
	BGMPlayer::This()->ChangeVolume(5.f);

	_follower->SetEnable(false);
	_cameraobj[0]->GetComponent<FollowingCamera>()->SetEnable(true);
	_cameraobj[1]->GetTransform()->SetLocalEulerAngle(_cameraobj[0]->GetTransform()->GetLocalEulerAngle());



	auto players = GameObject::FindObjectsByTag(GameObjectTag::PLAYER);
	for (auto& player : players)
	{
		player->SetActive(true);

		if (player->GetName() == "Cody")
		{
			player->GetTransform()->SetLocalPosition(_cinematicCody->GetTransform()->GetLocalPosition());
			player->GetTransform()->SetLocalEulerAngle(XMLoadFloat3(&_cinemaDesc[0].offsetrotation[1]) * 3);
		}
		else
		{
			player->GetTransform()->SetLocalPosition(_cinematicMay->GetTransform()->GetLocalPosition());
			player->GetTransform()->SetLocalEulerAngle(XMLoadFloat3(&_cinemaDesc[0].offsetrotation[0]) * 3);
		}
	}

	_cinematicMay->SetActive(false);
	_cinematicCody->SetActive(false);
}
