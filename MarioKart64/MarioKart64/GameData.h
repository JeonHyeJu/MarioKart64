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

	void SetRankings(const std::vector<int>& _vec);
	const std::vector<ECharacter>& GetRankings();

	ECharacter GetPlayerCharacter() const;
	uint8_t GetPlayerCnt() const;

	void SetPlayerIdx(uint8_t _idx);
	uint8_t GetPlayerIdx() const;

	void SetPlayerRank(uint8_t _val);
	uint8_t GetPlayerRank() const;

	void SetPlayers(const std::vector<SPlayerInfo>& _players);
	const std::vector<SPlayerInfo>& GetPlayers() const;

	void SetMapPakcage(const SMapPackage& _data);

	void SetPlayerItem(EItemType _item);
	EItemType GetPlayerItem();

	void SetMinimapLoc(uint8_t _playerIdx, float4 _loc);
	float4 GetMinimapLoc(uint8_t _playerIdx) const;
	
	void SetPlayerRotation(uint8_t _playerIdx, float4 _rot);
	float4 GetPlayerRotation(uint8_t _playerIdx) const;

	ECircuit NextMap();
	ECircuit GetCurMap() const;

	void SetFinishState(EFinishState _val);
	EFinishState GetFinishState() const;

	void SetPlayerLap(int _val);
	int GetPlayerLap() const;

	static const int MAX_PLAYER_CNT = 8;
	static const int MAX_MAP_CNT = 4;

	SMapSizeInfo MapSizeInfo;

private:
	uint8_t PlayerRank = 8;
	uint8_t PlayerIdx = 0;

	std::vector<SPlayerInfo> Players;
	std::vector<ECharacter> RankCharacters = { ECharacter::NONE, ECharacter::NONE, ECharacter::NONE, ECharacter::NONE, ECharacter::NONE, ECharacter::NONE };
	SMapPackage MapPackage;

	static GameData* pInstance;

	ECircuit CurMapType = ECircuit::END;

	EFinishState FinishState = EFinishState::END;
};

