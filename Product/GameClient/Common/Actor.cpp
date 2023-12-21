#include "pch.h"
#include "Actor.h"

Actor::Actor(float maxHp) noexcept
	: _actorType(Type::END)
	, _state(State::IDLE)
	, _dirtyState(State::IDLE)
	, _inputHandler(nullptr)
	, _animator(nullptr)
	, _renderer(nullptr)
	, _camera(nullptr)
	, _rigidbody(nullptr)
	, _PickUpBone(nullptr)
	, _interactObj(nullptr)
	, _ValveInteractUseObject(nullptr)
	, _savePoint(nullptr)
	, _movementEffectPlayer(nullptr)
	, _voiceEffectPlayer(nullptr)
	, _isDirtyState(false)
	, _maxHp(maxHp)
	, _hp(_maxHp)
	, _isDead(false)
	, _audioasset{}
	, _lerpPosition{}
	, _lerpRotate(0.f)
{
}

void Actor::GetFollowCamera(FollowingCamera** camera)
{
	if (_camera)
	{
		*camera = _camera;
	}
}
