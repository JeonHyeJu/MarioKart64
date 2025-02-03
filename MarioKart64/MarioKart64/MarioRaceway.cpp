#include "PreCompile.h"
#include "MarioRaceway.h"

AMarioRaceway::AMarioRaceway()
{
	MapType = ECircuit::MARIO_RACEWAY;
	MinimapInitLoc = { -250.f, 0.f, -55.f };

	StartPosition = {
		{ -100.f, 10.0f, 800.0f },
		{ 100.f, 10.0f, 800.0f },
		{ -100.f, 10.0f, 600.0f },
		{ 100.f, 10.0f, 600.0f },
		{ -100.f, 10.0f, 400.0f },
		{ 100.f, 10.0f, 400.0f }
	};

	Init();

	Scale = 4500.f;
	SetActorRotation({ 0.f, 180.f, 0.f });
	SetActorRelativeScale3D({ Scale, Scale, Scale });

	// Item locations. for debug
	//Player->SetActorLocation({ 1734.f, 10.0f, 4060.0f });

	//Player->SetActorLocation({ 10238.f, 10.0f, 318.0f });

	//Player->SetActorLocation({ 8657.f, 10.0f, -4202.0f });
}

AMarioRaceway::~AMarioRaceway()
{

}

void AMarioRaceway::BeginPlay()
{
	ABaseMap::BeginPlay();
}

void AMarioRaceway::Tick(float _deltaTime)
{
	ABaseMap::Tick(_deltaTime);
}

void AMarioRaceway::InitItem()
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
}