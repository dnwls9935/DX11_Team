#include "pch.h"
#include "ParticleSystemAnimation.h"
#include "GameObject.h"
#include "ShaderManager.h"
#include "Transform.h"

ParticleSystemAnimation::ParticleSystemAnimation(void) noexcept
    : Renderer(false)
{
    _particleRange.x = 0.1f;
    _particleRange.y = 0.1f;
    _particleRange.z = 0.1f;

	// 파티클의 속도와 속도 변화를 설정합니다.
	_particleVelocity = 1.0f;
	_particleVelocityVariation = 0.2f;

	// 파티클의 물리적 크기를 설정합니다.
	_sizeStart = 0.2f;
	_sizeEnd = 0.2f;

	// 초당 방출 할 파티클 수를 설정합니다.
	_particlesPerSecond = 250.0f / 1000.f;

	// 파티클 시스템에 허용되는 최대 파티클 수를 설정합니다.
	_maxParticles = 5000;

	// 파티클 리스트를 생성합니다.
	_particleList = new ParticleAnimation[_maxParticles];
	_particleIndices = new uint[_maxParticles];
	_particleDepths = new float[_maxParticles];

	// 파티클 리스트를 초기화 합니다.
	for (int i = 0; i < _maxParticles; i++)
	{
		_particleList[i].active = false;
		_particleList[i].uv = XMFLOAT2(0, 0);
		_particleList[i].animationSpeed = (float)rand() / RAND_MAX + CE_MATH::Random(100, 150);
		_particleList[i].startSize = 2;
		_particleList[i].endSize = (float)rand() / RAND_MAX + CE_MATH::Random(4, 10);
	}

	// 아직 배출되지 않으므로 현재 파티클 수를 0으로 초기화합니다.
	_currentParticleCount = 0;

	// 초당 파티클 방출 속도의 초기 누적 시간을 지웁니다.
	_accumulatedTime = 0.0f;
	_deviceContext = D3D11Device::Instance()->GetContextNativePTR();

	InitBuffer();

	_particleColorType = CETween::EaseType::easeLiner;
	_particleSizeType = CETween::EaseType::easeLiner;

	_pass = PASS::ALPHABLENDING_MSAA;

	_amount = XMFLOAT2(0, 0);
}

void ParticleSystemAnimation::InitXXX(void) noexcept
{
	_materials[0].SetShader(ShaderManager::This().GetShader(Shader::Type::PARTICLE_ANIMATION));
	_transform = _owner->GetTransform();
}

void ParticleSystemAnimation::UpdateXXX(float fElapsedTime) noexcept
{
	if (_owner == nullptr)
		return;
	if (_isOwnerActive == false)
		return;
	if (_bEnable == false)
		return;
	if (_play == false)
		return;

	KillParticles();

	_deltaTime += fElapsedTime;

	if (_deltaTime > _duration && _loop == false)
	{
		Stop();
		_deltaTime = 0;
		return;
	}

	EmitParticles(fElapsedTime);

	for (int i = 0; i < _currentParticleCount; i++)
	{
		_particleList[i].deltaTime += fElapsedTime;
		_particleList[i].position = _particleList[i].position + ((_particleList[i].direction) * _particleList[i].velocity * fElapsedTime);
		
		_particleList[i].uv.x += fElapsedTime * _particleList[i].animationSpeed;

		if (_particleList[i].uv.x >= _amount.x)
		{
			_particleList[i].uv.x = 0.f;
			_particleList[i].uv.y++;

			if (_particleList[i].uv.y >= _amount.y)
				_particleList[i].uv = XMFLOAT2(0, 0);
		}
	}

	UpdateBuffers(fElapsedTime);
}

void ParticleSystemAnimation::ReleaseXXX(void) noexcept
{
	// 인덱스 버퍼를 해제합니다.
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = nullptr;
	}

	// 버텍스 버퍼를 해제한다.
	if (_vertexBuffer)
	{
		_vertexBuffer->Release();
		_vertexBuffer = nullptr;
	}

	if (_instanceBuffers)
	{
		_instanceBuffers->Release();
		_instanceBuffers = nullptr;
	}

	if (_instancingDatas)
	{
		delete[] _instancingDatas;
		_instancingDatas = nullptr;
	}

	// 파티클 목록을 해제합니다.
	if (_particleList)
	{
		delete[] _particleList;
		_particleList = nullptr;
	}

	if (_particleIndices)
	{
		delete[] _particleIndices;
		_particleIndices = nullptr;
	}
	
	if (_particleDepths)
	{
		delete[] _particleDepths;
		_particleDepths = nullptr;
	}

	if (_vertices)
	{
		delete[] _vertices;
		_vertices = nullptr;
	}

	Destroy(this);
}

void ParticleSystemAnimation::OnEnable(void) noexcept
{
}

void ParticleSystemAnimation::OnDisable(void) noexcept
{
	for (int i = 0; i < _maxParticles; i++)
	{
		_particleList[i].active = false;
		_currentParticleCount = 0;
	}
}

void ParticleSystemAnimation::Render(const Camera* const camera) const noexcept
{
	if (_currentParticleCount == 0)
		return;
	if (_owner == nullptr)
		return;
	if (_isOwnerActive == false)
		return;
	if (_bEnable == false)
		return;

	unsigned int stride = sizeof(VTXPARTICLEANIM);
	unsigned int offset = 0;

	_deviceContext->IASetVertexBuffers(0, 1, &_vertexBuffer, &stride, &offset);
	_deviceContext->IASetIndexBuffer(_indexBuffer, DXGI_FORMAT_R16_UINT, 0);
	_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	_materials[0].Render((int)_pass
						, XMMatrixIdentity()//_transform->GetWorldMatrix()
						, camera);

	_deviceContext->DrawIndexed(_indexCount, 0, 0);
	_deviceContext->GSSetShader(nullptr, nullptr, 0);
}

void ParticleSystemAnimation::InitBuffer(void) noexcept
{
	ID3D11Device* device = D3D11Device::Instance()->GetDeviceNativePTR();
	// 정점 배열의 최대 정점 수를 설정합니다.
	_vertexCount = _maxParticles * 6;

	// 인덱스 배열의 최대 인덱스 수를 설정합니다.
	_indexCount = _vertexCount;

	// 렌더링 될 입자에 대한 정점 배열을 만듭니다.
	_vertices = new VTXPARTICLEANIM[_vertexCount];

	// 인덱스 배열을 만듭니다.
	uint16* indices = new uint16[_indexCount];

	// 처음에는 정점 배열을 0으로 초기화합니다.
	memset(_vertices, 0, (sizeof(VTXPARTICLEANIM) * _vertexCount));

	// 인덱스 배열을 초기화합니다.
	for (int i = 0; i < _indexCount; i++)
	{
		indices[i] = i;
	}

	// 동적 정점 버퍼의 설명을 설정한다.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VTXPARTICLEANIM) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource 구조에 정점 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = _vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// 이제 마침내 정점 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer)))
	{
		CE_ASSERT("ckswns", "Create Buffer에 실패했습니다");
	}

	// 정적 인덱스 버퍼의 설명을 설정합니다.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(uint16) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// 하위 리소스 구조에 인덱스 데이터에 대한 포인터를 제공합니다.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// 인덱스 버퍼를 만듭니다.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer)))
	{
		CE_ASSERT("ckswns", "Create Buffer에 실패했습니다");
	}

	// 더 이상 필요하지 않으므로 인덱스 배열을 해제합니다.
	delete[] indices;
	indices = 0;
}

void ParticleSystemAnimation::EmitParticles(float fElapsedTime) noexcept
{
	// 프레임 시간을 증가시킵니다.
	_accumulatedTime += fElapsedTime;

	// 새 파티클을 방출할 시간인지 확인합니다.
	while (_accumulatedTime > (_particlesPerSecond))
	{
		_accumulatedTime -= _particlesPerSecond;

		// 방출 할 파티클이 있으면 프레임 당 하나씩 방출합니다.
		if ((_currentParticleCount < (_maxParticles - 1)))
		{
			_currentParticleCount++;

			// 이제 임의 화 된 파티클 속성을 생성합니다.
			float positionX = (((float)rand() - (float)rand()) / RAND_MAX) * _particleRange.x;
			float positionY = (((float)rand() - (float)rand()) / RAND_MAX) * _particleRange.y;
			float positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * _particleRange.z;

			Vector3 dir = Vector3(0, 0, -1);
			float radiusX = _particleRange.x * 0.5f;
			float radiusZ = _particleRange.z * 0.5f;

			float velocity = _particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * _particleVelocityVariation;



			// 이제 블렌딩을 위해 파티클을 뒤에서 앞으로 렌더링해야하므로 파티클 배열을 정렬해야 합니다.
			// Z 깊이를 사용하여 정렬하므로 목록에서 파티클을 삽입해야 하는 위치를 찾아야합니다.
			int index = 0;
			bool found = false;
			while (!found)
			{
				if ((_particleList[index].active == false) || (_particleList[index].position.z < positionZ))
				{
					found = true;
				}
				else
				{
					index++;
				}
			}


			// 삽입 할 위치를 알았으므로 인덱스에서 한 위치 씩 배열을 복사하여 새 파티클을 위한 공간을 만들어야합니다.
			int i = _currentParticleCount;
			int j = i - 1;

			while (i != index)
			{
				_particleList[i].position = _particleList[j].position;
				_particleList[i].color = _particleList[j].color;
				_particleList[i].emissiveColor = _particleList[j].emissiveColor;
				_particleList[i].size = _particleList[j].size;
				_particleList[i].deltaTime = _particleList[j].deltaTime;
				_particleList[i].velocity = _particleList[j].velocity;
				_particleList[i].active = _particleList[j].active;
				_particleList[i].direction = _particleList[j].direction;
				_particleList[i].uv = _particleList[j].uv;
				_particleList[i].animationSpeed = _particleList[j].animationSpeed;
				_particleList[i].startSize = _particleList[j].startSize;
				_particleList[i].endSize = _particleList[j].endSize;
				i--;
				j--;
			}

			XMVECTOR posi = XMVectorSet(positionX, positionY, positionZ, 0);
			posi = XMVector3TransformCoord(posi, _transform->GetWorldMatrix());

			XMFLOAT3 position;
			XMStoreFloat3(&position, posi);

			// 이제 정확한 깊이 순서로 파티클 배열에 삽입하십시오.
			_particleList[index].position = Vector3(position.x, position.y, position.z);
			_particleList[index].deltaTime = 0;
			_particleList[index].size = _sizeStart;
			//_particleList[index].size.y			= _sizeStart;
			_particleList[index].color = _colorStart;
			_particleList[index].emissiveColor = _emissiveColorStart;
			_particleList[index].velocity = velocity;
			_particleList[index].active = true;
			_particleList[index].direction = dir;

			_particleList[index].uv = XMFLOAT2(0, 0);
			_particleList[index].animationSpeed = (float)rand() / RAND_MAX * CE_MATH::Random(100, 150);
			_particleList[index].startSize = 2;
			_particleList[index].endSize = (float)rand() / RAND_MAX + CE_MATH::Random(4, 10);;
		}
		else
			_accumulatedTime = 0;
	}
}

void ParticleSystemAnimation::KillParticles(void) noexcept
{
	for (int i = 0; i < _maxParticles; i++)
	{
		if ((_particleList[i].active == true) && (_particleList[i].deltaTime > _lifeTime))
		{
			_particleList[i].active = false;
			_currentParticleCount--;

			// 이제 모든 살아있는 파티클을 배열위로 이동시켜 파괴된 파티클을 지우고 배열을 올바르게 정렬된 상태로 유지합니다.
			for (int j = i; j < _maxParticles - 1; j++)
			{
				_particleList[j].position = _particleList[j + 1].position;
				_particleList[j].color = _particleList[j + 1].color;
				_particleList[j].emissiveColor = _particleList[j + 1].emissiveColor;
				_particleList[j].size = _particleList[j + 1].size;
				_particleList[j].deltaTime = _particleList[j + 1].deltaTime;
				_particleList[j].velocity = _particleList[j + 1].velocity;
				_particleList[j].active = _particleList[j + 1].active;
				_particleList[j].direction = _particleList[j + 1].direction;

				_particleList[j].uv = _particleList[j + 1].uv;
				_particleList[j].animationSpeed = _particleList[j + 1].animationSpeed;
				_particleList[j].startSize = _particleList[j + 1].startSize;
				_particleList[j].endSize = _particleList[j + 1].endSize;

			}
		}
	}
}

void ParticleSystemAnimation::UpdateBuffers(float fElapsedTime) noexcept
{
	SortParticles();

	// 처음에는 정점 배열을 0으로 초기화합니다.
	VTXPARTICLEANIM* verticesPtr;
	memset(_vertices, 0, (sizeof(VTXPARTICLEANIM) * _vertexCount));

	// 이제 파티클 목록 배열에서 정점 배열을 만듭니다. 각 파티클은 두 개의 삼각형으로 만들어진 쿼드입니다.
	int index = 0;

	for (int i = 0; i < _currentParticleCount; i++)
	{
		int particleIndex = _particleIndices[i];

		float deltaTime = _particleList[i].deltaTime / _lifeTime;

		_vertices[index].position = XMFLOAT3(_particleList[particleIndex].position.x
											, _particleList[particleIndex].position.y
											, _particleList[particleIndex].position.z);
		_vertices[index].size.x = CETween::Lerp(_particleList[particleIndex].startSize, _particleList[particleIndex].endSize, deltaTime, _particleSizeType);
		_vertices[index].size.y = CETween::Lerp(_particleList[particleIndex].startSize, _particleList[particleIndex].endSize, deltaTime, _particleSizeType);

		_vertices[index].color = XMFLOAT4(CETween::Lerp(_colorStart.x, _colorEnd.x, deltaTime, _particleColorType)
										, CETween::Lerp(_colorStart.y, _colorEnd.y, deltaTime, _particleColorType)
										, CETween::Lerp(_colorStart.z, _colorEnd.z, deltaTime, _particleColorType)
										, CETween::Lerp(_colorStart.w, _colorEnd.w, deltaTime, _particleColorType));

		_vertices[index].emissiveColor = XMFLOAT4(CETween::Lerp(_emissiveColorStart.x, _emissiveColorEnd.x, deltaTime, _particleColorType)
												, CETween::Lerp(_emissiveColorStart.y, _emissiveColorEnd.y, deltaTime, _particleColorType)
												, CETween::Lerp(_emissiveColorStart.z, _emissiveColorEnd.z, deltaTime, _particleColorType)
												, CETween::Lerp(_emissiveColorStart.w, _emissiveColorEnd.w, deltaTime, _particleColorType));

		_vertices[index].amount = _amount;
		_vertices[index].atlas = _particleList[particleIndex].uv;

		index++;
	}

	// 정점 버퍼를 잠급니다.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(_deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		CE_ASSERT("ckswns", "자원획득에 실패했습니다");
	}

	// 정점 버퍼의 데이터를 가리키는 포인터를 얻는다.
	verticesPtr = (VTXPARTICLEANIM*)mappedResource.pData;

	// 데이터를 정점 버퍼에 복사합니다.
	memcpy(verticesPtr, (void*)_vertices, (sizeof(VTXPARTICLEANIM) * _vertexCount));

	// 정점 버퍼의 잠금을 해제합니다.
	_deviceContext->Unmap(_vertexBuffer, 0);
}

void ParticleSystemAnimation::Play(bool loop, float duration) noexcept
{
	_loop = loop;
	_duration = duration;
	_play = true;
	_deltaTime = 0;

	for (int i = 0; i < _maxParticles; ++i)
	{
		_particleList[i].deltaTime = 0;
		_particleList[i].active = false;
	}

	_currentParticleCount = 0;
}

void ParticleSystemAnimation::Stop(void) noexcept
{
	_play = false;

	for (int i = 0; i < _maxParticles; i++)
	{
		_particleList[i].active = false;
		_currentParticleCount = 0;
	}
}

template <class T> void QuickDepthSort(T* indices, float* depths, int lo, int hi)
{
	//  lo is the lower index, hi is the upper index
	//  of the region of array a that is to be sorted
	int i = lo, j = hi;
	float h;
	T index;
	float x = depths[(lo + hi) / 2];

	//  partition
	do
	{
		while (depths[i] < x) i++;
		while (depths[j] > x) j--;
		if (i <= j)
		{
			h = depths[i];
			depths[i] = depths[j];
			depths[j] = h;

			index = indices[i];
			indices[i] = indices[j];
			indices[j] = index;
			i++; j--;
		}
	} while (i <= j);

	//  recursion
	if (lo < j) QuickDepthSort(indices, depths, lo, j);
	if (i < hi) QuickDepthSort(indices, depths, i, hi);
}

void ParticleSystemAnimation::SortParticles(void) noexcept
{
	XMVECTOR camearaWorldPosition = Camera::GetMainCamera()->GetWorldMatrix().r[3];
	for (int i = 0; i < _currentParticleCount; i++)
	{
		_particleIndices[i] = i;

		XMFLOAT3 p = (XMFLOAT3)_particleList[i].position;
		XMVECTOR position = XMLoadFloat3(&p);
		float depthDistance = XMVectorGetX(XMVector3LengthSq(camearaWorldPosition - position));
		_particleDepths[i] = depthDistance;
	}

	QuickDepthSort(_particleIndices, _particleDepths, 0, _currentParticleCount - 1);
}

ParticleSystemAnimation* ParticleSystemAnimation::Create() noexcept
{
    return new ParticleSystemAnimation();
}


void ParticleSystemAnimation::Destroy(ParticleSystemAnimation* ptr) noexcept
{
    delete ptr;
}
