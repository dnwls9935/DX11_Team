#include "pch.h"
#include "BandSawBlade.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "AudioSource.h"
#include "PressBtn.h"
#include "EffectPlayer.h"

BandSawBlade::BandSawBlade(void) noexcept
	: _bandsaw(nullptr)
	, _audioSource(nullptr)
	, _btnA(nullptr)
	, _btnB(nullptr)
	, _playSound(false)
{

}

BandSawBlade::~BandSawBlade(void) noexcept
{

}

void BandSawBlade::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Shed2\\Machine\\SawBench_Blade.fbx"));
	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
							* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(0
																						, 0
																						, 0));
	renderer->SetOffsetMatrix(offsetmatrix);

	XMVECTOR angle = CE_MATH::Deg2Rad * XMVectorSet(-89.98f, 180.f, 0, 0);
	transform->Rotate(angle);

	_originY = XMVectorGetY(transform->GetWorldPosition());

	b = false;

	_bandsaw = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\Machine\\Play_World_Shed_Awakening_Amb_Spot_Bandsaw.ogg");
	_audioSource = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_audioSource);
}

void BandSawBlade::Update(float fElapsedTime) noexcept
{

	if (true == _btnA->GetComponent<PressBtn>()->GetPress() &&
		true == _btnB->GetComponent<PressBtn>()->GetPress() &&
		_playSound == false)
	{
		_playSound = true;

		_audioSource->Play(_bandsaw,0.05f,false,nullptr,false,true);
	}



	if(b)
		transform->Translate(XMVectorSet(0, -1.5f, 0, 0));
	else
		transform->Translate(XMVectorSet(0, 1.5f, 0, 0));

	b = !b;
}