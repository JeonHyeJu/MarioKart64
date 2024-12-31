#include "PreCompile.h"
#include "PlayGameMode.h"
#include "TestMap.h"
#include "Player.h"
#include <EngineCore/CameraActor.h>
#include <EnginePlatform/EngineInput.h>

APlayGameMode::APlayGameMode()
{
	TestMapPtr = GetWorld()->SpawnActor<ATestMap>();
	Player = GetWorld()->SpawnActor<APlayer>();

	TestMapPtr->SetActorRelativeScale3D({ 2.f, 2.f, 2.f });
	TestMapPtr->SetActorLocation({ 60.0f, 0.f, 0.f });

	Camera = GetWorld()->GetMainCamera();

	Camera->AddActorRotation({ 10.f, 0.f, 0.f });
	Camera->AddRelativeLocation({ 0.f, 0.f, -500.f });
	Camera->AttachToActor(Player.get());
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();
}

void APlayGameMode::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	const FTransform& trfmPlayer = Player->GetTransform();
	const FTransform& trfmCamera = Camera->GetTransform();

	/*{
		std::string playerLog = "Player Loc: " + std::to_string(trfmPlayer.Location.X) + ", " + std::to_string(trfmPlayer.Location.Y) + ", " + std::to_string(trfmPlayer.Location.Z);
		std::string cameraLog = "Camera Loc: " + std::to_string(trfmCamera.Location.X) + ", " + std::to_string(trfmCamera.Location.Y) + ", " + std::to_string(trfmCamera.Location.Z);
		OutputDebugStringA((playerLog + "\n" + cameraLog + "\n").c_str());
	}*/

	if (UEngineInput::IsPress('R'))
	{
		Camera->AddActorRotation(FVector{ -180.f * _deltaTime, 0.f, 0.f });
	}
	if (UEngineInput::IsPress('F'))
	{
		Camera->AddActorRotation(FVector{ 180.f * _deltaTime, 0.f, 0.f });
	}
	if (UEngineInput::IsPress('W'))
	{
		Camera->AddRelativeLocation({ 0.f, 0.f, 100.f * _deltaTime, 1.0f });
	}
	if (UEngineInput::IsPress('S'))
	{
		Camera->AddRelativeLocation({ 0.f, 0.f, -100.f * _deltaTime, 1.0f });
	}
	if (UEngineInput::IsPress('A'))
	{
		Camera->AddRelativeLocation({ -100.f * _deltaTime, 0.0f, 0.f, 1.0f });
	}
	if (UEngineInput::IsPress('D'))
	{
		Camera->AddRelativeLocation({ 100.f * _deltaTime, 0.0f, 0.f, 1.0f });
	}
	if (UEngineInput::IsPress('Q'))
	{
		Camera->AddActorRotation({ 0.f, -100.f * _deltaTime, 0.f, 1.0f });
	}
	if (UEngineInput::IsPress('E'))
	{
		Camera->AddActorRotation({ 0.f, 100.f * _deltaTime, 0.f, 1.0f });
	}
}
