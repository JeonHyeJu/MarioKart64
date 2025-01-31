#include "PreCompile.h"
#include "GameData.h"

GameData* GameData::pInstance = nullptr;

GameData::GameData()
{
	Players.reserve(MAX_PLAYER_CNT);
}

GameData::~GameData()
{

}

void GameData::Release()
{
	if (pInstance != nullptr)
	{
		delete pInstance;
		pInstance = nullptr;
	}
}

GameData* GameData::GetInstance()
{
	if (pInstance == nullptr)
	{
		pInstance = new GameData;
	}

	return pInstance;
}

uint8_t GameData::GetPlayerIdx() const
{
	return PlayerIdx;
}

ECharacter GameData::GetPlayerCharacter() const
{
	return static_cast<ECharacter>(PlayerIdx);
}

uint8_t GameData::GetPlayerCnt() const
{
	return PlayerCnt;
}

void GameData::SetPlayerRank(uint8_t _val)
{
	PlayerRank = _val;
}

uint8_t GameData::GetPlayerRank() const
{
	return PlayerRank;
}

void GameData::SetPlayerIdx(uint8_t _idx)
{
	PlayerIdx = _idx;
}

void GameData::SetPlayers(const std::vector<SPlayerInfo>& _players)
{
	Players = _players;	// copy
}

const std::vector<SPlayerInfo>& GameData::GetPlayers() const
{
	return Players;
}

void GameData::GetHighRankPlayersIdx(int* _refArr, int _size)
{
	// Temp
	assert(_size == 6 && "High rank has 6 size only.");

	// Temp
	_refArr[0] = 1;
	_refArr[1] = 2;
	_refArr[2] = 3;
	_refArr[3] = 4;
	_refArr[4] = 5;
	_refArr[5] = 6;
}

void GameData::SetItem(uint8_t _playerIdx, EItemType _item)
{
	if (_playerIdx < Players.size())
	{
		Players[_playerIdx].Item = _item;
	}
}

EItemType GameData::GetItem(uint8_t _playerIdx)
{
	if (_playerIdx < Players.size())
	{
		return Players[_playerIdx].Item;
	}

	return EItemType::NONE;
}

void GameData::SetMapPakcage(const SMapPackage& _data)
{
	MapPackage = _data;
}

void GameData::SetMinimapLoc(uint8_t _playerIdx, float4 _loc)
{
	if (_playerIdx < Players.size())
	{
		Players[_playerIdx].MapLoc = _loc;
	}
}

float4 GameData::GetMinimapLoc(uint8_t _playerIdx) const
{
	if (_playerIdx < Players.size())
	{
		return Players[_playerIdx].MapLoc;
	}

	return float4{ 0.f, 0.f, 0.f, 0.f };
}

void GameData::SetPlayerRotation(uint8_t _playerIdx, float4 _rot)
{
	if (_playerIdx < Players.size())
	{
		Players[_playerIdx].Rotation = _rot;
	}
}

float4 GameData::GetPlayerRotation(uint8_t _playerIdx) const
{
	if (_playerIdx < Players.size())
	{
		return Players[_playerIdx].Rotation;
	}

	return float4{ 0.f, 0.f, 0.f, 0.f };
}

ECircuit GameData::NextMap()
{
	if (MapPackage.CurIdx >= 3)
	{
		return ECircuit::END;
	}

	return MapPackage.Maps[++MapPackage.CurIdx];
}

ECircuit GameData::GetCurMap() const
{
	return MapPackage.Maps[MapPackage.CurIdx];
}

void GameData::SetFinishState(EFinishState  _val)
{
	FinishState = _val;
}

EFinishState GameData::GetFinishState() const
{
	return FinishState;
}

void GameData::SetPlayerLap(int _val)
{
	Players[PlayerIdx].Lap = _val;
}

int GameData::GetPlayerLap() const
{
	return Players[PlayerIdx].Lap;
}