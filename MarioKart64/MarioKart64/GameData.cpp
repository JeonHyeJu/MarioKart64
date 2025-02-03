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
	return static_cast<uint8_t>(Players.size());
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

void GameData::SetRankings(const std::vector<int>& _vec)
{
	size_t size = min(_vec.size(), Players.size());

	for (size_t i = 0; i < size; ++i)
	{
		int rank = _vec[i];
		Players[i].Rank = rank;
		RankCharacters[rank] = Players[i].Chracter;
	}
}

const std::vector<ECharacter>& GameData::GetRankings()
{
	return RankCharacters;
}

void GameData::SetPlayerItem(EItemType _item)
{
	Players[PlayerIdx].Item = _item;
}

EItemType GameData::GetPlayerItem()
{
	return Players[PlayerIdx].Item;
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

void GameData::SetPlayerTime(uint8_t _idx, float _val)
{
	if (_idx < Players.size())
	{
		Players[_idx].Time = _val;
	}
}

float GameData::GetPlayerTime(uint8_t _idx) const
{
	if (_idx < Players.size())
	{
		return Players[_idx].Time;
	}

	return -1;
}
