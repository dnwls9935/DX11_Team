#include "pch.h"
#include "WallSlider.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "PressBtn.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

WallSlider::WallSlider(void) noexcept
	: _lerpTime(0.f)
	, _btnA(nullptr)
	, _btnB(nullptr)
	, _playSound(false)
	, _playAni(false)
{

}

WallSlider::~WallSlider(void) noexcept
{

}

void WallSlider::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Shed2\\Machine\\WallSlider.fbx"));
	gameObject->AddComponent(renderer);
	
	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f);

	renderer->SetOffsetMatrix(offsetmatrix);

	XMVECTOR position = XMVectorSet(-64.16f, 32.66f, -110.3f, 1.f);
	transform->SetWorldPosition(position);

	XMVECTOR angle = CE_MATH::Deg2Rad * XMVectorSet(0, 0, 0, 0);
	transform->SetWorldEulerAngle(angle);

	_reverse = false;
	_angle = 0.f;

	Rigidbody* rigid = Rigidbody::Create(false);
	gameObject->AddComponent(rigid);

	rigid->SetIsKinematic(true);

	rigid->SetFreezePositionX(true);
	rigid->SetFreezePositionY(true);
	rigid->SetFreezePositionZ(true);

	rigid->SetFreezeRotationX(true);
	rigid->SetFreezeRotationY(true);
	rigid->SetFreezeRotationZ(true);

	rigid->SetUseGravity(false);

	Collider* collider = Collider::CreateBoxCollider(0.1f, 3.f, 1.5f, XMVectorSet(-3.f, 3.f, 0.6f, 1.f));
	collider->SetIsTrigger(true);
	gameObject->AddComponent(collider);

	


	{
		GameObject* wallSliderSurface = GameObject::Instantiate("WallSliderSurface");
		wallSliderSurface->SetTag(GameObjectTag::ROT_OBSTACLE);
		wallSliderSurface->GetTransform()->SetParent(transform);
		wallSliderSurface->GetTransform()->SetLocalPosition(XMVectorSet(-2.09f, -0.1299973f, 0.3400001f, 1.f));

		ModelRenderer* wallSliderRenderer = ModelRenderer::Create();
		wallSliderRenderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Shed2\\Machine\\WallSlider_surface.fbx"));
		wallSliderRenderer->SetOffsetMatrix(offsetmatrix);
		wallSliderSurface->AddComponent(wallSliderRenderer);

	}

	_body = GameObject::Instantiate("WallBody");
	_body->SetTag(GameObjectTag::DEFAULT);
	_body->GetTransform()->SetParent(transform);
	_body->GetTransform()->SetLocalPosition(XMVectorSet(-0.2f, 3.3f, 0.f, 1.f));

	Rigidbody* Bodyrigid = Rigidbody::Create(false);
	_body->AddComponent(Bodyrigid);

	Bodyrigid->SetIsKinematic(true);

	Bodyrigid->SetFreezePositionX(true);
	Bodyrigid->SetFreezePositionY(true);
	Bodyrigid->SetFreezePositionZ(true);

	Bodyrigid->SetFreezeRotationX(true);
	Bodyrigid->SetFreezeRotationY(true);
	Bodyrigid->SetFreezeRotationZ(true);

	Bodyrigid->SetUseGravity(false);

	Collider* Body = Collider::CreateCapsuleCollider(2.5f, 3.f);
	_body->AddComponent(Body);

	gameObject->SetTag(GameObjectTag::ROT_OBSTACLE);

	_flipSound = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\Machine\\WallSlider\\Play_World_Shed_Vacuum_Platform_FlipPlatform_OnFlip.ogg");


	_audioSource = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_audioSource);
}

void WallSlider::FixedUpdate(float fElapsedTime) noexcept
{	
	if (true == _btnA->GetComponent<PressBtn>()->GetPress() &&
		true == _btnB->GetComponent<PressBtn>()->GetPress())
	{
		_playAni = true;
	}
}

void WallSlider::Update(float fElapsedTime) noexcept
{
}

void WallSlider::LateUpdate(float fElapsedTime) noexcept
{
	if (_playAni)
	{

		if (!_reverse)
		{
			_lerpTime += fElapsedTime * 1.5f;

			if (_lerpTime >= 1.5f)
			{
				_reverse = !_reverse;
				_playSound = false;
			}
		}
		else
		{
			_lerpTime -= fElapsedTime * 1.5f;

				if (_lerpTime <= -0.5f)
				{
					_reverse = !_reverse;
					_playSound = false;
				}
		}


		if (CE_MATH::equl_f(_lerpTime , 0.5f) && !_playSound)
		{

			_audioSource->Play(_flipSound, 0.5f, false, nullptr, false, true);
			_playSound = true;
		}

		float t = CETween::Lerp(0.f, 180.f, std::max(0.f, std::min(1.f, _lerpTime)), CETween::EaseType::easeInOutQuart);


		transform->SetWorldEulerAngle(CE_MATH::Deg2Rad * XMVectorSet(0, t, 0, 1));
	}
}
