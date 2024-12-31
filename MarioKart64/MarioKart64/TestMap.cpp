#include "PreCompile.h"
#include "TestMap.h"
#include "ObjRenderer.h"
#include "CGlobal.h"
#include <EngineCore/DefaultSceneComponent.h>

ATestMap::ATestMap()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	std::string path = CGlobal::GetModelPath("Courses\\Royal_Raceway", "Royal_Raceway");

	Renderer = CreateDefaultSubObject<ObjRenderer>();
	Renderer->Init(path);
	Renderer->SetupAttachment(RootComponent);
}

ATestMap::~ATestMap()
{
}

void ATestMap::BeginPlay()
{
	AActor::BeginPlay();
}

void ATestMap::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
}
