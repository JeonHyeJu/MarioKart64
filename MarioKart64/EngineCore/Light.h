#pragma once
#include "SceneComponent.h"
#include "EngineSprite.h"
#include "RenderUnit.h"

struct FLightData
{
    float4 LightPos;
    float4 LightDir;
    float4 LightRevDir;
    float4 LightColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
    float4 AmbientLight = float4(0.1f, 0.1f, 0.1f, 1.0f);
    float4 ViewLightPos;
    float4 ViewLightDir;
    float4 ViewLightRevDir;
    float4 CameraPosition; 
};

struct FLightDatas 
{
    int Count;
    FLightData LightArr[256];
};

class ULight : public USceneComponent
{
    friend class ULevel;
	friend class UEngineCamera;

public:
	ENGINEAPI ULight();
	ENGINEAPI ~ULight();

	ULight(const ULight& _Other) = delete;
	ULight(ULight&& _Other) noexcept = delete;
	ULight& operator=(const ULight& _Other) = delete;
	ULight& operator=(ULight&& _Other) noexcept = delete;

protected:
    ENGINEAPI void BeginPlay() override;


private:
    FLightData LightData;

    void LightUpdate(class UEngineCamera* _Camera, float _DeltaTime);
};

