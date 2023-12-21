#include "pch.h"
#include "Damaged.h"
#include "GameObject.h"
#include "Transform.h"
#include "Image.h"
#include "RectTransform.h"
#include "Camera.h"

Damaged::Damaged(Transform* monster, float damage, int8 attacker) noexcept
	: _monster(monster)
	, _damage(damage)
	, _width(0)
	, _attacker(attacker)
{
}

void Damaged::Start(void) noexcept
{
	_scale = 2;

	float x = ((float)rand() / (float)RAND_MAX) - ((float)rand() / (float)RAND_MAX);
	float y = ((float)rand() / RAND_MAX) - ((float)rand() / RAND_MAX);
	_random = float2(x, y);

	int num = (_damage * 10);
	std::string number = std::to_string(num);
	_count = number.length();

	for (int i = 0; i < number.length(); i++)
	{
		GameObject* obj = GameObject::Instantiate("Damaged num");
		obj->GetTransform()->SetParent(transform);

	//	obj->GetTransform()->SetLocalScale(2, 2, 0);

		UI::Image* img = UI::Image::Create();

		//이미지 셋팅
		char key[256];
		if(_attacker == 1)
			sprintf_s(key, 256, "%s%c%s", "Asset\\Texture\\Font\\blue\\", number.at(i), ".dds");
		else if(_attacker == 2)
			sprintf_s(key, 256, "%s%c%s", "Asset\\Texture\\Font\\green\\", number.at(i), ".dds");
		else
		sprintf_s(key, 256, "%s%c%s", "Asset\\Texture\\Font\\", number.at(i), ".dds");

		img->SetTexture(ASSETMANAGER->GetTextureData(key));
		img->SetColor(Vector4(1, 1, 1, 1));
		obj->AddComponent(img);

		_vecImg.push_back(img);

		obj->SetSortOrder(12);
		obj->GetTransform()->SetLocalPosition(_width, 0, 0);

		float width = img->GetWidth();
		_width += width;
	}

	_camera = Camera::GetMainCamera();
}

void Damaged::Update(float fElapsedTime) noexcept
{
	if (_camera == nullptr)
		return;

	/* Set Position */
	XMMATRIX viewProjMat = _camera->GetViewMatrix() * _camera->GetProjMatrix();

	XMVECTOR offset = XMVectorSet(0, 1 + _random.y + _deltaTime, 0, 0) + XMVector3Normalize(_monster->GetRight()) * _random.x;
	Vector3	pos = _monster->GetWorldMatrix().r[3] + offset;
	Vector3 worldPos = XMVector3TransformCoord((XMVECTOR)pos, viewProjMat);
	worldPos.x = (((worldPos.x + 1.f) * 0.5f) * WINCX) - (_width * 0.5f * _scale);
	worldPos.y = ((2.f - worldPos.y + 1.f) * 0.5f - 1.f) * WINCY;
	worldPos.z = 0;

	transform->SetWorldPosition((XMVECTOR)worldPos);

	/**/
	_deltaTime += fElapsedTime;
	if (_deltaTime < 0.1f)
	{
		_scale = 3 - CETween::Lerp(1, 2, _deltaTime * 10);
		for (int i = 0; i < _count; ++i)
		{
			_vecImg[i]->GetGameObject()->GetTransform()->SetLocalScale(_scale, _scale, 0);
		//	_vecImg[i]->GetGameObject()->GetTransform()->SetLocalPosition(_scale*i, 0, 0);
		}
	}
	else if (_deltaTime > 0.3f && _deltaTime < 1.2f)
	{
		float t = _deltaTime - 0.3f;
		float alpha = 1 - CETween::Lerp(0, 1, t * 5);
		if (alpha < 0)
			alpha = 0;

		for (int i = 0; i < _count; ++i)
		{
			_vecImg[i]->SetColor(Vector4(1, 1, 1, alpha));
		}
	}
	else if (_deltaTime > 1.2f)
	{
		End();
	}

}

void Damaged::End(void) noexcept
{
	std::vector < Transform* > childs = transform->GetChilds();
	for (int i = 0; i < childs.size(); ++i)
	{
		GameObject::Destroy(childs[i]->GetGameObject());
	}

	gameObject->SetDestroy();
}

