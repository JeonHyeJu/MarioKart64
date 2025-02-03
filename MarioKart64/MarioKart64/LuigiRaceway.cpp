#include "PreCompile.h"
#include "LuigiRaceway.h"

ALuigiRaceway::ALuigiRaceway()
{
	MapType = ECircuit::LUIGI_RACEWAY;
	MinimapInitLoc = { 50.f, 0.f, -200.f };

	StartPosition = {
		{ -650.0f, -201.0f, 700.0f },
		{ -470.0f, -201.0f, 700.0f },
		{ -650.0f, -201.0f, 500.0f },
		{ -470.0f, -201.0f, 500.0f },
		{ -650.0f, -201.0f, 300.0f },
		{ -470.0f, -201.0f, 300.0f }
	};

	Init();

	Scale = 4000.f;
	SetActorLocation({ 0.0f, 0.f, 0.f });
	SetActorRotation({ 0.f, 180.f, 0.f });
	SetActorRelativeScale3D({ Scale, Scale, Scale });

	// Item locations. for debug
	//Player->SetActorLocation({ -400.0f, -60.0f, 7000.0f });

	//Player->SetActorLocation({ -1200.f, -782.f, 2641.f });
	//Player->SetActorRotation({ 0.f, 180.f, 0.f });

	//Player->SetActorLocation({ -3800.0f, -60.0f, -6600.f });
	//Player->SetActorRotation({ 0.f, 90.f, 0.f });
}

ALuigiRaceway::~ALuigiRaceway()
{

}

void ALuigiRaceway::BeginPlay()
{
	ABaseMap::BeginPlay();
}

void ALuigiRaceway::Tick(float _deltaTime)
{
	ABaseMap::Tick(_deltaTime);
}

void ALuigiRaceway::InitItem()
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
	ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, 2427.f }, 0.f, nullptr });
	ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 1, y, 2427.f }, 0.f, nullptr });
	ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 2, y, 2427.f }, 0.f, nullptr });
	ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 3, y, 2427.f }, 0.f, nullptr });
	ItemBoxes.emplace_back(SItemInfo{ FVector{ initX + x * 4, y, 2427.f }, 0.f, nullptr });

	float initZ = -6580.f;
	initX = -3479.f;
	x = -56.f;
	y = -160.f;
	ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y, initZ }, 0.f, nullptr });
	ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y + 15 * 1, initZ + x * 1 }, 0.f, nullptr });
	ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y + 15 * 2, initZ + x * 2 }, 0.f, nullptr });
	ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y + 15 * 3, initZ + x * 3 }, 0.f, nullptr });
	ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y + 15 * 4, initZ + x * 4 }, 0.f, nullptr });
	ItemBoxes.emplace_back(SItemInfo{ FVector{ initX, y + 15 * 5, initZ + x * 5 }, 0.f, nullptr });
}