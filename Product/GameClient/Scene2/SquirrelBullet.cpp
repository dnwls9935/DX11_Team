#include "pch.h"
#include "SquirrelBullet.h"
#include "GameObject.h"
#include "Transform.h"
#include "Rigidbody.h"
#include "Collider.h"
#include "SpriteRenderer.h"
#include "ParticleSystem.h"
#include "Light.h"
#include "ModelRenderer.h"

SquirrelBullet::SquirrelBullet(Vector3 direction) noexcept
	: Projectile(direction,150, GameObjectTag::PLAYER, 5.f, 5.f, false)
	, _deltaTime(0)

{
}

void SquirrelBullet::Start(void) noexcept
{
	ModelRenderer* renderer = ModelRenderer::Create();
	renderer->SetModel(ASSETMANAGER->GetModelData("Asset\\Models\\Tree\\Bullet\\Bullet7.fbx"));
	gameObject->AddComponent(renderer);
	XMMATRIX mat = XMMatrixScaling(0.005f, 0.005f, 0.01f)
									*XMMatrixRotationQuaternion(XMQuaternionRotationRollPitchYaw(CE_MATH::ToRadian(0)
										, CE_MATH::ToRadian(0)
										, CE_MATH::ToRadian(-90)));
	renderer->SetOffsetMatrix(mat);

	Light* light = Light::CreatePoint();
	light->SetRange(25.f);
	light->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0.95f, 0.45f, 0.3f, 1));
	gameObject->AddComponent(light);

	__super::Start();
}

void SquirrelBullet::Update(float fElapsedTime) noexcept
{
	__super::Update(fElapsedTime);

	//float size = CETween::Lerp(0.2f, 1.f, std::min(1.f, _deltaTime));
	//_spriteRenderer->SetSize(size, size);
	//
}


void SquirrelBullet::OnTriggerEnter(Collider* mine, Collider* other) noexcept
{
}
