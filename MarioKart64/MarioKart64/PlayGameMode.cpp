#include "PreCompile.h"
#include "PlayGameMode.h"
#include "TestMap.h"


APlayGameMode::APlayGameMode()
{
	TestMapPtr = GetWorld()->SpawnActor<ATestMap>();
}

APlayGameMode::~APlayGameMode()
{

}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();
}

void APlayGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}
