#include "PreCompile.h"
#include "GameData.h"

GameData* GameData::pInstance = nullptr;

GameData::GameData()
{
	Players.reserve(MAX_PLAYER_CNT);
	Maps.reserve(MAX_MAP_CNT);
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

void GameData::SetMapIdx(uint8_t _val)
{
	MapIdx = _val;
}

uint8_t GameData::GetMapIdx() const
{
	return MapIdx;
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

void GameData::SetPlayers(const std::vector<SPlayerInfo>& _players)
{
	Players = _players;	// copy
	PlayerCnt = static_cast<uint8_t>(Players.size());
}

const std::vector<SPlayerInfo>& GameData::GetPlayers() const
{
	return Players;
}

void GameData::GetHighRankPlayersIdx(int* _refArr, int _size)
{
	// Temp
	assert(_size == 4 && "High rank has 4 size only.");

	// Temp
	_refArr[0] = 1;
	_refArr[1] = 2;
	_refArr[2] = 3;
	_refArr[3] = 4;
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

void GameData::SetMaps(const std::vector<ECircuit>& _maps)
{
	Maps = _maps;
}

const std::vector<ECircuit>& GameData::GetMaps() const
{
	return Maps;
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

void GameData::SetCurMap(ECircuit _type)
{
	CurMapType = _type;
}

ECircuit GameData::GetCurMap() const
{
	return CurMapType;
}

void GameData::SetFinishState(EFinishState  _val)
{
	FinishState = _val;
}

EFinishState GameData::GetFinishState() const
{
	return FinishState;
}

void GameData::SetPlayerLap(uint8_t _playerIdx, int _val)
{
	if (_playerIdx < Players.size())
	{
		Players[_playerIdx].Lap = _val;
	}
}

int GameData::GetPlayerLap(uint8_t _playerIdx) const
{
	if (_playerIdx < Players.size())
	{
		return Players[_playerIdx].Lap;
	}

	return -1;
}