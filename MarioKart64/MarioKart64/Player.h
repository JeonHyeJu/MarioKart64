#pragma once
#include "Driver.h"
#include "CData.h"
#include <fstream>	// temp
#include <sstream>	// temp

class UCollision;
class APlayer : public ADriver
{
public:
	APlayer();
	~APlayer();

	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

	void EndLap() override;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	void TickItem(float _deltaTime) override;
	void GetHandleRotation(float _deltaTime, float& _refRot) override;

	void OnChangeLap(int _lap);
	void OnChangeItem(int _itemIdx);
	void OnCollisionEnter(UCollision* _this, UCollision* _other) override;

	void StartDriving() override;
	void Driving(float _deltaTime) override;
	void FinishDriving() override;

	void PlayHandleSound();

private:
	void TestForDebug(float _deltaTime);
	void GatherTrainigData(float _deltaTime);

	USoundPlayer BoostSP;
	USoundPlayer WheelSP;

	int PrevItemIdx = -1;
	int PrevH = -1;
};