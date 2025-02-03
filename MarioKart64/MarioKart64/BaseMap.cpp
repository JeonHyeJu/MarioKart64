#include "PreCompile.h"
#include "BaseMap.h"
#include "CircuitRenderer.h"
#include "CGlobal.h"
#include "GameData.h"
#include "ItemBox.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/CameraActor.h>

ABaseMap::ABaseMap()
{
	DummyVertex.reserve(0);
	DummyNavs.reserve(0);
	ItemBoxes.reserve(50);

	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	RCircuit = CreateDefaultSubObject<CircuitRenderer>();
	RCircuit->SetOrder(0);
	RCircuit->SetupAttachment(RootComponent);

	srand(Seed);
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

	CheckAllItemBoxes(_deltaTime);
}

void ABaseMap::Init()
{
	RCircuit->Init(MapType);
	if (SRenderInfo::MapInfos.contains(MapType))
	{
		NavDataPtr = &SRenderInfo::MapInfos.find(MapType)->second.NavInfos;
	}

	InitItem();
	for (int i = 0, size = static_cast<int>(ItemBoxes.size()); i < size; ++i)
	{
		AItemBox* ptr = CreateItemBox(ItemBoxes[i].Position);
		ItemBoxes[i].ItemBox = ptr;
	}
}

AItemBox* ABaseMap::CreateItemBox(const FVector& _pos)
{
	std::shared_ptr<AItemBox> ptr = GetWorld()->SpawnActor<AItemBox>();
	ptr->SetActorLocation(_pos);
	bool isRotX = (rand() % 2 == 0);
	bool isRotY = (rand() % 2 == 0);
	bool isRotZ = (rand() % 2 == 0);
	ptr->SetRandomState(isRotX, isRotY, isRotZ);
	return ptr.get();
}

void ABaseMap::CheckAllItemBoxes(float _deltaTime)
{
	for (size_t i = 0, size = ItemBoxes.size(); i < size; ++i)
	{
		SItemInfo& itemBox = ItemBoxes[i];

		if (itemBox.ItemBox == nullptr)
		{
			itemBox.RegenSec += _deltaTime;
			if (itemBox.RegenSec >= ITEM_REGEN_SEC)
			{
				itemBox.RegenSec = 0.f;

				// Regenerate itemBox
				AItemBox* ptr = CreateItemBox(itemBox.Position);
				itemBox.ItemBox = ptr;
			}
			continue;
		}

		if (itemBox.ItemBox->IsActive() == false)
		{
			itemBox.ItemBox->Destroy();
			itemBox.ItemBox = nullptr;
		}
	}
}

// ItemBoxes were not exectued AttachToActor()
void ABaseMap::RemoveAllItemBoxes()
{
	for (SItemInfo& data : ItemBoxes)
	{
		if (data.ItemBox != nullptr)
		{
			data.ItemBox->Destroy();
		}
	}

	ItemBoxes.clear();
}

const std::vector<SNavData>& ABaseMap::GetNavData() const
{
	if (NavDataPtr == nullptr)
	{
		return DummyNavs;
	}

	return *NavDataPtr;
}

const SNavData& ABaseMap::GetNavData(UINT _idx) const
{
	if (NavDataPtr == nullptr || _idx >= NavDataPtr->size())
	{
		return DummyNav;
	}

	return (*NavDataPtr)[_idx];
}

void ABaseMap::SetDebugLocation(const FVector& _loc)
{
	if (RCircuit != nullptr)
	{
		RCircuit->SetDebugLocation(_loc);
	}
}