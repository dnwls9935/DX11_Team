#include "pch.h"
#include "AnimateSpriteTrail.h"
#include "Transform.h"

AnimateSpriteTrail::AnimateSpriteTrail( SpriteAnimation*	animation
										, const uint32		poolSize
										, const float		emitTime
										, const bool		isPreLoad) noexcept
	: _poolSize(poolSize)
	, _emitTime(emitTime)
	, _index(0)
	, _deltaTime(0)
	, _playing(false)
{
	_renderers.reserve(poolSize);
	_animators.reserve(poolSize);
	_objs.reserve(poolSize);

	for (int i = 0; i < poolSize; ++i)
	{
		if (isPreLoad)
			_objs.emplace_back(GameObject::Instantiate());
		else
			_objs.emplace_back(GameObject::Instantiate());

		_renderers.emplace_back(SpriteRenderer::Create(true));

		_renderers[i]->SetTexture(animation->GetTexture().front());

		_animators.emplace_back(SpriteAnimator::Create(false));

		SpriteAnimation* ani = new SpriteAnimation(*animation);
		ani->AddEvent(ani->GetFrameCount() - 1, std::bind(&AnimateSpriteTrail::OnEndAniXXX, this, i));

		_animators[i]->InsertAnimation("idle", ani);
		_animators[i]->Play("idle");

		_renderers[i]->SetPassType(SpriteRenderer::RenderOption::EFFECT);
		_objs[i]->AddComponent(_renderers[i]);
		_objs[i]->AddComponent(_animators[i]);

		_objs[i]->SetLayer(GameObjectLayer::TRANSLUCENT);

		_objs[i]->SetActive(false);
	}
}

void AnimateSpriteTrail::Start(void) noexcept
{

}

void AnimateSpriteTrail::FixedUpdate(float fElapsedTime) noexcept
{

}

void AnimateSpriteTrail::Update(float fElapsedTime) noexcept
{
	if (_playing == false)
		return;

	_deltaTime += fElapsedTime;

	if (_deltaTime >= _emitTime)
	{
		_deltaTime = 0;
		_objs[_index]->SetActive(true);
		_animators[_index]->Play("idle");
		_objs[_index]->GetTransform()->SetWorldPosition(transform->GetWorldPosition());
		_objs[_index]->GetTransform()->ImmediateUpdate();

		_index++;

		if (_index >= _poolSize)
			_index = 0;
	}
}

void AnimateSpriteTrail::LateUpdate(float fElapsedTime) noexcept
{
	for (int i = 0; i < _poolSize; ++i)
	{
		if (_objs[i]->GetActive())
		{
			float t =  _animators[i]->GetAnimationByKey("idle")->GetProgress();

			float a = CETween::Lerp(1, 0, t, CETween::EaseType::easeLiner);
			float r = CETween::Lerp(0.5f, 0.4f, std::min(t * 2.f, 1.f), CETween::EaseType::easeOutQuart);
			float g = CETween::Lerp(0.4f, 0.4f, std::min(t * 2.f, 1.f), CETween::EaseType::easeOutQuart);
			float b = CETween::Lerp(0.4f, 0.5f, std::min(t * 2.f, 1.f), CETween::EaseType::easeOutQuart);

			_renderers[i]->GetMaterial()->SetColor(ShaderSemantic::Type::COLOR_DIFFUSE, Vector4(r, g, b, a));	
		}
	}
}

void AnimateSpriteTrail::OnEnable(void) noexcept
{
	_index		= 0;
	_deltaTime	= 0;

	for (int i = 0; i < _poolSize; ++i)
	{
		_objs[i]->SetActive(true);
	}
}

void AnimateSpriteTrail::OnDisable(void) noexcept
{
	_index		= 0;
	_deltaTime	= 0;

	for (int i = 0; i < _poolSize; ++i)
	{
		_objs[i]->SetActive(false);
	}
}

void AnimateSpriteTrail::OnEndAniXXX(const uint32 index) noexcept
{
	_objs[index]->SetActive(false);
}

void AnimateSpriteTrail::Play(void) noexcept
{
	_playing	= true;
	_index		= 0;
	_deltaTime	= 0;
}

void AnimateSpriteTrail::Stop(void) noexcept
{
	_playing	= false;
	_index		= 0;
	_deltaTime	= 0;
}

void AnimateSpriteTrail::StopImmediate(void) noexcept
{
	_playing	= false;
	_index		= 0;
	_deltaTime	= 0;

	for (int i = 0; i < _poolSize; ++i)
	{
		_objs[i]->SetActive(false);
	}
}
