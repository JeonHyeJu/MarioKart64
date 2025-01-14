#include "PreCompile.h"
#include "SceneComponent.h"

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