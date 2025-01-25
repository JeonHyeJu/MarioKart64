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

	void Init(ECircuit _type);

	const std::vector<SNavData>& GetNavData() const;
	const SNavData& GetNavData(UINT _idx) const;
	const SNavData& GetCurNavData() const;
	UINT GetNavIndex() const;
	void SetNavIndex(UINT _idx);
	void SetDebugLocation(const FVector& _loc);
	void CheckAllItemBoxes(float _deltaTime);
	void RemoveAllItemBoxes();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	class AItemBox* CreateItemBox(const FVector& _pos);

	std::shared_ptr<class CircuitRenderer> RCircuit = nullptr;
	std::vector<SNavData>* NavDataPtr = nullptr;
	std::vector<SItemInfo> ItemBoxes;

	ECircuit MapType = ECircuit::END;
	int CurNavIdx = -1;
	const float ITEM_REGEN_SEC = 10.f;

	// Dummy for return of reference
	std::vector<FEngineVertex> DummyVertex;
	std::vector<SNavData> DummyNavs;
	SNavData DummyNav;

	UINT Seed = 1;
};
