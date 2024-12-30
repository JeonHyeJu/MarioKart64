#include "PreCompile.h"
#include "TestMap.h"
#include "ObjRenderer.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>

ATestMap::ATestMap()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	// Temp
	UEngineDirectory dir;
	dir.MoveParentToDirectory("MarioKart64\\Resources\\Models\\Courses\\Royal_Raceway");
	std::string path = dir.GetPathToString();

	Renderer = CreateDefaultSubObject<ObjRenderer>();
	Renderer->Init(path + "\\Royal_Raceway.obj", path + "\\Royal_Raceway.mtl");
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
