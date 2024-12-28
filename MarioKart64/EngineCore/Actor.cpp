#include "PreCompile.h"
#include "Actor.h"
#include "SceneComponent.h"

AActor::AActor()
{
}

AActor::~AActor()
{
	RootComponent = nullptr;
}

void AActor::BeginPlay()
{
	if (nullptr != RootComponent)
	{
		RootComponent->BeginPlay();
	}

	for (std::shared_ptr<class UActorComponent>& ActorComponent : ActorComponentList)
	{
		ActorComponent->BeginPlay();
	}
	
}

void AActor::Tick(float _DeltaTime)
{
	if (nullptr != RootComponent)
	{
		RootComponent->ComponentTick(_DeltaTime);
	}

	for (std::shared_ptr<class UActorComponent> ActorComponent : ActorComponentList)
	{
		ActorComponent->ComponentTick(_DeltaTime);
	}
}