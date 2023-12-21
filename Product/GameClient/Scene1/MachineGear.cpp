#include "pch.h"
#include "MachineGear.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"

#include "PressBtn.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

MachineGear::MachineGear(void) noexcept
{

}

MachineGear::~MachineGear(void) noexcept
{

}

void MachineGear::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Shed2\\Machine\\GrindngStone.FBX"));
	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);
	renderer->SetOffsetMatrix(offsetmatrix);

	transform->SetWorldEulerAngle(0, 0, 0);
	transform->SetLocalScale(1.f, 1.f, 1.f);

	/* 피직스 수정중 */
	_rigidbody = Rigidbody::Create(false);

	_rigidbody->SetIsKinematic(true);

	_rigidbody->SetFreezePositionX(true);
	_rigidbody->SetFreezePositionY(true);
	_rigidbody->SetFreezePositionZ(true);

	_rigidbody->SetFreezeRotationX(true);
	_rigidbody->SetFreezeRotationY(true);
	_rigidbody->SetFreezeRotationZ(true);

	_rigidbody->SetUseGravity(false);

	gameObject->AddComponent(_rigidbody);

	_collider = Collider::CreateBoxCollider(5.6f, 5.6f, 1.1f);
	//_collider->SetIsTrigger(true);
	gameObject->AddComponent(_collider);

	gameObject->SetTag(GameObjectTag::OBSTACLE);

	_rotate = false;

	_start = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\Machine\\Play_World_Shed_Awakening_Platform_Sander_Machine_Start.ogg");
	_loop = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\Machine\\Play_World_Shed_Awakening_Platform_Sander_Machine_Loop.ogg");
	_cogLoop = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\Machine\\Play_World_Shed_Main_Amb_Spot_Cogwheels_Large_02.ogg");



	_cogAudioSource = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_cogAudioSource);

	_gearAudioSource = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_gearAudioSource);
}

void MachineGear::FixedUpdate(float fElapsedTime) noexcept
{
 
	_rigidbody->SetFreezeRotationZ(true);
	if (true == _btnA->GetComponent<PressBtn>()->GetPress() &&
		true == _btnB->GetComponent<PressBtn>()->GetPress() &&
		false == _rotate)
	{
		_rigidbody->SetEnable(true);
		_rigidbody->SetFreezeRotationZ(false);
		_rotate = true;


		_cogAudioSource->Play(_cogLoop, 0.05f, true, nullptr, false, true);

		_gearAudioSource->Play(_start,0.05f,false, std::bind(&MachineGear::OnEndStart, this),false, true);

	}
}

void MachineGear::Update(float fElapsedTime) noexcept
{
	if(true == _rotate)
		transform->Rotate(XMVectorSet(0, 0, 1, 0) * fElapsedTime);
}

void MachineGear::LateUpdate(float fElapsedTime) noexcept
{
}

void MachineGear::OnEndStart(void) noexcept
{
	_gearAudioSource->Play(_loop,0.05f,true,nullptr,false,true);
}
