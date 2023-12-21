#pragma once
#include "Behaviour.h"

namespace ce
{
	class SpriteRenderer;
}
class Actor;

class SpriteHP : public Behaviour
{
public:		SpriteHP(Actor* actor, Vector4 color = Vector4(1, 1, 1, 1), bool isInstantiate = false) noexcept;
public:		virtual		~SpriteHP(void) noexcept = default;

public:		void		Start(void) noexcept override;
public:		void		Update(float fElapsedTime) noexcept override;

public:		void		OnDestroy(void) noexcept override;

public:		void		Visible(void) noexcept;
public:		void		Invisible(void) noexcept;

private:	Actor*		_actor;
private:	bool		_isInstantiate;

private:	Transform* _hpTransform;

private:	float		_hp;
private:	float		_time;

private:	Vector3		_hpBarPos;
private:	Vector4		_color;

private:	SpriteRenderer* _hpBar;
private:	SpriteRenderer* _hpBarBase;

private:	GameObject* _hpObj;
};

