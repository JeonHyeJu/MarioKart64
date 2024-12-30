#include "PreCompile.h"
#include "Pawn.h"

APawn::APawn()
{
}

APawn::~APawn()
{
}

void APawn::BeginPlay()
{
	AActor::BeginPlay();
}

void APawn::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
}
