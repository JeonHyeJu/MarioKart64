#include "PreCompile.h"
#include "TestMap.h"
#include "ObjRenderer.h"
#include "CGlobal.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/CameraActor.h>

ATestMap::ATestMap()
{
	Base.reserve(1);

	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	std::string path = CGlobal::GetModelPath("Courses\\Royal_Raceway", "Royal_Raceway");

	Renderer = CreateDefaultSubObject<ObjRenderer>();
	Renderer->SetOrder(0);
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

const std::vector<FEngineVertex>& ATestMap::GetRoadVertecies()
{
	if (Renderer == nullptr)
	{
		return Base;
	}
	
	return Renderer->GetRoadVertecies();
}