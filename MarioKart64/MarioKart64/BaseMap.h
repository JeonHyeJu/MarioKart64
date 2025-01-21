#pragma once
#include <EngineCore/Actor.h>
#include "CData.h"

class BaseMap : public AActor
{
public:
	BaseMap();
	~BaseMap();

	BaseMap(const BaseMap& _other) = delete;
	BaseMap(BaseMap&& _other) noexcept = delete;
	BaseMap& operator=(const BaseMap& _other) = delete;
	BaseMap& operator=(BaseMap&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	std::shared_ptr<class CircuitRenderer> Renderer = nullptr;
};
