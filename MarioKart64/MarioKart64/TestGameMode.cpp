#include "PreCompile.h"
#include "TestGameMode.h"
#include "TestActor.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/EngineGUIWindow.h>
#include <EngineCore/EngineGUI.h>
#include <EngineCore/EngineVertexBuffer.h>
#include <EngineCore/EngineCamera.h>
#include <EnginePlatform/EngineInput.h>

ATestGameMode::ATestGameMode()
{
	{
		mTestActor = GetWorld()->SpawnActor<TestActor>();
	}
	
	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	//Camera->SetActorLocation({ 0.0f, 0.0f, -1000.0f, 1.0f });

	Camera->SetActorLocation({ 0.0f, 200.0f, -1000.0f, 1.0f });
	Camera->AddActorRotation({ 45.0f, 0.0f, 0.0f, 1.0f });

	Camera->GetCameraComponent()->SetZSort(0, true);
}

ATestGameMode::~ATestGameMode()
{
}

void ATestGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

//	if (nullptr == Logo && true == Logo->IsDestroy())
//	{
//		Logo = nullptr;
//	}
}