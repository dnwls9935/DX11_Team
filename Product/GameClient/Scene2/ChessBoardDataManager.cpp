#include "pch.h"
#include "ChessBoardDataManager.h"
#include "ChessBoardData.h"
#include "GameObject.h"
#include "Collider.h"
#include "Rigidbody.h"
#include "transform.h"



ChessBoardDataManager::ChessBoardDataManager(void) noexcept
	: _rightupper(12.256f, 0, 10.284f)
	, _rightunder(10.284f, 0, -12.256f)
	, _leftupper(-10.284f, 0, 12.256f)
	, _leftunder(-12.256f, 0, -10.284f)
{
}

void ChessBoardDataManager::Start(void) noexcept
{
	_chessboard.reserve(NUMCHESSBOARD);
	XMVECTOR	start = XMVectorSet(-14.f, 0, 14.f, 1);
	XMVECTOR	pos{};
	CHESSBOARD	chessboard{};
	std::string chessname{};
	ChessBoardData* chessBoarddata{};

	for (int i = 0; i < 8; ++i)
	{
		for (int j = 0; j < 8; ++j)
		{
			/* ü�� ���� ���� �̸� */
			int index = i * 8 + j;
			chessname = "Chess" + std::to_string(index);

			chessboard._boarddata = GameObject::Instantiate(chessname.c_str());
			chessboard._boarddata->SetLayer(GameObjectLayer::OBJECT);
			/* ��ġ�� �ٸ� Summon���� ����ִ��� Bool Check*/
			chessboard._chessboardIn = false;

			/* üũ ���� ���� ��ü */
			chessBoarddata = new ChessBoardData(index);
			chessboard._boarddata->AddComponent(chessBoarddata);

			/* ��ġ */
			pos = start + XMVectorSet(j * 4.f, 0, -(i * 4.f), 0);
			pos = XMVector3TransformNormal(pos, XMMatrixRotationY(CE_MATH::ToRadian(-40)));
			chessboard._boarddata->GetTransform()->SetLocalPosition(pos);


			_chessboard.emplace_back(chessboard);
		}
	}
}

void ChessBoardDataManager::Update(float fElapsedTime) noexcept
{
	return;
}

DirectX::FXMVECTOR ChessBoardDataManager::GetPosVec(const uint index) const noexcept
{
	if (index > 63)
	{
		CE_ASSERT("rldnr", "Vector Index �������� �Ѿ..");
		return XMVectorZero();
	}

	return _chessboard[index]._boarddata->GetTransform()->GetWorldPosition();
}



ChessBoardData* ChessBoardDataManager::GetBoardData(const uint index) const noexcept
{
	if (index > 63)
	{
		CE_ASSERT("rldnr", "Vector Index �������� �Ѿ..");
		return nullptr;
	}

	return _chessboard[index]._boarddata->GetComponent<ChessBoardData>();
}

const bool ChessBoardDataManager::GetCheckIn(const uint index) const noexcept
{
	if (index > 63)
	{
		CE_ASSERT("rldnr", "Vector Index �������� �Ѿ..");
		return true;
	}

	return _chessboard[index]._chessboardIn;
}

void ChessBoardDataManager::SetCheckIn(const uint index, bool set) noexcept
{
	if (index > 63)
	{
		CE_ASSERT("rldnr", "Vector Index �������� �Ѿ..");
		return;
	}

	_chessboard[index]._chessboardIn = set; 
}

int ChessBoardDataManager::CalDirection(uint index, edirection edir) noexcept
{
	switch (edir)
	{
	case edirection::BACK:
		if (index - 8 < 0)
		{
			return -1;
		}
		else 
		{
			return index - 8;
		}
		break;
	case edirection::FRONT:
		if (index + 8 > NUMCHESSBOARD - 1)
		{
			return -1;
		}
		else
		{
			return index + 8;
		}

		break;
	case edirection::LEFT:
		if ( ( ( (index - 1) / 7 )  < (index / 7) ) )
		{
			return -1;
		}
		else
		{
			return index - 1;
		}

		break;
	case edirection::RIGHT:
		if ( ( ( (index + 1) / 7) > (index / 7 ) ) ) 
		{
			return -1;
		}
		else
		{
			return index + 1;
		}
		break;
	default:
		return -1;
		break;
	}
}

bool ChessBoardDataManager::isOut(FXMVECTOR pos) noexcept
{
	/* �ȿ� �ִ�. */
	if (XMVectorGetX(XMVector3Dot(pos - XMLoadFloat3(&_leftunder), XMVector3Normalize(XMLoadFloat3(&_rightupper) - XMLoadFloat3(&_leftunder)))) > 0 &&
		XMVectorGetX(XMVector3Dot(pos - XMLoadFloat3(&_leftupper), XMVector3Normalize(XMLoadFloat3(&_rightunder) - XMLoadFloat3(&_leftupper)))) > 0 &&
		XMVectorGetX(XMVector3Dot(pos - XMLoadFloat3(&_rightupper), XMVector3Normalize(XMLoadFloat3(&_leftunder) - XMLoadFloat3(&_rightupper)))) > 0 &&
		XMVectorGetX(XMVector3Dot(pos - XMLoadFloat3(&_rightunder), XMVector3Normalize(XMLoadFloat3(&_leftupper) - XMLoadFloat3(&_rightunder)))) > 0)
	{
		return false;
	}
	/* �ۿ� �ִ�. */
	else
	{
		return true;
	}
}

void ChessBoardDataManager::PlayFX(int currentIndex, cardinalPoint cardinalPoint)
{
	if (currentIndex > 63)
	{
		CE_ASSERT("dnwls9935", "Vector Index �������� �Ѿ..");
		return;
	}

	switch (cardinalPoint)
	{
	case ChessBoardDataManager::cardinalPoint::E:
		break;
	case ChessBoardDataManager::cardinalPoint::W:
		break;
	case ChessBoardDataManager::cardinalPoint::S:
		break;
	case ChessBoardDataManager::cardinalPoint::N:
		break;
	case ChessBoardDataManager::cardinalPoint::FOUR:
		break;
	case ChessBoardDataManager::cardinalPoint::EIGHT:
		EightPlayFX(currentIndex);
		break;
	default:
		break;
	}
}

void ChessBoardDataManager::PlayFX(int currentIndex)
{
	if (currentIndex > 63 && currentIndex < -1)
	{
		CE_ASSERT("rldnr", "Vector Index �������� �Ѿ..");
		return;
	}
	_chessboard[currentIndex]._boarddata->GetComponent<ChessBoardData>()->Play();
}

void ChessBoardDataManager::EightPlayFX(int currentIndex)
{
	int index = currentIndex;
	
	int lineX = currentIndex / 8;
	int lineY = currentIndex % 8;

	//���� - ���� ��ġ�� ���� �����ڸ��� �ƴҶ���
	if (lineY != 0)
	{
		index = currentIndex - 1;
		if (index < 63 && index > -1)
			_chessboard[index]._boarddata->GetComponent<ChessBoardData>()->Play();
	}

	//������ - ���� ��ġ�� ���� �����ڸ��� �ƴҶ���
	if (lineY != 7)
	{
		index = currentIndex + 1;
		if (index < 63 && index > -1)
			_chessboard[index]._boarddata->GetComponent<ChessBoardData>()->Play();
	}

	//���� - ���� ��ġ�� �ֻ���� �ƴҶ���
	if (lineX != 0)
	{
		index = currentIndex - 8;
		if (index < 63 && index > -1)
			_chessboard[index]._boarddata->GetComponent<ChessBoardData>()->Play();
	}

	//�Ʒ��� - ���� ��ġ�� ���ϴ��� �ƴҶ���
	if (lineX != 7)
	{
		index = currentIndex + 8;
		if (index < 63 && index > -1)
			_chessboard[index]._boarddata->GetComponent<ChessBoardData>()->Play();
	}

	//���� �� �밢�� - �ֻ��, ���� �����ڸ� X
	if (lineX != 0 && lineY != 0)
	{
		index = currentIndex - 9;
		if (index < 63 && index > -1)
			_chessboard[index]._boarddata->GetComponent<ChessBoardData>()->Play();
	}

	//������ �Ʒ� �밢�� -���ϴ�, ���� �����ڸ� X
	if (lineX != 7 && lineY != 7)
	{
		index = currentIndex + 9;
		if (index < 63 && index > -1)
			_chessboard[index]._boarddata->GetComponent<ChessBoardData>()->Play();
	}

	//������ �� �밢�� - �ֻ��, ������ �����ڸ� X
	if (lineX != 0 && lineY != 7)
	{
		index = currentIndex - 7;
		if (index < 63 && index > -1)
			_chessboard[index]._boarddata->GetComponent<ChessBoardData>()->Play();
	}

	//���� �Ʒ� �밢�� - ���ϴ�, ������ �����ڸ�X
	if (lineX != 7 && lineY != 0)
	{
		index = currentIndex + 7;
		if (index < 63 && index > -1)
			_chessboard[index]._boarddata->GetComponent<ChessBoardData>()->Play();
	}
}
