#include "PreCompile.h"
#include "TestGameMode.h"
#include "TestActor.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/EngineCamera.h>
#include <EnginePlatform/EngineInput.h>

ATestGameMode::ATestGameMode()
{
	{
		Logo = GetWorld()->SpawnActor<TestActor>();
		// Logo->SetActorLocation({ 300.0f, 0.0f, 0.0f });
	}
	
	// {
	//	std::shared_ptr<ATitleLogo> Logo = GetWorld()->SpawnActor<ATitleLogo>();
	//	Logo->SetActorLocation({ 300.0f, 0.0f, 0.0f });
	//	Logo->GetRenderer()->SetSpriteData({ 0.0f, 0.0f }, {0.02f, 0.02f});
	// }

	// {
	//	std::shared_ptr<ATitleLogo> Logo = GetWorld()->SpawnActor<ATitleLogo>();
	//	Logo->SetActorLocation({ -300.0f, 0.0f, 0.0f });
	// }

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });
	Camera->GetCameraComponent()->SetZSort(0, true);
}

ATestGameMode::~ATestGameMode()
{
}

void ATestGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	if (UEngineInput::IsPress('C'))
	{
		GetWorld()->GetMainCamera()->FreeCameraSwitch();
	}

//	if (nullptr == Logo && true == Logo->IsDestroy())
//	{
//		Logo = nullptr;
//	}
}