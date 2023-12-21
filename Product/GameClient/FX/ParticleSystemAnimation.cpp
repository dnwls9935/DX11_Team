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

	// ��ƼŬ�� �ӵ��� �ӵ� ��ȭ�� �����մϴ�.
	_particleVelocity = 1.0f;
	_particleVelocityVariation = 0.2f;

	// ��ƼŬ�� ������ ũ�⸦ �����մϴ�.
	_sizeStart = 0.2f;
	_sizeEnd = 0.2f;

	// �ʴ� ���� �� ��ƼŬ ���� �����մϴ�.
	_particlesPerSecond = 250.0f / 1000.f;

	// ��ƼŬ �ý��ۿ� ���Ǵ� �ִ� ��ƼŬ ���� �����մϴ�.
	_maxParticles = 5000;

	// ��ƼŬ ����Ʈ�� �����մϴ�.
	_particleList = new ParticleAnimation[_maxParticles];
	_particleIndices = new uint[_maxParticles];
	_particleDepths = new float[_maxParticles];

	// ��ƼŬ ����Ʈ�� �ʱ�ȭ �մϴ�.
	for (int i = 0; i < _maxParticles; i++)
	{
		_particleList[i].active = false;
		_particleList[i].uv = XMFLOAT2(0, 0);
		_particleList[i].animationSpeed = (float)rand() / RAND_MAX + CE_MATH::Random(100, 150);
		_particleList[i].startSize = 2;
		_particleList[i].endSize = (float)rand() / RAND_MAX + CE_MATH::Random(4, 10);
	}

	// ���� ������� �����Ƿ� ���� ��ƼŬ ���� 0���� �ʱ�ȭ�մϴ�.
	_currentParticleCount = 0;

	// �ʴ� ��ƼŬ ���� �ӵ��� �ʱ� ���� �ð��� ����ϴ�.
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
	// �ε��� ���۸� �����մϴ�.
	if (_indexBuffer)
	{
		_indexBuffer->Release();
		_indexBuffer = nullptr;
	}

	// ���ؽ� ���۸� �����Ѵ�.
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

	// ��ƼŬ ����� �����մϴ�.
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
	// ���� �迭�� �ִ� ���� ���� �����մϴ�.
	_vertexCount = _maxParticles * 6;

	// �ε��� �迭�� �ִ� �ε��� ���� �����մϴ�.
	_indexCount = _vertexCount;

	// ������ �� ���ڿ� ���� ���� �迭�� ����ϴ�.
	_vertices = new VTXPARTICLEANIM[_vertexCount];

	// �ε��� �迭�� ����ϴ�.
	uint16* indices = new uint16[_indexCount];

	// ó������ ���� �迭�� 0���� �ʱ�ȭ�մϴ�.
	memset(_vertices, 0, (sizeof(VTXPARTICLEANIM) * _vertexCount));

	// �ε��� �迭�� �ʱ�ȭ�մϴ�.
	for (int i = 0; i < _indexCount; i++)
	{
		indices[i] = i;
	}

	// ���� ���� ������ ������ �����Ѵ�.
	D3D11_BUFFER_DESC vertexBufferDesc;
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VTXPARTICLEANIM) * _vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// subresource ������ ���� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA vertexData;
	vertexData.pSysMem = _vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// ���� ��ħ�� ���� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&vertexBufferDesc, &vertexData, &_vertexBuffer)))
	{
		CE_ASSERT("ckswns", "Create Buffer�� �����߽��ϴ�");
	}

	// ���� �ε��� ������ ������ �����մϴ�.
	D3D11_BUFFER_DESC indexBufferDesc;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(uint16) * _indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// ���� ���ҽ� ������ �ε��� �����Ϳ� ���� �����͸� �����մϴ�.
	D3D11_SUBRESOURCE_DATA indexData;
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// �ε��� ���۸� ����ϴ�.
	if (FAILED(device->CreateBuffer(&indexBufferDesc, &indexData, &_indexBuffer)))
	{
		CE_ASSERT("ckswns", "Create Buffer�� �����߽��ϴ�");
	}

	// �� �̻� �ʿ����� �����Ƿ� �ε��� �迭�� �����մϴ�.
	delete[] indices;
	indices = 0;
}

void ParticleSystemAnimation::EmitParticles(float fElapsedTime) noexcept
{
	// ������ �ð��� ������ŵ�ϴ�.
	_accumulatedTime += fElapsedTime;

	// �� ��ƼŬ�� ������ �ð����� Ȯ���մϴ�.
	while (_accumulatedTime > (_particlesPerSecond))
	{
		_accumulatedTime -= _particlesPerSecond;

		// ���� �� ��ƼŬ�� ������ ������ �� �ϳ��� �����մϴ�.
		if ((_currentParticleCount < (_maxParticles - 1)))
		{
			_currentParticleCount++;

			// ���� ���� ȭ �� ��ƼŬ �Ӽ��� �����մϴ�.
			float positionX = (((float)rand() - (float)rand()) / RAND_MAX) * _particleRange.x;
			float positionY = (((float)rand() - (float)rand()) / RAND_MAX) * _particleRange.y;
			float positionZ = (((float)rand() - (float)rand()) / RAND_MAX) * _particleRange.z;

			Vector3 dir = Vector3(0, 0, -1);
			float radiusX = _particleRange.x * 0.5f;
			float radiusZ = _particleRange.z * 0.5f;

			float velocity = _particleVelocity + (((float)rand() - (float)rand()) / RAND_MAX) * _particleVelocityVariation;



			// ���� ������ ���� ��ƼŬ�� �ڿ��� ������ �������ؾ��ϹǷ� ��ƼŬ �迭�� �����ؾ� �մϴ�.
			// Z ���̸� ����Ͽ� �����ϹǷ� ��Ͽ��� ��ƼŬ�� �����ؾ� �ϴ� ��ġ�� ã�ƾ��մϴ�.
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


			// ���� �� ��ġ�� �˾����Ƿ� �ε������� �� ��ġ �� �迭�� �����Ͽ� �� ��ƼŬ�� ���� ������ �������մϴ�.
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

			// ���� ��Ȯ�� ���� ������ ��ƼŬ �迭�� �����Ͻʽÿ�.
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

			// ���� ��� ����ִ� ��ƼŬ�� �迭���� �̵����� �ı��� ��ƼŬ�� ����� �迭�� �ùٸ��� ���ĵ� ���·� �����մϴ�.
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

	// ó������ ���� �迭�� 0���� �ʱ�ȭ�մϴ�.
	VTXPARTICLEANIM* verticesPtr;
	memset(_vertices, 0, (sizeof(VTXPARTICLEANIM) * _vertexCount));

	// ���� ��ƼŬ ��� �迭���� ���� �迭�� ����ϴ�. �� ��ƼŬ�� �� ���� �ﰢ������ ������� �����Դϴ�.
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

	// ���� ���۸� ��޴ϴ�.
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	if (FAILED(_deviceContext->Map(_vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource)))
	{
		CE_ASSERT("ckswns", "�ڿ�ȹ�濡 �����߽��ϴ�");
	}

	// ���� ������ �����͸� ����Ű�� �����͸� ��´�.
	verticesPtr = (VTXPARTICLEANIM*)mappedResource.pData;

	// �����͸� ���� ���ۿ� �����մϴ�.
	memcpy(verticesPtr, (void*)_vertices, (sizeof(VTXPARTICLEANIM) * _vertexCount));

	// ���� ������ ����� �����մϴ�.
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
