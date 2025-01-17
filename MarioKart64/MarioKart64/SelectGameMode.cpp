#include "PreCompile.h"
#include "SelectGameMode.h"
#include "SelectGame.h"
#include "SelectCharacter.h"
#include "SelectMap.h"
#include <EngineCore/CameraActor.h>

ASelectGameMode::ASelectGameMode()
{
	SelectGame = GetWorld()->SpawnActor<ASelectGame>();
	SelectGame->SetCallback(std::bind(&ASelectGameMode::OnEndSelectGame, this));
	SelectGame->SetActive(false);

	SelectCharacter = GetWorld()->SpawnActor<ASelectCharacter>();
	SelectCharacter->SetCallback(std::bind(&ASelectGameMode::OnEndSelectCharacter, this));
	SelectCharacter->SetActive(false);
	
	SelectMap = GetWorld()->SpawnActor<ASelectMap>();
	//SelectMap->SetCallback(std::bind(&ASelectGameMode::OnEndSelectCharacter, this));

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

void ASelectGameMode::OnEndSelectGame()
{
	SelectGame->SetActive(false);
}

void ASelectGameMode::OnEndSelectCharacter()
{
	SelectCharacter->SetActive(false);
}

void ASelectGameMode::OnEndSelectMap()
{
	SelectMap->SetActive(false);
}