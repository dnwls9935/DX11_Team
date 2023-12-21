#include "pch.h"
#include "SpriteHP.h"
#include "GameObject.h"
#include "SpriteRenderer.h"
#include "Transform.h"
#include "RectTransform.h"
#include "Actor.h"

SpriteHP::SpriteHP(Actor* actor, Vector4 color, bool isInstantiate) noexcept
	: _actor(actor)
	, _color(color)
	, _isInstantiate(isInstantiate)
{
}

void SpriteHP::Start(void) noexcept
{
	_hp = _actor->GetHP();
	if (_isInstantiate)
		_hpObj = GameObject::Instantiate("Hp Bar Background");
	else
		_hpObj = GameObject::Instantiate("Hp Bar Background");

	_hpTransform = _hpObj->GetTransform();
	_hpTransform->SetParent(transform);

	SpriteRenderer* hpBarBack = SpriteRenderer::Create(true);
	hpBarBack->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Dungeon\\EnemyHealth_Background.dds"));
	hpBarBack->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(0, 0, 0, 1));
	hpBarBack->SetPivotX(0.5f);
	hpBarBack->SetPivotY(1.f);
	hpBarBack->SetSize(15, 3);
	hpBarBack->SetPassType(SpriteRenderer::RenderOption::UI);
	_hpObj->AddComponent(hpBarBack);

	//_hpBarPos = (XMVector3Normalize(transform->GetUp()) * 2);
	_hpTransform->SetLocalPosition(0, 10, 0);
	_hpObj->SetSortOrder(10);
	_hpObj->SetLayer(GameObjectLayer::TRANSLUCENT);

	/* Hp Bar */
	GameObject* obj = GameObject::Instantiate("Hp Bar");

	obj->GetTransform()->SetParent(_hpTransform);
	_hpBar = SpriteRenderer::Create(true);
	_hpBar->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Dungeon\\EnemyHealth.dds"));
	_hpBar->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, _color);
	_hpBar->SetPivotX(0.5f);
	_hpBar->SetPivotY(1.f);
	_hpBar->SetSize(15, 3);
	_hpBar->SetFillType(SpriteRenderer::FillType::HORIZONTAL_INV);
	_hpBar->SetPassType(SpriteRenderer::RenderOption::UI);
	obj->AddComponent(_hpBar);

	obj->GetTransform()->SetLocalPosition(0, 0, 0);
	obj->SetSortOrder(12);
	obj->SetLayer(GameObjectLayer::TRANSLUCENT);

	/* Hp Bar White Base */
	obj = GameObject::Instantiate("Hp Bar White");

	obj->GetTransform()->SetParent(_hpTransform);
	_hpBarBase = SpriteRenderer::Create(true);
	_hpBarBase->SetTexture(ASSETMANAGER->GetTextureData("Asset\\UI\\Dungeon\\EnemyHealth.dds"));
	_hpBarBase->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(1, 1, 1, 1));
	_hpBarBase->SetPivotX(0.5f);
	_hpBarBase->SetPivotY(1.f);
	_hpBarBase->SetSize(15, 3);
	_hpBarBase->SetFillType(SpriteRenderer::FillType::HORIZONTAL_INV);
	_hpBarBase->SetPassType(SpriteRenderer::RenderOption::UI);
	obj->AddComponent(_hpBarBase);

	obj->GetTransform()->SetLocalPosition(0, 0, 0);
	obj->SetSortOrder(11);
	obj->SetLayer(GameObjectLayer::TRANSLUCENT);
}

void SpriteHP::Update(float fElapsedTime) noexcept
{
	_hpTransform->SetLocalPosition(0, 10, 0);
	_hpTransform->SetLocalEulerAngle(0, 0, 0);

	/* Set Hp Bar */
	float hp = _actor->GetHP();
	if (hp != _hp)
	{
		if (hp > _hp)
		{
			hp = std::min(_actor->GetMaxHP(), hp);

			float per = hp / _actor->GetMaxHP();
			_hpBar->SetFillAmount(per);
			_hpBarBase->SetFillAmount(per);
		}
		else
		{
			float max = _actor->GetMaxHP();
			_hpBar->SetFillAmount(hp / max);
			_hpBarBase->SetFillAmount(_hp / max);
			_time = 0;
		}
		_hp = hp;
	}

	if (_hpBar->GetFillAmount() != _hpBarBase->GetFillAmount())
	{
		_hpBarBase->SetFillAmount(CETween::Lerp(_hpBarBase->GetFillAmount(), _hpBar->GetFillAmount(), _time));
		_time += fElapsedTime;
	}
}

void SpriteHP::OnDestroy(void) noexcept
{
	std::vector<Transform*> childs = _hpTransform->GetChilds();
	for (int i = 0; i < childs.size(); ++i)
	{
		GameObject::Destroy(childs[i]->GetGameObject());
	}
}

void SpriteHP::Visible(void) noexcept
{
	if(_hpObj != nullptr)
		_hpObj->SetActive(true);
}

void SpriteHP::Invisible(void) noexcept
{
	if (_hpObj != nullptr)
		_hpObj->SetActive(false);
}