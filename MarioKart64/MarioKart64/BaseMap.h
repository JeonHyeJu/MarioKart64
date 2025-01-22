#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/EngineVertex.h>
#include "CData.h"

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

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::shared_ptr<class CircuitRenderer> RCircuit = nullptr;
	std::vector<SNavData>* NavData = nullptr;

	ECircuit MapType = ECircuit::END;
	int CurNavIdx = -1;
	
	// Dummy
	std::vector<FEngineVertex> Base;
	std::vector<SNavData> BaseNavs;
	SNavData BaseNav;
};
