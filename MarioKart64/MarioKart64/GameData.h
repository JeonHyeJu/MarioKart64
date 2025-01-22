#pragma once
#include <stdint.h>
#include "CData.h"
#include <EngineCore/EngineVertex.h>

class GameData
{
public:
	GameData();
	~GameData();

	GameData(const GameData& _other) = delete;
	GameData(GameData&& _other) noexcept = delete;
	GameData& operator=(const GameData& _other) = delete;
	GameData& operator=(GameData&& _other) noexcept = delete;

	void Release();
	static GameData* GetInstance();

	void GetHighRankPlayersIdx(int* _refArr, int _size);

	void SetMapIdx(uint8_t _val);
	uint8_t GetMapIdx() const;

	uint8_t GetPlayerCnt() const;

	void SetPlayerRank(uint8_t _val);
	uint8_t GetPlayerRank() const;

	void SetPlayers(const std::vector<SPlayerInfo>& _players);
	const std::vector<SPlayerInfo>& GetPlayers() const;

	void SetMaps(const std::vector<ECircuit>& _maps);
	const std::vector<ECircuit>& GetMaps() const;

	void SetItem(uint8_t _playerIdx, EItemType _item);
	EItemType GetItem(uint8_t _playerIdx);

	void SetMinimapLoc(uint8_t _playerIdx, float4 _loc);
	float4 GetMinimapLoc(uint8_t _playerIdx) const;
	
	void SetPlayerRotation(uint8_t _playerIdx, float4 _rot);
	float4 GetPlayerRotation(uint8_t _playerIdx) const;

	void SetCurMap(ECircuit _type);
	ECircuit GetCurMap() const;

	static const int MAX_PLAYER_CNT = 8;
	static const int MAX_MAP_CNT = 4;

	SMapSizeInfo MapSizeInfo;

private:
	uint8_t PlayerRank = 8;
	uint8_t PlayerCnt = 0;
	uint8_t MapIdx = 0;

	// 0 is must be the user's character.
	std::vector<SPlayerInfo> Players;
	std::vector<ECircuit> Maps;

	static GameData* pInstance;

	ECircuit CurMapType = ECircuit::END;
};
