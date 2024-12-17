#include "PreCompile.h"
#include "Actor.h"
#include "SceneComponent.h"

AActor::AActor()
{
}

AActor::~AActor()
{
}

void AActor::BeginPlay()
{
	if (nullptr != RootComponent)
	{
		RootComponent->BeginPlay();
	}
	
}

void AActor::Tick(float _DeltaTime)
{

}