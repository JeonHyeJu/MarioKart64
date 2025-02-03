#include "PreCompile.h"
#include "BowserCastle.h"

ABowserCastle::ABowserCastle()
{
	MapType = ECircuit::BOWSERS_CASTLE;
	MinimapInitLoc = { -196.f, 0.f, -200.f };

	StartPosition = {
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f },
		{ 0.f, 0.f, 0.f }
	};

	Init();

	Scale = 4000.f;
	SetActorRotation({ 0.f, 180.f, 0.f });
	SetActorRelativeScale3D({ Scale, Scale, Scale });

	// Item locations. for debug
	//Player->SetActorLocation({ -82.f, 0.f, 6059.f });

	//Player->SetActorLocation({ 4909.f, 0.f, 3198.f });

	//Player->SetActorLocation({ 4838.f, 0.f, -1705.f });
	//Player->SetActorRotation({ 0.f, -90.f, 0.f });
}

ABowserCastle::~ABowserCastle()
{

}

void ABowserCastle::BeginPlay()
{
	ABaseMap::BeginPlay();
}

void ABowserCastle::Tick(float _deltaTime)
{
	ABaseMap::Tick(_deltaTime);
}

void ABowserCastle::InitItem()
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
}