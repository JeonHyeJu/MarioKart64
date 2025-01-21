#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/EngineVertex.h>
#include "CData.h"

class ATestMap : public AActor
{
public:
	ATestMap();
	~ATestMap();

	ATestMap(const ATestMap& _other) = delete;
	ATestMap(ATestMap&& _other) noexcept = delete;
	ATestMap& operator=(const ATestMap& _other) = delete;
	ATestMap& operator=(ATestMap&& _other) noexcept = delete;

	const std::vector<SNavData>& GetNavData() const;
	const SNavData& GetNavData(int _idx) const;
	const SNavData& GetCurNavData() const;
	int GetNavIndex() const;
	void SetNavIndex(int _idx);
	void SetDebugLocation(const FVector& _loc);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::vector<FEngineVertex> Base;
	std::vector<SNavData> BaseNav;
	std::shared_ptr<class CircuitRenderer> Renderer = nullptr;
};
