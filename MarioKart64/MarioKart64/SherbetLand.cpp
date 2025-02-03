#include "PreCompile.h"
#include "SherbetLand.h"

ASherbetLand::ASherbetLand()
{
	MapType = ECircuit::SHERBET_LAND;
	MinimapInitLoc = { -53.f, 0.f, -58.f };

	StartPosition = {
		{ 4719.f, 150.0f, -3392.0f },
		{ 4719.f, 150.0f, -3392.0f },
		{ 4719.f, 150.0f, -3392.0f },
		{ 4719.f, 150.0f, -3392.0f },
		{ 4719.f, 150.0f, -3392.0f },
		{ 4719.f, 150.0f, -3392.0f },
	};

	Init();

	Scale = 50.f;
	SetActorRotation({ 0.f, 180.f, 0.f });
	SetActorRelativeScale3D({ Scale, Scale, Scale });

	// Item locations. for debug
	//Player->SetActorLocation({ -7407.f, 150.0f, -2594.0f });
	//Player->SetActorRotation({ 0.f, 180.0f, 0.f });

	//Player->SetActorLocation({ -9188.f, -300.0f, -10933.0f });

	//Player->SetActorLocation({ -1849.f, 150.0f, -2965.0f });
}

ASherbetLand::~ASherbetLand()
{

}

void ASherbetLand::BeginPlay()
{
	ABaseMap::BeginPlay();
}

void ASherbetLand::Tick(float _deltaTime)
{
	ABaseMap::Tick(_deltaTime);
}

void ASherbetLand::InitItem()
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
}