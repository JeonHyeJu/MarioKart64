#pragma once
#include "SceneComponent.h"
#include "EngineSprite.h"
#include "RenderUnit.h"

class URenderer : public USceneComponent
{
	friend class UEngineCamera;

public:
	ENGINEAPI URenderer();
	ENGINEAPI ~URenderer();

	URenderer(const URenderer& _Other) = delete;
	URenderer(URenderer&& _Other) noexcept = delete;
	URenderer& operator=(const URenderer& _Other) = delete;
	URenderer& operator=(URenderer&& _Other) noexcept = delete;

	ENGINEAPI void SetOrder(int _Order);
	ENGINEAPI void BeginPlay() override;

	ENGINEAPI virtual void RenderTransUpdate(UEngineCamera* _Camera);

	ENGINEAPI virtual void Render(UEngineCamera* _Camera, float _DeltaTime);
	ENGINEAPI URenderUnit& GetRenderUnit(UINT  _Index = 0);
	ENGINEAPI URenderUnit& CreateRenderUnit();
	ENGINEAPI void SetMesh(std::string_view _Name, UINT _Index = 0);
	ENGINEAPI void SetMaterial(std::string_view _Name, UINT _Index = 0);

private:

public:
	std::vector<URenderUnit> Units;
};

