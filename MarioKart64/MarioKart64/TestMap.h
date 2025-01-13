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

	const std::vector<NavData>& GetNavData() const;
	const NavData& GetNavData(int _idx) const;
	const NavData& GetCurNavData() const;
	int GetNavIndex() const;
	void SetNavIndex(int _idx);

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::vector<FEngineVertex> Base;
	std::vector<NavData> BaseNav;
	std::shared_ptr<class ObjRenderer> Renderer = nullptr;
};
