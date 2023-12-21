#include "pch.h"
#include "FuseCrab.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "BoneChildObject.h"
#include "AnimateSpriteTrail.h"
#include "ParticleSystem.h"
#include "AudioSource.h"
#include "EffectPlayer.h"

#include "TargetPoint.h"

FuseCrab::FuseCrab(int sequence) noexcept
	: _sequence(sequence)
	, _fuselight(nullptr)
{
}

FuseCrab::~FuseCrab(void) noexcept
{
}

void FuseCrab::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Characters\\FuseCrab\\FuseCrab.fbx"));
	gameObject->AddComponent(renderer);

	XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
							* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(0)
																						, CE_MATH::ToRadian(-90)
																						, CE_MATH::ToRadian(0)));
	renderer->SetOffsetMatrix(offsetmatrix);

	_animator = Animator::Create();
	gameObject->AddComponent(_animator);

	XMVECTOR defaultRot = XMVectorSet(0, 0, 0, 0);
	transform->SetWorldEulerAngle(defaultRot);

	XMStoreFloat4(&_defaultRotation, defaultRot);

	switch (_sequence)
	{
	case 0:
		_defaultPosition = XMFLOAT4(-27.45f, -1.58f, 38.31f, 0);
		_animator->Play("FuseCrab.ao|Shed_Awakening_FuseSocket_JumpOut_FuseCrab1"
						, true
						, nullptr
						, 0.16f
						, std::bind(&FuseCrab::OnIntroAnimationFinished, this));
		break;
	case 1:
		_defaultPosition = XMFLOAT4(-24.82f, -1.48f, 39.07f, 0);
		_animator->Play("FuseCrab.ao|Shed_Awakening_FuseSocket_JumpOut_FuseCrab2"
						, true
						, nullptr
						, 0.16f
						, std::bind(&FuseCrab::OnIntroAnimationFinished, this));
		break;
	case 2:

		_defaultPosition = XMFLOAT4(-70.5f, 12.f, 23.8f, 0);
		_animator->Play("FuseCrab.ao|Shed_Awakening_FuseSocket_JumpOut_FuseCrab3"
						, true
						, nullptr
						, 0.13f
						, std::bind(&FuseCrab::OnIntroAnimationFinished, this));
		break;
	}

	transform->SetWorldPosition(XMLoadFloat4(&_defaultPosition));
	_animator->SetPlaySpeed(0.f);

	_rigid = Rigidbody::Create(false);
	_rigid->SetUseGravity(false);
	gameObject->AddComponent(_rigid);
	_rigid->SetSimulation(false);

	_collider = Collider::CreateSphereCollider(2.f);
	_collider->SetIsTrigger(true);
	gameObject->AddComponent(_collider);


	if (_sequence == 2)
	{
		GameObject* fuseCrabUI = GameObject::Instantiate("FuseCrabPickUpPosition");
		//fuseCrabUI->GetTransform()->SetWorldPosition();
		TargetPoint* tp = new TargetPoint(XMVectorSet(3.22f, 25.97f, -57.8f, 1.f), TargetPoint::targetType::FUSECRAB);
		fuseCrabUI->AddComponent(tp);
	}

	_insertaudio = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\FuseSocket\\Play_World_Shed_Awakening_Interactable_FuseSocket_Insert.ogg");
	_lightaudio = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\FuseSocket\\Play_World_Shed_Awakening_Interactable_FuseSocket_Light.ogg");
	_jumpoutaudio = ASSETMANAGER->GetAudioAsset("Asset\\Audio\\Shed\\FuseSocket\\Play_SFX_CS_Shed_Awakening_FuseSocket_JumpOut.ogg");

	_audioSource = new EffectPlayer(AudioSource::Create());
	gameObject->AddComponent(_audioSource);

	/* 걸어가는 꽂게는 FX 부착안함 */
	if (_sequence == 2)
		return;

	/* ========FX======== */
	 /* Fuse or Base*/
	GameObject* smokeobject = GameObject::Instantiate("FuseCrabSmoke");
	smokeobject->GetTransform()->SetParent(transform);

	HierarcyNode* BaseBone = renderer->GetModel()->GetHierarchyNode("Fuse");
	BoneChildObject* boneChild = BoneChildObject::Create(BaseBone, offsetmatrix);
	smokeobject->AddComponent(boneChild);

	
	std::vector<Texture*>	textures{};
	std::vector<float>		frames{};
	std::string temp{};

	for (int i = 1; i < 65; ++i)
	{
		temp = "Asset\\Texture\\smokeloop\\smokeloop" + std::to_string(i) + ".dds";
		textures.push_back(ASSETMANAGER->GetTextureData(temp.c_str()));
		frames.push_back( i * 0.0001f );
	}
	temp.clear();

	SpriteAnimation* smoke = new SpriteAnimation(frames, textures, false);
	 _spriteanimaion = new AnimateSpriteTrail(smoke, 100, 0.0003f, true);
	 smokeobject->AddComponent(_spriteanimaion);
	 smokeobject->SetLayer(GameObjectLayer::TRANSLUCENT);

	 delete smoke;
	/* =================== */

	/* =====Particle===== */

	 GameObject* particle = GameObject::Instantiate("FuseCrabParticle");
	 particle->GetTransform()->SetParent(transform);

	 boneChild = BoneChildObject::Create(BaseBone, offsetmatrix);
	 particle->AddComponent(boneChild);

	 _smokeparticle = ParticleSystem::Create();
	 _smokeparticle->SetStartSize(0.35f);
	 _smokeparticle->SetEndSize(0.1f);
	 _smokeparticle->SetLifeTime(0.5f);
	 _smokeparticle->SetRange(Vector3::one * 0.5f);
	 _smokeparticle->SetParticlePerSecond(250.0f / 20000.f);
	 _smokeparticle->SetVelocity(-1.f);
	 _smokeparticle->SetStartColor(Vector4(1.f, 1.f, 1.f, 1.f));
	 _smokeparticle->SetEndColor(Vector4(1.f, 1.f, 1.f, 1.f));
	 _smokeparticle->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE
											, ASSETMANAGER->GetTextureData("Asset\\Texture\\Dungeon\\Golden_Wings_Stars_D.dds"));

	 particle->AddComponent(_smokeparticle);
	 particle->SetLayer(GameObjectLayer::TRANSLUCENT);

	_pickUp = false;

}

void FuseCrab::FixedUpdate(float fElapsedTime) noexcept
{
	if (!_fuselight)
	{
		if (_sequence == 0 && GameObject::FindObjectByName("FuseLight0"))
			_fuselight = GameObject::FindObjectByName("FuseLight0")->GetComponent<FuseLight>();
		else if (_sequence == 1 && GameObject::FindObjectByName("FuseLight1"))
			_fuselight = GameObject::FindObjectByName("FuseLight1")->GetComponent<FuseLight>();
		else if (_sequence == 2 && GameObject::FindObjectByName("FuseLight2"))
			_fuselight = GameObject::FindObjectByName("FuseLight2")->GetComponent<FuseLight>();

	}
}

void FuseCrab::Update(float fElapsedTime) noexcept
{
	/* 아무것도 아닌상태 */
	if (!_pickUp)
	{
		transform->SetWorldEulerAngle(XMLoadFloat4(&_defaultRotation));
	}
}


void FuseCrab::LateUpdate(float fElapsedTime) noexcept
{
	if (_pickUp)
	{
		/* 들고 다니는 상태 */
		if (_playerAlign && _playerTransform)
		{

			XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
				* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(-90)
					, CE_MATH::ToRadian(-90)
					, CE_MATH::ToRadian(0)));
			gameObject->GetComponent<ModelRenderer>()->SetOffsetMatrix(offsetmatrix);

			XMMATRIX offsetMatrix = _playerAlign->GetCombinedTransform()
				* XMMatrixScaling(0.01f, 0.01f, 0.01f)
				* XMMatrixRotationY(CE_MATH::Deg2Rad * -90.f)
				* _playerTransform->GetWorldMatrix();

			transform->SetWorldEulerAngle(CE_MATH::CalculateEuler(offsetMatrix));
			transform->SetWorldPosition(offsetMatrix.r[3]);
		}
		/* 꽂은 상태 */
		else if (!_playerAlign && !_playerTransform)
		{
			XMMATRIX offsetmatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
				* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(0)
					, CE_MATH::ToRadian(-90)
					, CE_MATH::ToRadian(0)));
			gameObject->GetComponent<ModelRenderer>()->SetOffsetMatrix(offsetmatrix);

			switch (_sequence)
			{
			case 0:
				_defaultPosition = XMFLOAT4(-27.45f, -1.58f, 38.31f, 1);
				_animator->Play("FuseCrab.ao|Shed_Awakening_FuseSocket_JumpOut_FuseCrab1"
					, true
					, nullptr);
				break;
				
			case 1:
				_defaultPosition = XMFLOAT4(-24.82f, -1.58f, 39.07f, 1);
				_animator->Play("FuseCrab.ao|Shed_Awakening_FuseSocket_JumpOut_FuseCrab2"
					, true
					, nullptr);
				break;
			case 2:
				_defaultPosition = XMFLOAT4(-70.5f, 12.f, 23.8f, 1);
				_animator->Play("FuseCrab.ao|Shed_Awakening_FuseSocket_JumpOut_FuseCrab3"
					, true
					, nullptr);
				break;
			}

			transform->SetWorldEulerAngle(XMLoadFloat4(&_defaultRotation));
			transform->SetWorldPosition(XMLoadFloat4(&_defaultPosition));
			_animator->SetPlaySpeed(0.f);
			_fuselight->SetLight(true);
		}
	}

	
}

void FuseCrab::StartAnimation() noexcept
{

	_animator->SetPlaySpeed(1.f);

	if (_sequence != 2)
	{

		_audioSource->Play(_jumpoutaudio, 0.2f);

		_spriteanimaion->Play();
		if (_smokeparticle)
			_smokeparticle->Play();
	}
}

void FuseCrab::PickUp() noexcept
{	
	_animator->Play("FuseCrab.ao|FuseCrab_Carried"
					, true);
	_pickUp = true;
}

void FuseCrab::ClearTarget(void) noexcept
{
	_playerAlign = nullptr;
	_playerTransform = nullptr;
}

void FuseCrab::PlayInsertSound() noexcept
{
	_audioSource->Play(_insertaudio , 0.2f);
}

void FuseCrab::PlayLightSound() noexcept
{
	_audioSource->Play(_lightaudio , 0.2f);
}

void FuseCrab::OnIntroAnimationFinished()
{
	if ("FuseCrab2" == gameObject->GetName())
	{
		_animator->Play("FuseCrab.ao|Shed_Awakening_Slide_FuseSlide_FuseCrab"
			, false
			, std::bind(&FuseCrab::OnSlideAnimationFinished, this));

		transform->SetWorldPosition(XMVectorSet(3.22f, 25.97f, -57.8f, 1.f));
		XMStoreFloat4(&_defaultRotation, XMVectorSet(0.f, CE_MATH::Deg2Rad * 180.f, 0.f, 0.f));
		transform->ImmediateUpdate();
		_animator->SetPlaySpeed(0.f);
		_rigid->SetSimulation(true);

		GameObject::FindObjectByName("FuseCrabUI")->SetActive(true);
	}
	else
	{
		/* 뼈 위치 */
		XMMATRIX boneMatrix		= gameObject->GetComponent<ModelRenderer>()->GetModel()->GetHierarchyNode("Align")->GetCombinedTransform();
		XMMATRIX offsetMatrix	= XMMatrixScaling(0.01f, 0.01f, 0.01f)
									* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(0
																								, CE_MATH::ToRadian(-90.f)
																								, 0));
		XMMATRIX transformMatrix = transform->GetWorldMatrix();

		XMMATRIX mat = boneMatrix * offsetMatrix * transformMatrix;


		_animator->Play("FuseCrab.ao|FuseCrab_Dazed"
				, true);
		transform->SetWorldPosition(mat.r[3]);

		_rigid->SetSimulation(true);
		_spriteanimaion->StopImmediate();

		if (_smokeparticle)
			_smokeparticle->Stop();
	}
}

void FuseCrab::OnSlideAnimationFinished()
{
	XMMATRIX boneMatrix = gameObject->GetComponent<ModelRenderer>()->GetModel()->GetHierarchyNode("Align")->GetCombinedTransform();
	XMMATRIX offsetMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f)
		* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(0
			, CE_MATH::ToRadian(-90.f)
			, 0));
	XMMATRIX transformMatrix = transform->GetWorldMatrix();

	XMMATRIX mat = boneMatrix * offsetMatrix * transformMatrix;

	mat.r[3] = XMVectorSetY(mat.r[3], XMVectorGetY(mat.r[3]) + 1.3f);


	_animator->Play("FuseCrab.ao|FuseCrab_Dazed"
		, true);
	transform->SetWorldPosition(mat.r[3]);
	XMStoreFloat4(&_defaultRotation, XMVectorZero());
	_collider->SetEnable(true);
	_rigid->SetEnable(true);
}
