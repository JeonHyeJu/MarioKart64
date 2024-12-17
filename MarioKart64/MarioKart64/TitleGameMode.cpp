#include "PreCompile.h"
#include "TitleGameMode.h"
#include "TitleLogo.h"

ATitleGameMode::ATitleGameMode()
{
	GetWorld()->SpawnActor<ATitleLogo>();
}

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}