#include "PreCompile.h"
#include "WarioStadium.h"

AWarioStadium::AWarioStadium()
{
	MapType = ECircuit::WARIO_STADIUM;
	MinimapInitLoc = { -64.f, 0.f, -120.f };

	StartPosition = {
		{ 5100.0f, 10.0f, -100.0f },
		{ 4900.0f, 10.0f, -100.0f },
		{ 5100.0f, 10.0f, -300.0f },
		{ 4900.0f, 10.0f, -300.0f },
		{ 5100.0f, 10.0f, -500.0f },
		{ 4900.0f, 10.0f, -500.0f }
	};

	Init();

	Scale = 1000.f;
	SetActorRotation({ 0.f, 180.f, 0.f });
	SetActorRelativeScale3D({ Scale, Scale, Scale });

	// Item locations. for debug
	//Player->SetActorLocation({ -3405.0f, 10.0f, 660.0f });
	//Player->SetActorRotation({ 0.f, 180.f, 0.f });

	//Player->SetActorLocation({ 1677.0f, 10.0f, 7043.0f });
	//Player->SetActorRotation({ 0.f, 90.f, 0.f });

	//Player->SetActorLocation({ 18.0f, 10.0f, -180.0f });
	//Player->SetActorRotation({ 0.f, 180.f, 0.f });
}

AWarioStadium::~AWarioStadium()
{

}

void AWarioStadium::BeginPlay()
{
	ABaseMap::BeginPlay();
}

void AWarioStadium::Tick(float _deltaTime)
{
	ABaseMap::Tick(_deltaTime);
}

void AWarioStadium::InitItem()
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
}