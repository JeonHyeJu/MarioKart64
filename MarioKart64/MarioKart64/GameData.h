#pragma once
#include <stdint.h>
#include "CData.h"
#include <EngineCore/EngineVertex.h>

enum class EFinishState
{
	FINISH_READY = 0,	// Set by PlayGameMode, Get in UIPlay
	FINISH_OPEN,		// Set by UIPlay, Get in PlayGameMode
	FINISH_COUNT,		// Set by PlayGameMode, Get in UIPlay
	FINISH_RACING,		// Set by PlayGameMode, Get in UIPlay
	FINISH_RESULT,		// Set by UIPlay, Get in PlayGameMode
	FINISH_FX,			// Set by PlayGameMode, Get in UIPlay
	FINISH_TOTAL,		// Set by UIPlay, Get in PlayGameMode
	END
};

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

	ECharacter GetPlayerCharacter() const;
	uint8_t GetPlayerIdx() const;
	uint8_t GetPlayerCnt() const;

	void SetPlayerRank(uint8_t _val);
	uint8_t GetPlayerRank() const;

	void SetPlayerIdx(uint8_t _idx);
	void SetPlayers(const std::vector<SPlayerInfo>& _players);
	const std::vector<SPlayerInfo>& GetPlayers() const;

	void SetMapPakcage(const SMapPackage& _data);

	void SetItem(uint8_t _playerIdx, EItemType _item);
	EItemType GetItem(uint8_t _playerIdx);

	void SetMinimapLoc(uint8_t _playerIdx, float4 _loc);
	float4 GetMinimapLoc(uint8_t _playerIdx) const;
	
	void SetPlayerRotation(uint8_t _playerIdx, float4 _rot);
	float4 GetPlayerRotation(uint8_t _playerIdx) const;

	ECircuit NextMap();
	ECircuit GetCurMap() const;

	void SetFinishState(EFinishState _val);
	EFinishState GetFinishState() const;

	void SetPlayerLap(uint8_t _playerIdx, int _val);
	int GetPlayerLap(uint8_t _playerIdx) const;

	static const int MAX_PLAYER_CNT = 8;
	static const int MAX_MAP_CNT = 4;

	SMapSizeInfo MapSizeInfo;

private:
	const uint8_t PlayerCnt = 6;
	uint8_t PlayerRank = 8;
	uint8_t PlayerIdx = 0;

	std::vector<SPlayerInfo> Players;
	SMapPackage MapPackage;

	static GameData* pInstance;

	ECircuit CurMapType = ECircuit::END;

	EFinishState FinishState = EFinishState::END;
};

