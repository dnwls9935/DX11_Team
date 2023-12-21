#pragma once

#include "Behaviour.h"

namespace ce
{
	class ModelRenderer;
	class ParticleSystem;
	class TrailRenderer;
	class Model;
}

class AcornBulletFX : public Behaviour
{
public:			AcornBulletFX(Transform* parent, Model* model) noexcept;
public:			virtual ~AcornBulletFX(void) noexcept;

public:			void	Start(void) noexcept override;
public:			void	Update(float fElapsedTime) noexcept override;

public:			void	DestroyFX(void) noexcept;
public:			void	StartFX(void) noexcept;

private:		void	CreateSmokeParticle();
private:		void	CreateTwinkleParticle();
private:		void	CreateTopTrail();
private:		void	CreateCenterTrail();
private:		void	CreateBottomTrail();
	  

private:		bool				_start;
private:		Transform*			_parent;
private:		Model*				_parentModel;

private:		GameObject*			_smokeParticleObject;
private:		Transform*			_smokeParticleTransform;
private:		ParticleSystem*		_smokeParticle;

private:		GameObject*			_twinkleParticleObject;
private:		Transform*			_twinkleParticleTransform;
private:		ParticleSystem*		_twinkleParticle;

private:		GameObject*			_topTrailObject;
private:		Transform*			_topTrailTransform;
private:		TrailRenderer*		_topTrail;

private:		GameObject*			_botTrailObject;
private:		Transform*			_botTrailTransform;
private:		TrailRenderer*		_botTrail;

private:		GameObject*			_centerTrailObject;
private:		Transform*			_centerTrailTransform;
private:		TrailRenderer*		_centerTrail;

};

