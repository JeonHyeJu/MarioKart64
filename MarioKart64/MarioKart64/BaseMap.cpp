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

void ABaseMap::Init(ECircuit _type)
{
	MapType = _type;

	RCircuit->Init(MapType);
	if (SRenderInfo::MapInfos.contains(_type))
	{
		NavDataPtr = &SRenderInfo::MapInfos.find(MapType)->second.NavInfos;
	}

	// TODO: move with type
	switch (_type)
	{
	case ECircuit::LUIGI_RACEWAY:
	{
		float initX = -620.f;
		float x = 56.f;
		float y = -130.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, 7485.f }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, 7486.f }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, 7487.f }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3, y, 7488.f }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4, y, 7489.f }, 0.f, nullptr });

		initX = -1101.f;
		x = -56.f;
		y = -750.f;
		ItemBoxes.emplace_back(SItemInfo { FVector{ initX, y, 2427.f }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo { FVector{ initX + x * 1, y, 2427.f }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo { FVector{ initX + x * 2, y, 2427.f }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo { FVector{ initX + x * 3, y, 2427.f }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4, y, 2427.f }, 0.f, nullptr });

		float initZ = -6580.f;
		initX = -3479.f;
		x = -56.f;
		y = -160.f;
		ItemBoxes.emplace_back(SItemInfo { FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo { FVector{ initX, y + 15 * 1, initZ + x * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo { FVector{ initX, y + 15 * 2, initZ + x * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo { FVector{ initX, y + 15 * 3, initZ + x * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo { FVector{ initX, y + 15 * 4, initZ + x * 4 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y + 15 * 5, initZ + x * 5 }, 0.f, nullptr });
		break;
	}
	case ECircuit::KOOPA_TROOPA_BEACH:
	{
		float initX = -700.f;
		float initZ = 4619.f;
		float x = 56.f;
		float z = 72.f;
		float y = 30.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 5 + 10.f, y, initZ + z * 5 }, 0.f, nullptr });

		initX = -7648.f;
		initZ = -2625.f;
		x = -56.f;
		z = -56.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 5 + 10.f, y, initZ + z * 5 }, 0.f, nullptr });

		initX = -6489.f;
		initZ = -12730.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 5 + 10.f, y, initZ + z * 5 }, 0.f, nullptr });
		break;
	}
	case ECircuit::MARIO_RACEWAY:
	{
		float initX = 2371.f;
		float initZ = 4456;
		float x = 56.f;
		float z = -56.f;
		float y = 30.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });

		initX = 10237.f;
		initZ = 0.f;
		x = -60.f;
		z = 20.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });

		initX = 8159.f;
		initZ = -3258.f;
		x = 60.f;
		z = 60.f;
		y = 40.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y + 10.f, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y + 15.f, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y + 20.f, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y + 25.f, initZ + z * 4 }, 0.f, nullptr });
		break;
	}
	case ECircuit::WARIO_STADIUM:
	{
		float initX = -3160.f;
		float initZ = 79;
		float x = -126.f;
		float z = -126.f;
		float y = 30.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });

		initX = 3469.f;
		initZ = 6536.f;
		x = -176.f;
		z = -126.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });

		initX = 212.f;
		initZ = -1404.f;
		x = -100.f;
		z = -1.f;
		y = -230.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });
		break;
	}
	case ECircuit::SHERBET_LAND:
	{
		float initX = -8208.f;
		float initZ = -11032.f;
		float x = -126.f;
		float z = -126.f;
		float y = -230.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });

		y = 125.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ -4629.f, y, 4268.f }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ -7597.f, y, -3230.f }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ -7666.f, y, -3340.f }, 0.f, nullptr });

		initX = -803.f;
		initZ = -3523.f;
		x = -126.f;
		z = -126.f;
		y = 170.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });
		break;
	}
	case ECircuit::ROYAL_RACEWAY:
	{
		float initX = -2018.f;
		float initZ = 8627.f;
		float x = 26.f;
		float z = 56.f;
		float y = 60.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y + 10.f, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y + 15.f, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y + 20.f, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y + 25.f, initZ + z * 4 }, 0.f, nullptr });

		initX = 2908.f;
		initZ = 11147.f;
		x = 56.f;
		z = -40.f;
		y = 45.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });

		initX = -8830.f;
		initZ = 1832;
		x = -56.f;
		z = 40.f;
		y = 30.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });
		break;
	}
	case ECircuit::BOWSERS_CASTLE:
	{
		float initX = -21.f;
		float initZ = 6297.f;
		float x = 50.f;
		float z = -56.f;
		float y = 30.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });

		initX = 4586.f;
		initZ = 3055.f;
		x = 0.f;
		z = 76.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });

		initX = 4307.f;
		initZ = -1884.f;
		x = 0.f;
		z = 126.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });
		break;
	}
	case ECircuit::RAINBOW_ROAD:
	{
		float initX = 2465.f;
		float initZ = 26145.f;
		float x = 46.f;
		float z = -86.f;
		float y = 2470.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });

		initX = -1872.f;
		initZ = 14975.f;
		x = -56.f;
		z = 76.f;
		y = 2025.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });

		initX = -7173.f;
		initZ = 3573.f;
		x = -56.f;
		z = 56.f;
		y = -10.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 5 + 10.f, y, initZ + z * 5 }, 0.f, nullptr });

		initX = -8557.f;
		initZ = -5874.f;
		x = -66.f;
		z = 66.f;
		y = 1100.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4 + 10.f, y, initZ + z * 4 }, 0.f, nullptr });

		initX = -7562.f;
		initZ = -11059.f;
		x = 56.f;
		z = -56.f;
		y = 1200.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });

		initX = 7683.f;
		initZ = -10054.f;
		x = -15.f;
		z = -106.f;
		y = 1390.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });

		initX = -900.f;
		initZ = -14018.f;
		x = -15.f;
		z = 106.f;
		y = 2200.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });

		initX = 2519.f;
		initZ = -12183.f;
		x = 106.f;
		z = 0.f;
		y = 4135.f;
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, initZ + z * 1 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, initZ + z * 2 }, 0.f, nullptr });
		ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3 + 10.f, y, initZ + z * 3 }, 0.f, nullptr });
		break;
	}
	}

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