#pragma once

#include "Behaviour.h"

namespace ce
{
	class Rigidbody;
	class Collider;
	class ModelRenderer;
	class ParticleSystem;
	class PathFollower;
}
class EffectPlayer;

class SuctionEffect : public Behaviour
{
public:		SuctionEffect(GameObject* targetObject0 = nullptr,
						  GameObject* targetObject1 = nullptr,
					      GameObject* targetObject2 = nullptr,
						  Vector3	  offsetVector1 = Vector3(0,0,0),
						  Vector3	  offsetVector2 = Vector3(0,0,0),
						  bool		  particleInv = false) noexcept;
public:		virtual ~SuctionEffect(void) noexcept;

public:		void Start(void) noexcept override;
public:		void FixedUpdate(float fElapsedTime) noexcept override;
public:		void Update(float fElapsedTime) noexcept override;
public:		void LateUpdate(float fElapsedTime) noexcept override;

public:		void SetOn(bool b) { _on = b; }
public:		bool GetOn() { return _on; }

public:		void SetSuction(bool b) noexcept { _suction = b; } ;
public:		bool GetSuction() noexcept { return _suction; };

public:		void SetNext(const std::string& nextSuction) { _nextSuction = nextSuction; };
public:		std::string GetNext() { return _nextSuction; }


private:	bool				 _on;
private:	bool				 _suction;
private:	GameObject*			 _targetObject0;
private:	GameObject*			 _targetObject1;
private:	GameObject*			 _targetObject2;

private:	ModelRenderer*		 _renderer;

private:	Rigidbody*			 _rigid;
private:	Collider*			 _collider;
private:	std::string			 _nextSuction;

private:	float				 _U;
private:	float				 _V;


private:	bool				_loadingCheck;

private:	AudioAsset*			_activate;
private:	AudioAsset*			_loop;
private:	AudioAsset*			_metalLoop;
private:	EffectPlayer*		_audioSource;
private:	bool				_soundcheck;

private:	Vector3				_offsetVector1;
private:	Vector3				_offsetVector2;

private:	ParticleSystem*		_volumeParticle;
private:	bool				_particleInv;

private:	bool				_particleSwitch;

};

