#include "PreCompile.h"
#include "KoopaTroopaBeach.h"

AKoopaTroopaBeach::AKoopaTroopaBeach()
{
	MapType = ECircuit::KOOPA_TROOPA_BEACH;
	MinimapInitLoc = { -52.f, 0.f, -100.f };

	StartPosition = {
		{ 0.0f, 20.0f, -1000.0f },
		{ 0.0f, 20.0f, -1000.0f },
		{ 0.0f, 20.0f, -1000.0f },
		{ 0.0f, 20.0f, -1000.0f },
		{ 0.0f, 20.0f, -1000.0f },
		{ 0.0f, 20.0f, -1000.0f }
	};

	Init();

	Scale = 50.f;
	SetActorRelativeScale3D({ Scale, Scale, Scale });

	// Item locations. for debug
	//Player->SetActorLocation({ -395.0f, 15.0f, 4370});

	//Player->SetActorLocation({ -7911.f, 15.0f, -2318.f });

	//Player->SetActorLocation({ -6686.f, 15.0f, -12073.f });
}

AKoopaTroopaBeach::~AKoopaTroopaBeach()
{

}

void AKoopaTroopaBeach::BeginPlay()
{
	ABaseMap::BeginPlay();
}

void AKoopaTroopaBeach::Tick(float _deltaTime)
{
	ABaseMap::Tick(_deltaTime);
}

void AKoopaTroopaBeach::InitItem()
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
}