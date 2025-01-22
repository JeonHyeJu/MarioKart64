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

	Renderer = CreateDefaultSubObject<CircuitRenderer>();
	Renderer->SetOrder(0);
	Renderer->SetupAttachment(RootComponent);
}

void ATestMap::Init(ECircuit _type)
{
	MapType = ECircuit::ROYAL_RACEWAY;

	Renderer->Init(MapType);
	if (SRenderInfo::MapInfos.contains(_type))
	{
		NavData = &SRenderInfo::MapInfos.find(MapType)->second.NavInfos;
	}
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

const std::vector<SNavData>& ATestMap::GetNavData() const
{
	if (Renderer == nullptr)
	{
		return BaseNavs;
	}

	return *NavData;
}

const SNavData& ATestMap::GetNavData(UINT _idx) const
{
	if (_idx >= NavData->size())
	{
		return BaseNav;
	}

	return (*NavData)[_idx];
}

const SNavData& ATestMap::GetCurNavData() const
{
	return GetNavData(CurNavIdx);
}

UINT ATestMap::GetNavIndex() const
{
	return CurNavIdx;
}

void ATestMap::SetNavIndex(UINT _idx)
{
	CurNavIdx = _idx;
}

void ATestMap::SetDebugLocation(const FVector& _loc)
{
	if (Renderer != nullptr)
	{
		Renderer->SetDebugLocation(_loc);
	}
}