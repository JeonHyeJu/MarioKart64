#include "PreCompile.h"
#include "TestMap.h"
#include "CircuitRenderer.h"
#include "CGlobal.h"
#include "GameData.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/CameraActor.h>

ATestMap::ATestMap()
{
	Base.reserve(1);

	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	std::string path = CGlobal::GetModelPath("Courses\\Royal_Raceway", "Royal_Raceway");

	Renderer = CreateDefaultSubObject<CircuitRenderer>();
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

const std::vector<NavData>& ATestMap::GetNavData() const
{
	if (Renderer == nullptr)
	{
		return BaseNav;
	}

	return Renderer->GetNavData();
}

const NavData& ATestMap::GetNavData(int _idx) const
{
	return Renderer->GetNavData(_idx);
}

const NavData& ATestMap::GetCurNavData() const
{
	return Renderer->GetCurNavData();
}

int ATestMap::GetNavIndex() const
{
	if (Renderer == nullptr)
	{
		return -1;
	}

	return Renderer->GetNavIndex();
}

void ATestMap::SetNavIndex(int _idx)
{
	if (Renderer == nullptr)
	{
		return;
	}

	return Renderer->SetNavIndex(_idx);
}