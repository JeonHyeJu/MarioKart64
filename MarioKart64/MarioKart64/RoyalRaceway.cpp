#include "PreCompile.h"
#include "RoyalRaceway.h"

ARoyalRaceway::ARoyalRaceway()
{
	MapType = ECircuit::ROYAL_RACEWAY;
	MinimapInitLoc = { -90.f, 0.f, -80.f };

	StartPosition = {
		{ -300.0f, 0.0f, 900.0f },
		{ -150.0f, 0.0f, 900.0f },
		{ -300.0f, 0.0f, 700.0f },
		{ -150.0f, 0.0f, 700.0f },
		{ -300.0f, 0.0f, 500.0f },
		{ -150.0f, 0.0f, 500.0f }
	};

	Init();

	Scale = 4.f;
	SetActorLocation({ 60.0f, 0.f, 0.f });
	SetActorRelativeScale3D({ Scale, Scale, Scale });

	// Item location. for debug
	//Player->SetActorLocation({ -1500.0f, 100.0f, 8485.0f });

	//Player->SetActorLocation({ 2373.0f, 100.0f, 10796.0f });

	//Player->SetActorLocation({ -8662.0f, 0.0f, 2019.0f });
}

ARoyalRaceway::~ARoyalRaceway()
{

}

void ARoyalRaceway::BeginPlay()
{
	ABaseMap::BeginPlay();
}

void ARoyalRaceway::Tick(float _deltaTime)
{
	ABaseMap::Tick(_deltaTime);
}

void ARoyalRaceway::InitItem()
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
}