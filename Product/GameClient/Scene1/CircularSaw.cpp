#include "pch.h"
#include "CircularSaw.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Animator.h"

#include "FuseCrab.h"

#include "May.h"
#include "Cody.h"
#include "AudioSource.h"
#include "EffectPlayer.h"


CircularSaw::CircularSaw(void) noexcept
	: _awake{}
	, _run(false)
	, _animator(nullptr)
	, _mayGrab(nullptr)
	, _codyGrab(nullptr)
	, _collider(nullptr)
	, _rigid(nullptr)
	, _audioAsset{}
	, _audioSource(nullptr)
{

}

CircularSaw::~CircularSaw(void) noexcept
{

}

void CircularSaw::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\CircularSaw\\CircularSaw.fbx"));
	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	renderer->SetOffsetMatrix(offsetmatrix);


	_defaultRotation = CE_MATH::Deg2Rad * XMVectorSet(0, -90.f, 44.239f, 0);
	transform->SetWorldEulerAngle(_defaultRotation);

	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play("CircularSaw.ao|CircularSaw_EV_Shed_Awakening_Saw_Mh"
					, true);

	_animator->SetPlaySpeed(1.f);

	_awake[(int)PLAYER::MAY] = false;
	_awake[(int)PLAYER::CODY] = false;
	_run = false;

	{
		_mayGrab = GameObject::Instantiate("CirculaSawMayGrab");

		/* Parent 있을때*/
		 _mayGrab->GetTransform()->SetWorldPosition(XMVectorSet(5.15f, 7.41f , -71.148f, 1));

		_codyGrab = GameObject::Instantiate("CirculaSawCodyGrab");

		/* Parent 있을때*/
		_codyGrab->GetTransform()->SetWorldPosition(XMVectorSet(6.237f, 7.542f , -71.243f, 1));
	}


	_rigid = Rigidbody::Create(true);
	gameObject->AddComponent(_rigid);

	_collider = Collider::CreateSphereCollider(7.f);
	_collider->SetIsTrigger(true);
	gameObject->AddComponent(_collider);

	_audioAsset[static_cast<uint>(eAudioType::SUCCESS)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\CircularSaw\\Play_SFX_EVShed_Awakening_Saw_Success.ogg");
	_audioAsset[static_cast<uint>(eAudioType::START)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\CircularSaw\\Play_World_Shed_Awakening_Interactable_CircularSaw_Start.ogg");
	_audioAsset[static_cast<uint>(eAudioType::SPARK)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\CircularSaw\\Play_World_Shed_Awakening_Interactable_CircularSaw_Spark.ogg");
	_audioAsset[static_cast<uint>(eAudioType::LOOP)] = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\CircularSaw\\World_Shed_Awakening_Interactable_CircularSaw_Loop.ogg");

	_audioSource = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_audioSource);

}

void CircularSaw::FixedUpdate(float fElapsedTime) noexcept
{
	if (true == _awake[(int)PLAYER::MAY] &&
		true == _awake[(int)PLAYER::CODY] &&
		_run == false) 
	{
		_run = true;
	}

	if ((_awake[static_cast<uint>(PLAYER::MAY)] || _awake[static_cast<uint>(PLAYER::CODY)]) && _start == false)
	{
		_start = true;

		_audioSource->Play(_audioAsset[static_cast<uint>(eAudioType::START)] , 0.01f , false , std::bind(&CircularSaw::OnEndStartAudio, this),false,true);

	}

}

void CircularSaw::Update(float fElapsedTime) noexcept
{
	
}

void CircularSaw::LateUpdate(float fElapsedTime) noexcept
{

	if(true == _run)
	{
		_animator->Play("CircularSaw.ao|Saw_EV_Shed_Awakening_Saw_Succsess"
						, false
						, std::bind(&CircularSaw::OnSuccessAniamtionEnd, this)
						, 0.1f
						, std::bind(&CircularSaw::OnIdleRunAimation, this));
		_run = false;
		_awake[(int)PLAYER::MAY] = false;
		_awake[(int)PLAYER::CODY] = false;

		_animator->SetPlaySpeed(1.f);
		_audioSource->Play(_audioAsset[static_cast<uint>(eAudioType::SUCCESS)],0.1f,false,nullptr,false,true);

	}
}

void CircularSaw::OnSuccessAniamtionEnd() noexcept
{
	_animator->SetPlaySpeed(0.f);
}

void CircularSaw::OnIdleRunAimation() noexcept
{
	GameObject* obj = GameObject::FindObjectByName("FuseCrab2");
	obj->GetComponent<Animator>()->SetPlaySpeed(1.f);
}

void CircularSaw::OnEndStartAudio() noexcept
{

	_audioSource->Play(_audioAsset[static_cast<uint>(eAudioType::LOOP)] , 0.01f , true,nullptr,false,true);
}

XMMATRIX CircularSaw::GetGrabMatrix(PLAYER player)
{
	switch (player)
	{
	case CircularSaw::PLAYER::MAY:
		return _mayGrab->GetTransform()->GetWorldMatrix();
		break;
	case CircularSaw::PLAYER::CODY:
		return _codyGrab->GetTransform()->GetWorldMatrix();
		break;
	}

	return XMMatrixIdentity();
}
