#include "PreCompile.h"
#include "TestMap.h"
#include "ObjRenderer.h"
#include <EngineCore/DefaultSceneComponent.h>

ATestMap::ATestMap()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;
	Renderer = CreateDefaultSubObject<ObjRenderer>();
}

ATestMap::~ATestMap()
{

}

void ATestMap::BeginPlay()
{
	AActor::BeginPlay();
}

void ATestMap::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}
