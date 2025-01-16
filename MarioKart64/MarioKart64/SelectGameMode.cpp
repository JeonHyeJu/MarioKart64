#include "PreCompile.h"
#include "SelectGameMode.h"
#include "SelectScene.h"
#include <EngineCore/CameraActor.h>

ASelectGameMode::ASelectGameMode()
{
	SelectScene = GetWorld()->SpawnActor<ASelectScene>();

	std::shared_ptr<ACameraActor> camera = GetWorld()->GetMainCamera();
	camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f });
}

ASelectGameMode::~ASelectGameMode()
{

}

void ASelectGameMode::BeginPlay()
{
	AActor::BeginPlay();
}

void ASelectGameMode::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
}