#include "pch.h"
#include "StonePillar.h"
#include "GameObject.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "Transform.h"
#include "ModelRenderer.h"
#include "ParticleSystem.h"
#include "Animator.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

StonePillar::StonePillar(void) noexcept
	:Destruction(500)
{
}

void StonePillar::Start(void) noexcept
{
	//Renderer
	XMMATRIX offset = XMMatrixScaling(0.01f, 0.01f, 0.01f);// *XMMatrixRotationX(CE_MATH::ToRadian(90));
	_renderer = ModelRenderer::Create(false);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Break_Wall_01_Pillar\\Break_Wall_01_Pillar.fbx"));
	_renderer->SetOffsetMatrix(offset);
	gameObject->AddComponent(_renderer);

	//Rigidbody
	Rigidbody* rigid = Rigidbody::Create(false);
	rigid->SetIsKinematic(true);
	gameObject->AddComponent(rigid);

	//Collider
	_collider0 = Collider::CreateBoxCollider(1, 2, 1, XMVectorSet(0, 2, 0, 0));
	gameObject->AddComponent(_collider0);

	_collider1 = Collider::CreateBoxCollider(1, 2, 1, XMVectorSet(0, 2, 0, 0));
	_collider1->SetIsTrigger(true);
	gameObject->AddComponent(_collider1);

	__super::Start();

	_breakAsset = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Stage2\\enviroment_Sound\\Play_World_Playroom_Castle_Dungeon_Destructible_StonePillar.ogg");

}

void StonePillar::Die(void) noexcept
{
	XMMATRIX offset = XMMatrixScaling(0.01f, 0.01f, 0.01f); //* XMMatrixRotationY(CE_MATH::ToRadian(180));
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Castle_Dungeon\\Break_Wall_01_Pillar\\Breakwall\\breakwall3.fbx"));
	_renderer->SetOffsetMatrix(offset);

	_animator = Animator::Create();
	gameObject->AddComponent(_animator);
	_animator->Play((uint16)0
		, false
		, std::bind(&StonePillar::OnEndAnimation, this)
		, 0.7f
		, std::bind(&StonePillar::Idle, this));
	//	[&]() {static_cast<Destruction*>(this)->Idle();});
	_animator->SetPlaySpeed(1.f);

	_collider0->SetEnable(false);
	_collider1->SetEnable(false);

	_breakEffectPlayer->Play(_breakAsset, 0.5f);

}
