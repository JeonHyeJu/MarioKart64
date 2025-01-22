#include "PreCompile.h"
#include "BaseMap.h"
#include "CircuitRenderer.h"
#include "CGlobal.h"
#include "GameData.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/CameraActor.h>

ABaseMap::ABaseMap()
{
	Base.reserve(1);

	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	RCircuit = CreateDefaultSubObject<CircuitRenderer>();
	RCircuit->SetOrder(0);
	RCircuit->SetupAttachment(RootComponent);
}

ABaseMap::~ABaseMap()
{
}

void ABaseMap::BeginPlay()
{
	AActor::BeginPlay();
}

void ABaseMap::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
}

void ABaseMap::Init(ECircuit _type)
{
	MapType = _type;

	RCircuit->Init(MapType);
	if (SRenderInfo::MapInfos.contains(_type))
	{
		NavData = &SRenderInfo::MapInfos.find(MapType)->second.NavInfos;
	}
}

const std::vector<SNavData>& ABaseMap::GetNavData() const
{
	if (RCircuit == nullptr)
	{
		return BaseNavs;
	}

	return *NavData;
}

const SNavData& ABaseMap::GetNavData(UINT _idx) const
{
	if (_idx >= NavData->size())
	{
		return BaseNav;
	}

	return (*NavData)[_idx];
}

const SNavData& ABaseMap::GetCurNavData() const
{
	return GetNavData(CurNavIdx);
}

UINT ABaseMap::GetNavIndex() const
{
	return CurNavIdx;
}

void ABaseMap::SetNavIndex(UINT _idx)
{
	CurNavIdx = _idx;
}

void ABaseMap::SetDebugLocation(const FVector& _loc)
{
	if (RCircuit != nullptr)
	{
		RCircuit->SetDebugLocation(_loc);
	}
}
