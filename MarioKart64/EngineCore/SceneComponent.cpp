#include "PreCompile.h"
#include "SceneComponent.h"
#include <EngineCore/EngineCamera.h>	// Temp

USceneComponent::USceneComponent()
{
}

USceneComponent::~USceneComponent()
{
}

void USceneComponent::BeginPlay()
{
	UActorComponent::BeginPlay();

	for (UTransformObject* Child : Childs)
	{
		USceneComponent* SceneChild = dynamic_cast<USceneComponent*>(Child);

		// Temp
		UEngineCamera* cam = dynamic_cast<UEngineCamera*>(SceneChild);
		if (cam != nullptr)
		{
			continue;
		}

		SceneChild->BeginPlay();
	}
}

void USceneComponent::ComponentTick(float _DeltaTime)
{
	UActorComponent::ComponentTick(_DeltaTime);

	for (UTransformObject* Child : Childs)
	{

		USceneComponent* SceneChild = dynamic_cast<USceneComponent*>(Child);

		if (false == SceneChild->IsActive())
		{
			continue;
		}

		SceneChild->ComponentTick(_DeltaTime);
	}
}