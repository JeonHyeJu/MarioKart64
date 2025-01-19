#pragma once
#include <stdint.h>
#include "CData.h"

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

	void SetItem(uint8_t _playerIdx, EItemType _item);
	EItemType GetItem(uint8_t _playerIdx);

	static const int MAX_PLAYER_CNT = 8;

private:
	uint8_t PlayerRank = 8;
	uint8_t PlayerCnt = 0;
	uint8_t MapIdx = 0;

	// 0 is must be the user's character.
	std::vector<SPlayerInfo> Players;

	static GameData* pInstance;
};
