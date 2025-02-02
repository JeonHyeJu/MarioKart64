#pragma once
#include <EngineCore/Pawn.h>
#include "CData.h"

class USpriteRenderer;
class LineRenderer;
class UCollision;
class ADriver : public APawn
{
public:
	ADriver();
	~ADriver();

	ADriver(const ADriver& _Other) = delete;
	ADriver(ADriver&& _Other) noexcept = delete;
	ADriver& operator=(const ADriver& _Other) = delete;
	ADriver& operator=(ADriver&& _Other) noexcept = delete;

	void InitCharacter(ECharacter _character);
	void SetMap(class ABaseMap* _ptr);
	void SetStart(bool _val);
	void InitRouteIndex(ECircuit _map);

	float GetVelocity() const
	{
		return Velocity;
	}
	int GetRouteIdx() const
	{
		return CurRouteIdx;
	}
	int GetLap() const
	{
		return Lap;
	}
	bool GetIsCheckingLap() const
	{
		return IsCheckLap;
	}
	bool GetIsFinished() const
	{
		return IsFinished;
	}
	float GetDistFromNextRoute();
	float GetDistWithRouteIdx(int _idx);
	int GetNextRouteIdx();

	int Rank = -1;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	virtual void GetForwardPhysics(float _deltaTime, float& _refDx, bool _isCollided = true);
	virtual void GetHandleRotation(float _deltaTime, float& _refRot);
	virtual void TickItem(float _deltaTime);
	virtual void PickItem(float _deltaTime);
	virtual void OnChangeLap(int _lap) {};
	virtual void OnChangeItem(int _itemIdx) {};
	virtual void EndLap() {};

	void GetHandleRotationAuto(float _deltaTime, const FVector& _dir, float& _refRot);
	void UseItem();
	void Spin();

	// Temp
	void UseItem_Shell(const EItemType& _itemType);
	void UseItem_Mushroom(const EItemType& _itemType);
	void UseItem_Banana(const EItemType& _itemType);
	void UseItem_Star(const EItemType& _itemType);
	void UseItem_Thunder(const EItemType& _itemType);
	void UseItem_Ghost(const EItemType& _itemType);
	void UseItem_FakeItemBox(const EItemType& _itemType);

	const int ITEM_NONE = static_cast<int>(EItemType::SIZE);

	bool IsAutomative = true;
	bool IsStarted = false;
	bool IsFinished = false;
	bool IsCheckLap = false;
	bool IsSpin = false;

	const float WHEEL_ACCEL = 80.f;
	float WheelVelocity = 0.f;

	float Velocity = 0.f;
	float VelocityV = 0.f;

	int Lap = 1;
	int ItemIndex = ITEM_NONE;

	const float BOOST_SPEED_UP = 2000.f;
	const float BOOST_SPEED_DOWN = 350.f;
	float BoostVal = 0.f;
	bool IsBoost = false;
	int MushroomBoostCnt = 0;

	// For train
	bool IsTraining = false;
	int DirVTrain = 0;
	int DirHTrain = 0;
	bool IsReverseTrain = false;
	std::vector<TrainData> TrainDatas;

	std::shared_ptr<USpriteRenderer> Renderer = nullptr;
	std::string SpriteName = "";
	FVector RendererSize = FVector::ZERO;

	// Temp
	std::stringstream FileTemp;

private:
	void Move(float _deltaTime);
	void TickBoost(float _deltaTime);

	bool CheckCollision(const FVector& _loc, int& _refIdx, float& _refDist);
	void CheckCollisionOfAllMap();
	void CheckFutureCollisionOfAllMap();
	float GetSlope();
	void CheckLap(bool _isReverse);

	void OnCollisionEnter(UCollision* _this, UCollision* _other);

	std::shared_ptr<UCollision> CollisionItem = nullptr;
	ABaseMap* MapPtr = nullptr;

	const int ALL_LAB = 3;

	const float FRICTIONAL_FORCE = .05f;
	const float MAX_SPEED = 20.f;
	const float MAX_TURN = 5.f;

	const float WEIGHT = 50.f;
	const float ACCELERATION = WEIGHT * 6.f;
	const float FRICTION_FORCE = WEIGHT * 2.f;	// 100px/s
	const float MAX_VELOCITY = 1000.f;
	const float GRAVITY_FORCE = -300.f;

	int PrevIdx = -1;
	int PrevGroupIdx = -1;

	int SpinCount = 0;

	SItemRoulette ItemRoulette;
	bool IsPickingItem = false;

	FVector PrevLoc = FVector::NONE;

	int NavIdx = -1;
	int FutureNavIdxs[3] = { 0, 0, 0 };

	// Temp S
	std::shared_ptr<class USpriteRenderer> DebugItem = nullptr;
	std::vector<float> FutureAngles;

	std::shared_ptr<USpriteRenderer> RendererDebug;
	bool isLog = false;

	int PrevNavIdx = -1;
	int PrevItemIdx = -1;
	// Temp E

	int DstRouteNavIdx = 1;
	int CurRouteIdx = 0;
	std::map<int, int> RouteIdx;
	
	const float STRIDE = 700.f;

	const char* RENDER_SPRITES[6] = {
		"Mario.png",
		"Luigi.png",
		"Peach.png",
		"Yoshi.png",
		"Wario.png",
		"Bowser.png",
	};
};

