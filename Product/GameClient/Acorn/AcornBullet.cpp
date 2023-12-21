#include "pch.h"
#include "AcornBullet.h"
#include "Transform.h"
#include "GameObject.h"
#include "ParticleSystem.h"
#include "Light.h"
#include "ModelRenderer.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "ShaderManager.h"
#include "AcornBulletFX.h"
#include "BulletFlare.h"
#include "MonsterActor.h"
#include "Burnable.h"

AcornBullet::AcornBullet(const FXMVECTOR		direction
						, const float			speed
						, const float			damage
						, const GameObjectTag	enemyTag) noexcept
	: Projectile(direction, speed, enemyTag, damage, 5, false)
	, _direction()
	, _speed(speed)
	, _damage(1)
	, _enemyTag(enemyTag)
{
	XMStoreFloat3(&_direction, direction);
}

void AcornBullet::Awake(void) noexcept
{
}

void AcornBullet::Start(void) noexcept
{
	__super::Start();
	
	_renderer = ModelRenderer::Create(false);
	_renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Basic\\RingMesh03.fbx"));
	gameObject->AddComponent(_renderer);
	/*XMMATRIX mat = XMMatrixScaling(0.00125f, 0.00125f, 0.0025f)
					* XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(0)
					, CE_MATH::ToRadian(0)
					, CE_MATH::ToRadian(-90)));*/
	/*XMMATRIX mat = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationX(XMConvertToRadians(90.f));
	_renderer->SetOffsetMatrix(mat);*/
	_renderer->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE, ASSETMANAGER->GetTextureData("Asset\\Texture\\UniqueProjectile\\UniqueProjectile_01.dds"));
	_renderer->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1.f, 0.419f, 0.101f, 1.f) * 3.f);
	_renderer->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_EMISSIVE, ASSETMANAGER->GetTextureData("Asset\\Texture\\UniqueProjectile\\UniqueProjectile_01.dds"));
	_renderer->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_EMISSIVE, Vector4(1.f, 0.419f, 0.101f, 1.f));


	Light* light = Light::CreatePoint();
	light->SetRange(25.f);
	light->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.95f, 0.45f, 0.3f, 1));
	gameObject->AddComponent(light);

	//transform->SetLocalScale(0.2f, 0.2f, 0.5f);

	GameObject* subProjectileObject = GameObject::Instantiate("AcornBulletProjectileObject");
	_subTransform = subProjectileObject->GetTransform();

	_subRenderer = ModelRenderer::Create(false);
	_subRenderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Basic\\RingMesh03.fbx"));
	subProjectileObject->AddComponent(_subRenderer);
	//_subRenderer->SetOffsetMatrix(mat);

	_subRenderer->GetMaterial().SetTexture(ShaderSemantic::Type::TEXTURE_DIFFUSE, ASSETMANAGER->GetTextureData("Asset\\Texture\\UniqueProjectile\\UniqueProjectile_01.dds"));
	_subRenderer->GetMaterial().SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.176f, 0.078f, 0.023f, 1));
	_subRenderer->GetMaterial().SetShader(ShaderManager::This().GetShader(Shader::Type::MODEL_FORWARD));

	_subTransform->SetParent(gameObject->GetTransform());
	_subTransform->SetLocalPosition(0, 0, -0.4f);
	_subTransform->SetLocalScale(1.5f, 1.5f, 1.5f);
	subProjectileObject->SetLayer(GameObjectLayer::TRANSLUCENT);

	GameObject* fxObject = GameObject::Instantiate("AcornBulletFX");
	fxObject->GetTransform()->SetParent(gameObject->GetTransform());
	fxObject->AddComponent(_fx = new AcornBulletFX(gameObject->GetTransform(), _renderer->GetModel()));
	_fx->StartFX();


	Rigidbody* rigidbody = Rigidbody::Create(false);
	rigidbody->SetUseGravity(false);
	gameObject->AddComponent(rigidbody);

	Collider* collider = Collider::CreateSphereCollider(1);
	collider->SetIsTrigger(true);
	collider->SetQueryEnable(false);
	gameObject->AddComponent(collider);

	GameObject* explosionObject = GameObject::Instantiate("BulletExplosionFX");
	explosionObject->GetTransform()->SetParent(transform);
	explosionObject->AddComponent(_explosionFX = new BulletFlare());


}

void AcornBullet::FixedUpdate(float fElapsedTime) noexcept
{
}

void AcornBullet::Update(float fElapsedTime) noexcept
{
	__super::Update(fElapsedTime);
	transform->Rotate(0, 0, fElapsedTime * 10.f, true);
}

void AcornBullet::LateUpdate(float fElapsedTime) noexcept
{
}

void AcornBullet::OnEnable(void) noexcept
{
}

void AcornBullet::OnDisable(void) noexcept
{
}

void AcornBullet::OnDestroy(void) noexcept
{
	Transform::TList& childs = const_cast<Transform::TList&>(transform->GetChilds());

	for (int i = 0; i < childs.size(); ++i)
		GameObject::Destroy(childs[i]->GetGameObject());
}

void AcornBullet::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
	GameObject* obj = other->GetGameObject();

	bool startFX = false;

	if (_enemyTag == GameObjectTag::PLAYER)
	{
		if (obj->GetTag() == GameObjectTag::PLAYER)
		{
			obj->GetComponent<Actor>()->GetHit(_damage);
			startFX = true;
		}
		else if (GameObjectTag::WALL == obj->GetTag())
		{
			startFX = true;
		}

	}
	else if (_enemyTag == GameObjectTag::MONSTER)
	{
		if (obj->GetTag() == GameObjectTag::BURNABLE)
		{
			obj->GetComponent<Burnable>()->Burn();
			startFX = true;
		}
		else if (obj->GetTag() == GameObjectTag::MONSTER)
		{
			obj->GetComponent<MonsterActor>()->GetHit(_damage);
			startFX = true;
		}
		else if (GameObjectTag::WALL == obj->GetTag())
		{
			startFX = true;
		}
	}


	if (true == startFX)
	{
		//_explosionFX->GetTransform()->SetParent(nullptr);
		_explosionFX->StartFX();
		GameObject::Destroy(gameObject);
	}

	//__super::OnTriggerEnter(mine, other);
}

void AcornBullet::OnTriggerExit(Collider* mine, Collider* other) noexcept
{
	//GameObject* obj = other->GetGameObject();

	////__super::OnTriggerExit(mine, other);
	//GameObject::Destroy(_subTransform->GetGameObject());
	//_fx->DestroyFX();
	//GameObject::Destroy(_fx->GetGameObject());
	//GameObject::Destroy(gameObject);
}
