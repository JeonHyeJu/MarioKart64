#include "PreCompile.h"
#include "RainbowRoad.h"

ARainbowRoad::ARainbowRoad()
{
	MapType = ECircuit::RAINBOW_ROAD;
	MinimapInitLoc = { -83.f, 0.f, -140.f };

	StartPosition = {
		{ 900.f, 5050.f, -1000.f },
		{ 900.f, 5050.f, -1000.f },
		{ 900.f, 5050.f, -1000.f },
		{ 900.f, 5050.f, -1000.f },
		{ 900.f, 5050.f, -1000.f },
		{ 900.f, 5050.f, -1000.f }
	};

	Init();

	Scale = 100.f;
	SetActorRelativeScale3D({ Scale, Scale, Scale });

	// Item locations. for debug
	//Player->SetActorLocation({ 1509.f, 2435.f, 24884.f });

	//Player->SetActorLocation({ -605.f, 2155.f, 15872.f });

	//Player->SetActorLocation({ -6979.f, -80.f, 4569.f });

	//Player->SetActorLocation({ -8419.f, 1035.f, -5402.f });

	//Player->SetActorLocation({ -7871.f, 1155.f, -11415.f });

	//Player->SetActorLocation({ 7310.f, 1155.f, -10154.f });

	//Player->SetActorLocation({ -364.f, 2137.f, -13920.f });

	//Player->SetActorLocation({ 2654.f, 4050.f, -12532.f });
}

ARainbowRoad::~ARainbowRoad()
{

}

void ARainbowRoad::BeginPlay()
{
	ABaseMap::BeginPlay();
}

void ARainbowRoad::Tick(float _deltaTime)
{
	ABaseMap::Tick(_deltaTime);
}

void ARainbowRoad::InitItem()
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
}