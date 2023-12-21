#pragma once

#include "Renderer.h"

#define PARTICLE_BUFFER_MAX 500

class ParticleSystemAnimation : public Renderer
{
public:		struct ParticleAnimation
			{
				Vector3		position;
				Vector4		color;
				Vector4		emissiveColor;
				float		size;
				float		velocity;
				float		deltaTime;
				bool		active;
				Vector3		direction;

				XMFLOAT2	uv;
				float		animationSpeed;

				float		startSize;
				float		endSize;
			};

public:		enum class PASS
			{
				ALPHABLENDING_MSAA,
				ALPHABLENDING,
				ONEBLEDING_MSAA,
				ONEBLEDING,
			};

private:	ParticleSystemAnimation(void) noexcept;
private:	virtual ~ParticleSystemAnimation(void) noexcept = default;

public:		void							InitXXX(void) noexcept override;
public:		void							UpdateXXX(float fElapsedTime) noexcept override;
public:		void							ReleaseXXX(void) noexcept override;

public:		void							OnEnable(void) noexcept override;
public:		void							OnDisable(void) noexcept override;

public:		void							Render(const Camera* const camera) const noexcept override;

private:	void							InitBuffer(void) noexcept;
private:	void							EmitParticles(float fElapsedTime) noexcept;
private:	void							KillParticles(void) noexcept;
private:	void							UpdateBuffers(float fElapsedTime) noexcept;
public:		void							Play(bool loop = true, float duration = 0) noexcept;
public:		void							Stop(void) noexcept;

private:	void							SortParticles(void) noexcept;

public:		void							SetStartColor(Vector4 color) noexcept { _colorStart = color; }
public:		void							SetEndColor(Vector4 color) noexcept { _colorEnd = color; }
public:		void							SetStartEmissiveColor(Vector4 color) noexcept { _emissiveColorStart = color; }
public:		void							SetEndEmissiveColor(Vector4 color) noexcept { _emissiveColorEnd = color; }
public:		void							SetVelocity(float velocity) noexcept { _particleVelocity = velocity; }
public:		void							SetVelocityVariation(float velocityVariation) noexcept { _particleVelocityVariation = velocityVariation; }
public:		void							SetStartSize(float size) noexcept { _sizeStart = size; }
public:		void							SetEndSize(float size) noexcept { _sizeEnd = size; }
public:		void							SetRange(Vector3 range) noexcept { _particleRange = range; }
public:		void							SetLifeTime(float time) noexcept { _lifeTime = time; }
public:		void							SetParticlePerSecond(float count) noexcept { _particlesPerSecond = count; }
public:		void							SetEmitTime(float time) noexcept { _accumulatedTime = time; }
public:		void							SetSizeLerpType(CETween::EaseType type) noexcept { _particleSizeType = type; }
public:		void							SetColorLerpType(CETween::EaseType type) noexcept { _particleColorType = type; }
public:		void							SetPass(PASS pass) noexcept { _pass = pass; }
public:		void							SetAmount(XMFLOAT2 amount) noexcept { _amount = amount; }

public:		static ParticleSystemAnimation* Create() noexcept;
public:		static void						Destroy(ParticleSystemAnimation* ptr) noexcept;


private:	Vector3						_particleRange;
private:	float						_particleVelocity = 0;
private:	float						_particleVelocityVariation = 0;
private:	float						_particlesPerSecond = 0;
private:	float						_lifeTime = 0.5f;
private:	int							_maxParticles = 0;
private:	int							_currentParticleCount = 0;
private:	float						_accumulatedTime = 0;
private:	ParticleAnimation*			_particleList = nullptr;
private:	uint*						_particleIndices = nullptr;
private:	float*						_particleDepths = nullptr;
private:	int							_vertexCount = 0;
private:	int							_indexCount = 0;
private:	VTXPARTICLEANIM*			_vertices = nullptr;
private:	ID3D11Buffer*				_vertexBuffer = nullptr;
private:	ID3D11Buffer*				_indexBuffer = nullptr;
private:	ID3D11Buffer*				_instanceBuffers = nullptr;
private:	ID3D11DeviceContext*		_deviceContext = nullptr;
private:	Vector4						_colorStart;
private:	Vector4						_colorEnd;
private:	Vector4						_emissiveColorStart;
private:	Vector4						_emissiveColorEnd;
private:	bool						_play = false;
private:	bool						_loop = true;
private:	float						_deltaTime = 0;
private:	float						_duration = 0;
private:	float						_sizeStart = 0;
private:	float						_sizeEnd = 0;
private:	float						_coneAngle = 0;
private:	float						_coneHeight = 1;
private:	D3D11_BUFFER_DESC			_vbInstDesc;
private:	D3D11_SUBRESOURCE_DATA		_vbInstSubresourceData;
private:	uint32						_modelStride = 0;
private:	CETween::EaseType			_particleSizeType;
private:	CETween::EaseType			_particleColorType;
private:	VTXMATRIXCOLOR*				_instancingDatas;

private:	PASS						_pass;
private:	XMFLOAT2					_amount;
};

