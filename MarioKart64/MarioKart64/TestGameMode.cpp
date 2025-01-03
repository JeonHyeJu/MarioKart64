#include "PreCompile.h"
#include "TestGameMode.h"
#include "TestActor.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/imgui.h>

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
}

ATestGameMode::~ATestGameMode()
{
}

void ATestGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}