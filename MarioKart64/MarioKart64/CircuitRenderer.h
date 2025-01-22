#pragma once
#include <EngineCore/Renderer.h>
#include <EngineBase/EngineDefine.h>
#include "CData.h"

class UEngineCamera;
class ModelLoader;
class CircuitRenderer : public URenderer
{
	friend class UEngineCamera;

public:
	CircuitRenderer();
	~CircuitRenderer();

	CircuitRenderer(const CircuitRenderer& _other) = delete;
	CircuitRenderer(CircuitRenderer&& _other) noexcept = delete;
	CircuitRenderer& operator=(const CircuitRenderer& _other) = delete;
	CircuitRenderer& operator=(CircuitRenderer&& _other) noexcept = delete;

	void Init(ECircuit _map);
	void SetDebugLocation(const FVector& _loc)
	{
		Debug.Index = _loc;
	}

protected:
	void BeginPlay() override;
	void Render(UEngineCamera* _camera, float _deltaTime) override;

private:
	void Sort(std::vector<SRenderInfo>& _refVec);

	SNavData Base;
	FDebug Debug;
};
