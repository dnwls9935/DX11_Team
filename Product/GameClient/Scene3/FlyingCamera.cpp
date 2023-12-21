#include "pch.h"
#include "FlyingCamera.h"
#include "GameObject.h"
#include "Transform.h"
#include "Camera.h"
#include "PlayerScreen.h"

FlyingCamera::FlyingCamera(Transform* player) noexcept
	: _player(player)
{
	_playerOffset = Vector3(0, 5, 0);
	_cameraOffset = Vector3(0, 0, -40.f);
	_boostOffset = Vector3(0, 0, -60.f);
}

void FlyingCamera::Start(void) noexcept
{ 
	_isLerpY = false;
	_isLerpX = false;
	transform->SetWorldEulerAngle(CE_MATH::ToRadian(10), 0, 0);

	_prevX = 0;

	//
	_camera = Camera::Create();
	_camera->SetTargetTexture(PlayerScreen::This().GetRenderTextureR());
	gameObject->AddComponent(_camera);

	_camera->RemovePostEffect(PostEffect::EffectType::BLOOM);
	_camera->RemovePostEffect(PostEffect::EffectType::DOF);
	_camera->RemovePostEffect(PostEffect::EffectType::BLUR);
	_camera->AddPostEffect(PostEffect::EffectType::HDR);
	_camera->AddPostEffect(PostEffect::EffectType::FOG);

	_camera->SetClearOption(Camera::ClearOption::SolidColor);
	_camera->SetClearColor(Vector4(1, 1, 1, 1));

	_camera->SetNear(1.f);

	gameObject->SetTag(GameObjectTag::CAMERA);
}

void FlyingCamera::Update(float fElapsedTime) noexcept
{
	XMVECTOR cameraOffset;
	if (_usingBoost)
	{
		if (_isBoostLerp)
		{
			_boostTime = std::min(1.f, _boostTime + fElapsedTime);

			if (!_endBoost)
				cameraOffset = (XMVECTOR)Vector3::Lerp(_cameraOffset, _boostOffset, _boostTime);
			else
				cameraOffset = (XMVECTOR)Vector3::Lerp(_boostOffset, _cameraOffset, _boostTime);

			if (_boostTime == 1.f)
			{
				_isBoostLerp = false;

				if (_endBoost)
					_usingBoost = false;
			}
		}
		else
		{
			cameraOffset = (XMVECTOR)_boostOffset;
		}

	}
	else
	{
		cameraOffset = (XMVECTOR)_cameraOffset;
	}

	XMVECTOR forward = XMVector3Normalize(transform->GetForward());

	float distance = XMVectorGetX(XMVector3Length(cameraOffset));

	XMVECTOR look = (forward * distance);

	XMVECTOR playerOffset = (XMVECTOR)_playerOffset;
	XMVECTOR playerPosition = (_player->GetWorldPosition() + playerOffset);

	XMVECTOR position = playerPosition - look;
	transform->SetWorldPosition(position);
	//

	XMVECTOR forwardNoY = XMVector3Normalize(XMVectorSetY(transform->GetForward(), 0));
	XMVECTOR playerForwardNoY = XMVector3Normalize(XMVectorSetY(_player->GetForward(), 0));

	const float dotY = XMVectorGetX(XMVector3Dot(playerForwardNoY, forwardNoY));
	float angleY = acosf(dotY);
	
	float curX = XMVectorGetX(_player->GetWorldEulerAngle());
	float angleX = curX - _prevX;
	_prevX = curX;
	/*====================================*/
	if (fabs(angleY) > CE_MATH::ToRadian(0.5f))
	{
		XMVECTOR cross = XMVector3Cross(forwardNoY, playerForwardNoY);
		if (XMVectorGetY(cross) < 0)
			angleY *= -1;

		_startAngleY = XMVectorGetY(transform->GetWorldEulerAngle());
		_lerpAngleY = angleY;
	}
	else
		_lerpAngleY = 0;

	if (fabs(angleX) > CE_MATH::ToRadian(0.5f))
	{
		_startAngleX = XMVectorGetX(transform->GetWorldEulerAngle());
		_lerpAngleX = angleX;
	}
	else 
		_lerpAngleX = 0;


	if (_lerpAngleY != 0)
	{

		float angle = CETween::Lerp(0, fabs(_lerpAngleY), 0.8f);

		if (_lerpAngleY < 0)
			angle *= -1;

		_y = _startAngleY + angle;

	}
	if (_lerpAngleX != 0)
	{
	
		float angle = CETween::Lerp(0, fabs(_lerpAngleX), 0.8f);// _timeX / _lerpTimeX);

		if (_lerpAngleX < 0)
			angle *= -1;

		_x = _startAngleX + angle;
	}

	transform->SetWorldEulerAngle(_x, _y, 0);
}

void FlyingCamera::SetBoost(bool use) noexcept
{
	if (use == _usingBoost)
		return;

	if (use)
	{
		_usingBoost = true;
		_endBoost = false;

		_isBoostLerp = true;
		_boostTime = 0;
	}
	else
	{
		_endBoost = true;

		_isBoostLerp = true;
		_boostTime = 0;
	}
}
