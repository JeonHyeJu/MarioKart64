#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/EngineVertex.h>

class ATestMap : public AActor
{
public:
	ATestMap();
	~ATestMap();

	ATestMap(const ATestMap& _other) = delete;
	ATestMap(ATestMap&& _other) noexcept = delete;
	ATestMap& operator=(const ATestMap& _other) = delete;
	ATestMap& operator=(ATestMap&& _other) noexcept = delete;

	const std::vector<FEngineVertex>& GetRoadVertecies();

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::vector<FEngineVertex> Base;
	std::shared_ptr<class ObjRenderer> Renderer = nullptr;
};
