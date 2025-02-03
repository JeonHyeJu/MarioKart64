#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/EngineVertex.h>
#include "CData.h"

struct SItemInfo
{
	FVector Position = FVector::ZERO;
	float RegenSec = 0.f;
	class AItemBox* ItemBox = nullptr;
};

class ABaseMap : public AActor
{
public:
	ABaseMap();
	~ABaseMap();

	ABaseMap(const ABaseMap& _other) = delete;
	ABaseMap(ABaseMap&& _other) noexcept = delete;
	ABaseMap& operator=(const ABaseMap& _other) = delete;
	ABaseMap& operator=(ABaseMap&& _other) noexcept = delete;

	const std::vector<SNavData>& GetNavData() const;
	const SNavData& GetNavData(UINT _idx) const;
	void SetDebugLocation(const FVector& _loc);
	void CheckAllItemBoxes(float _deltaTime);
	void RemoveAllItemBoxes();

	float GetScale() const
	{
		return Scale;
	}
	const FVector& GetMinimapInitLoc() const
	{
		return MinimapInitLoc;
	}
	const std::vector<FVector>& GetStartPosition() const
	{
		return StartPosition;
	}

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

	void Init();
	virtual void InitItem() {}

	ECircuit MapType = ECircuit::END;

	FVector MinimapInitLoc = FVector::ZERO;
	std::vector<FVector> StartPosition;
	std::vector<SItemInfo> ItemBoxes;

	std::vector<SNavData>* NavDataPtr = nullptr;
	UINT Seed = 1;
	float Scale = 1.f;

private:
	class AItemBox* CreateItemBox(const FVector& _pos);

	std::shared_ptr<class CircuitRenderer> RCircuit = nullptr;

	const float ITEM_REGEN_SEC = 10.f;

	// Dummy for return of reference
	std::vector<FEngineVertex> DummyVertex;
	std::vector<SNavData> DummyNavs;
	SNavData DummyNav;
};
