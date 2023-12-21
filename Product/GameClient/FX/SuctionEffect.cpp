#include "pch.h"
#include "SuctionEffect.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"

#include "SuctionHoze.h"

#include "FadeController.h"
#include "LoadingScene.h"
#include "PlayerScreen.h"

#include "ShaderManager.h"

#include "PressBtn.h"
#include "Lever.h"
#include "AudioSource.h"
#include "PathFollower.h"
#include "ParticleSystem.h"
#include "EffectPlayer.h"

SuctionEffect::SuctionEffect(GameObject* targetObject0,
							 GameObject* targetObject1,
							 GameObject* targetObject2,
							 Vector3	 offsetVector1,
							 Vector3	 offsetVector2,
							 bool		 particleInv) noexcept
	: _targetObject0(targetObject0), 
	_targetObject1(targetObject1), 
	_targetObject2(targetObject2),
	_suction(false),
	_nextSuction(""),
	_offsetVector1(offsetVector1),
	_offsetVector2(offsetVector2),
	_particleInv(particleInv),
	_particleSwitch(false),
	_soundcheck(false)

{

}

SuctionEffect::~SuctionEffect(void) noexcept
{
}

void SuctionEffect::Start(void) noexcept
{
	_renderer = ModelRenderer::Create(false);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Effect\\g_vacuum.fbx"));
	gameObject->AddComponent(_renderer);

	GameObject* particleObj = GameObject::Instantiate("particleObj");
	_volumeParticle = ParticleSystem::Create(ParticleSystem::Type::SPRITE, ParticleSystem::Shape::CONE);
	particleObj->GetTransform()->SetParent(transform);
	particleObj->GetTransform()->SetLocalPosition(0, 0, 0);
	
	particleObj->GetTransform()->SetLocalPosition(_offsetVector1.x, _offsetVector1.y, _offsetVector1.z);

	if(_particleInv)
	{ 
		_volumeParticle->SetStartSize(5.0f);
		_volumeParticle->SetEndSize(0.5f);
	}
	else
	{
		_volumeParticle->SetStartSize(0.5f);
		_volumeParticle->SetEndSize(5.0f);

	}
	_volumeParticle->SetLifeTime(1.5f);
	_volumeParticle->SetOffsetVector(_offsetVector2);
	_volumeParticle->SetRange(Vector3::one * 1.f);
	_volumeParticle->SetParticlePerSecond(1.f / 15.f);
	_volumeParticle->SetVelocity(11.5f);
	_volumeParticle->SetStartColor(Vector4(0.1f, 0.1f, 1, 1));
	_volumeParticle->SetEndColor(Vector4(1, 1, 1, 0.4f));
	_volumeParticle->SetPass(ParticleSystem::PASS::VOLUMETRIC);
	_volumeParticle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
		, ASSETMANAGER->GetTextureData("Asset\\Texture\\Noise07.dds"));
	_volumeParticle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_VOLUME
		, ASSETMANAGER->GetTextureData("Asset\\Texture\\smokevol2.dds"));
	_volumeParticle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_COLORGRADIENT
		, ASSETMANAGER->GetTextureData("Asset\\Texture\\GreyGradient.dds"));
	_volumeParticle->SetLifeOpacityInv(_particleInv);
	particleObj->AddComponent(_volumeParticle);
	particleObj->SetLayer(GameObjectLayer::TRANSLUCENT);
	_volumeParticle->Stop();


	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
			* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(270)
																		, CE_MATH::ToRadian(90)
																		, CE_MATH::ToRadian(90)));
	_renderer->SetOffsetMatrix(offsetmatrix);

	if ("WareHouse_02" == SCENEMANAGER->GetPreLoadSceneID()
		|| "WareHouse_03" == SCENEMANAGER->GetPreLoadSceneID())
	{
		if ("VacuumFan" == gameObject->GetName())
			_on = false;
		else
			_on = true;
	}else{
		_on = false;
	}


	if ("SuctionEffect" == gameObject->GetName()
		|| "SuctionEffect2_Enter" == gameObject->GetName())
	{
		_rigid = Rigidbody::Create(false);
		_rigid->SetIsKinematic(true);
		_rigid->SetFreezePositionX(true);
		_rigid->SetFreezePositionY(true);
		_rigid->SetFreezePositionZ(true);

		_rigid->SetFreezeRotationX(true);
		_rigid->SetFreezeRotationY(true);
		_rigid->SetFreezeRotationZ(true);

		_rigid->SetUseGravity(false);

		gameObject->AddComponent(_rigid);

		gameObject->SetLayer(GameObjectLayer::TRANSLUCENT);

		if ("WareHouse_03" == SCENEMANAGER->GetSceneID())
		{
			_collider = Collider::CreateBoxCollider(10, 4, 10, XMVectorSet(0, 0, 7.f, 1.f));
			gameObject->AddComponent(_collider);
			_collider->SetIsTrigger(true);
			_collider->SetEnable(true);
		}
		else
		{
			_collider = Collider::CreateBoxCollider(4, 3, 7, XMVectorSet(0, 0, 7.f, 1.f));
			gameObject->AddComponent(_collider);
			_collider->SetIsTrigger(true);
			_collider->SetEnable(false);
		}
	}
	else if ("SuctionEffect0_Enter" == gameObject->GetName()
		|| "SuctionEffect1_Enter" == gameObject->GetName())
	{
		_rigid = Rigidbody::Create(false);
		_rigid->SetIsKinematic(true);
		_rigid->SetFreezePositionX(true);
		_rigid->SetFreezePositionY(true);
		_rigid->SetFreezePositionZ(true);

		_rigid->SetFreezeRotationX(true);
		_rigid->SetFreezeRotationY(true);
		_rigid->SetFreezeRotationZ(true);

		_rigid->SetUseGravity(false);

		gameObject->AddComponent(_rigid);

		_collider = Collider::CreateBoxCollider(1, 1.5f, 1, XMVectorSet(0, 0, 3, 1.f));
		gameObject->AddComponent(_collider);
		_collider->SetIsTrigger(true);
		_collider->SetEnable(false);

		gameObject->SetLayer(GameObjectLayer::TRANSLUCENT);
	}
	else if ("VacuumFan0" == gameObject->GetName() || "VacuumFan1" == gameObject->GetName() || "VacuumFan" == gameObject->GetName())
	{
		_rigid = Rigidbody::Create(false);
		_rigid->SetIsKinematic(true);
		_rigid->SetFreezePositionX(true);
		_rigid->SetFreezePositionY(true);
		_rigid->SetFreezePositionZ(true);

		_rigid->SetFreezeRotationX(true);
		_rigid->SetFreezeRotationY(true);
		_rigid->SetFreezeRotationZ(true);

		_rigid->SetUseGravity(false);

		gameObject->AddComponent(_rigid);

		_collider = Collider::CreateCapsuleCollider(1.77f, 6.f, XMVectorSet(0, 0, 3, 0.f), XMVectorSet(CE_MATH::Deg2Rad * 90.f,0, CE_MATH::Deg2Rad * 90.f,0));
		gameObject->AddComponent(_collider);
		_collider->SetIsTrigger(true);
		_collider->SetEnable(false);

		gameObject->SetLayer(GameObjectLayer::TRANSLUCENT);
	}
	else if ("VacuumFanBig" == gameObject->GetName())
	{
		_rigid = Rigidbody::Create(true);
		_rigid->SetFreezePositionX(true);
		_rigid->SetFreezePositionY(true);
		_rigid->SetFreezePositionZ(true);

		_rigid->SetFreezeRotationX(true);
		_rigid->SetFreezeRotationY(true);
		_rigid->SetFreezeRotationZ(true);

		_rigid->SetUseGravity(false);

		gameObject->AddComponent(_rigid);

		_collider = Collider::CreateCapsuleCollider(5.f, 12.f, XMVectorSet(0, 0, 7.f, 0.f), XMVectorSet(CE_MATH::Deg2Rad * 90.f, 0, CE_MATH::Deg2Rad * 90.f, 0));
		gameObject->AddComponent(_collider);
		_collider->SetIsTrigger(true);
		_collider->SetEnable(false);
		
		_volumeParticle->SetRange(Vector3::one * 0.2f);
		_volumeParticle->SetLifeTime(2.f);
		_volumeParticle->SetEndSize(7.f);

		gameObject->SetLayer(GameObjectLayer::TRANSLUCENT);
	}
	else {
		gameObject->SetLayer(GameObjectLayer::TRANSLUCENT);
	}
	Material::TList& mats = _renderer->GetMaterials();

	for (int i = 0; i < mats.size(); ++i)
	{
		mats[i].SetShader(ShaderManager::This().GetShader(Shader::Type::MODEL_FORWARD));
		mats[i].SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1.f, 1.f, 1.f, 0.0f));

		//if ("SuctionEffect" == gameObject->GetName()
		//	|| "SuctionEffect2_Enter" == gameObject->GetName()
		//	|| "SuctionEffect0_Enter" == gameObject->GetName()
		//	|| "SuctionEffect1_Enter" == gameObject->GetName())
			//mats[i].SetTexture(ShaderSemantic::Type::TEXTURE_ALPHA, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\mask_3_reverse.dds"));
		//else
			//mats[i].SetTexture(ShaderSemantic::Type::TEXTURE_ALPHA, ASSETMANAGER->GetTextureData("Asset\\Texture\\mask_3_reverse2.dds"));
		

		bool b = true;
		mats[i].GetShader().SetRawValue("useAlpha", &b, sizeof(bool));
	}
	bool entercheck = false;
	entercheck = ("SuctionEffect1_Enter" == gameObject->GetName());
	if (entercheck)
	{
		_volumeParticle->SetLifeTime(1.f);
	}

	bool entercheck1 = false;
	entercheck1 = ("SuctionEffect2_Enter" == gameObject->GetName());
	if (entercheck1)
	{
		_volumeParticle->SetLifeTime(0.6f);
	}

	if ("SuctionEffect2_Exit" == gameObject->GetName())
		_volumeParticle->SetLifeTime(1.f);

	if ("SuctionEffect" == gameObject->GetName() &&
		_targetObject0 == nullptr)
	{
		_volumeParticle->SetLifeTime(0.85f);
		_volumeParticle->SetRange(Vector3::one * 0.2f);
		_volumeParticle->SetStartSize(7.f);
		_volumeParticle->SetEndSize(2.85f);
	}
	_U = 0.f;
	_V = 0.f;

	_loadingCheck = false;

	_activate = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\Suction\\Play_World_Shed_Vacuum_VerticalVacuum_Stomach_SuckEffect_Activate.ogg");
	_loop = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\Suction\\World_Shed_Vacuum_VerticalVacuum_SuckEffect_Loop.ogg");
	_metalLoop = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\Suction\\World_Shed_Vacuum_VerticalVacuum_SuckEffect_MetalScreech_Loop.ogg");

	_audioSource = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_audioSource);

}

void SuctionEffect::FixedUpdate(float fElapsedTime) noexcept
{
	if (_on && !_soundcheck)
	{
		_soundcheck = true;
		_audioSource->Play(_activate, 0.05f, false, [&]() {
			_audioSource->Play(_loop, 0.05f, true, nullptr, false, true);

			}
		, false, true);
	}
}

void SuctionEffect::Update(float fElapsedTime) noexcept
{

	if (nullptr != _targetObject0)
		_on = _targetObject0->GetComponent<SuctionHoze>()->GetSuction();
	else
	{
		if ("VacuumFanBig" == gameObject->GetName())
		{
			bool b1 = false;
			bool b2 = false;
			if (nullptr != _targetObject1)
			{
				b1 = _targetObject1->GetComponent<PressBtn>()->GetPress();
			}
			if (nullptr != _targetObject2)
			{
				b2 = _targetObject2->GetComponent<PressBtn>()->GetPress();
			}

			if (true == b1 || true == b2)
			{
				_collider->SetEnable(true);
				_on = true;
			}
			else
			{
				_collider->SetEnable(false);
				_on = false;
			}
		}
		else if ("VacuumFan0" == gameObject->GetName())
		{
			if (nullptr != _targetObject1 && nullptr != _targetObject2)
			{
				bool b0 = _targetObject1->GetComponent<Lever>()->GetDirection();
				bool b1 = _targetObject2->GetComponent<Lever>()->GetDirection();

				if (true == b0 && true == b1)
				{
					_collider->SetEnable(true);
					_on = true;
				}
				else
				{
					_collider->SetEnable(false);
					_on = false;
				}
			}
		}
		else if ("VacuumFan1" == gameObject->GetName())
		{
			if (nullptr != _targetObject1 && nullptr != _targetObject2)
			{
				bool b0 = _targetObject1->GetComponent<Lever>()->GetDirection();
				bool b1 = _targetObject2->GetComponent<Lever>()->GetDirection();

				if (false == b0 && false == b1)
				{
					_collider->SetEnable(true);
					_on = true;
				}
				else
				{
					_collider->SetEnable(false);
					_on = false;
				}
			}
		}
		else if ("VacuumFan" == gameObject->GetName())
		{
			bool b1 = false;
			if (nullptr != _targetObject1)
			{
				b1 = _targetObject1->GetComponent<PressBtn>()->GetPress();
			}

			if (true == b1)
			{
				_collider->SetEnable(true);
				_on = true;
			}
			else
			{
				_collider->SetEnable(false);
				_on = false;
			}
		}
	}

	gameObject->GetComponent<ModelRenderer>()->SetEnable(_on);

	if ("SuctionEffect1_Enter" == gameObject->GetName())
		_on = true;

	if (_on && !_particleSwitch)
	{
		_volumeParticle->Play();
		_particleSwitch = true;
	}
	else if (!_on && _particleSwitch)
	{
		_volumeParticle->Stop();
		_particleSwitch = false;;
	}

	if(_suction == false)
		_V += fElapsedTime;
	else
		_V -= fElapsedTime;



}

void SuctionEffect::LateUpdate(float fElapsedTime) noexcept
{
	Material::TList& mats = gameObject->GetComponent<ModelRenderer>()->GetMaterials();
	for (int i = 0; i < mats.size(); ++i)
	{
		float2 UV = { 0, _V };
		mats[i].SetValue(ShaderSemantic::Type::FLOAT2_TEXCOORD, &UV, sizeof(float2));
	}

	if (true == _on)
	{
		if(_collider != nullptr)
			_collider->SetEnable(true);

		const std::string& sceneID = SCENEMANAGER->GetSceneID();
		if (("WareHouse_01" == sceneID ||"WareHouse_03" == sceneID) &&
			_loadingCheck == false)
		{
			GameObject* may = GameObject::FindObjectByName("May");
			GameObject* cody = GameObject::FindObjectByName("Cody");
			if (nullptr == may || nullptr == cody)
				return;

			XMVECTOR mayPosition = may->GetTransform()->GetWorldMatrix().r[3];
			XMVECTOR codyPosition = cody->GetTransform()->GetWorldMatrix().r[3];
			XMVECTOR myPosition = transform->GetWorldMatrix().r[3];

			float m, c;
			m = XMVectorGetX(XMVector3Length(mayPosition - myPosition));
			c = XMVectorGetX(XMVector3Length(codyPosition - myPosition));

			if (5 >= m && 5 >= c)
			{
				if ("WareHouse_01" == SCENEMANAGER->GetSceneID())
				{
					FadeController::This()->StartFadeOut(1.0f
						, true
						, false
						, [&]() {
							//LoadingScene::SetNextSceneName("WareHouse_02");
							//SCENEMANAGER->LoadScene("Loading");
							while (SCENEMANAGER->IsWhileLoading()) { __noop; }
							SCENEMANAGER->ActivatePreLoadScene();
						});
					PlayerScreen::This().Hide();

					_loadingCheck = true;

					_on = false;
				}
				else if ("WareHouse_03" == SCENEMANAGER->GetSceneID())
				{
					FadeController::This()->StartFadeOut(1.0f
														, true
														, false
														, [&]() {
															/*LoadingScene::SetNextSceneName("Dungeon");
															SCENEMANAGER->LoadScene("Loading");*/
															while (SCENEMANAGER->IsWhileLoading()) { __noop; }
															SCENEMANAGER->ActivatePreLoadScene();
														});
					PlayerScreen::This().Hide();

					_loadingCheck = true;

					_on = false;
				}
			}
		}
	}
}
