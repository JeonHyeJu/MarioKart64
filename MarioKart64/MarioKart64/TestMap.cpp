#include "PreCompile.h"
#include "TestMap.h"
#include "ObjRenderer.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>

ATestMap::ATestMap()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;
	Renderer = CreateDefaultSubObject<ObjRenderer>();
	Renderer->SetSprite("Mario.png");
	Renderer->SetSpriteData(0);
	Renderer->SetScale3D({ 100.f, 100.f, 1.f });
	Renderer->SetupAttachment(RootComponent);
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
