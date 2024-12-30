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

	//Player->SetActorLocation({ 300.0f, 0.0f, 0.0f });

	Camera = GetWorld()->GetMainCamera();
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

	/*std::string playerLog = "Player: " + std::to_string(trfmPlayer.Location.X) + ", " + std::to_string(trfmPlayer.Location.Y) + ", " + std::to_string(trfmPlayer.Location.Z);
	std::string cameraLog = "Camera: " + std::to_string(trfmCamera.Location.X) + ", " + std::to_string(trfmCamera.Location.Y) + ", " + std::to_string(trfmCamera.Location.Z);
	OutputDebugStringA((playerLog + "\n" + cameraLog + "\n").c_str());*/

	Camera->SetActorLocation({ 0.0f, 100.0f, trfmPlayer.Location.Z - 500.f, 1.0f });
	//Camera->SetActorLocation({ 0.0f, 100.0f, -500.0f, 1.0f });

	if (UEngineInput::IsPress('T'))
	{
		Camera->AddRelativeLocation({ 0.f, 100.f * _deltaTime, 0.f, 1.0f });
	}
	if (UEngineInput::IsPress('G'))
	{
		Camera->AddRelativeLocation({ 0.f, -100.f * _deltaTime, 0.f, 1.0f });
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
		//AddRelativeLocation(FVector{ 100.0f * _deltaTime, 0.f, 0.f });
	}
	if (UEngineInput::IsPress('D'))
	{
		Camera->AddRelativeLocation({ 100.f * _deltaTime, 0.0f, 0.f, 1.0f });
		//AddRelativeLocation(FVector{ 100.0f * _deltaTime, 0.f, 0.f });
	}
	if (UEngineInput::IsPress('Q'))
	{
		Camera->AddActorRotation(FVector{ -180.f * _deltaTime, 0.f, 0.f });
	}
	if (UEngineInput::IsPress('E'))
	{
		Camera->AddActorRotation(FVector{ 180.f * _deltaTime, 0.f, 0.f });
	}
}
