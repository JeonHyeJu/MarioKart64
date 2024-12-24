#include "PreCompile.h"
#include "TitleGameMode.h"
#include "TitleLogo.h"
#include <EngineCore/CameraActor.h>
#include <EngineCore/SpriteRenderer.h>

ATitleGameMode::ATitleGameMode()
{
	{
		Logo = GetWorld()->SpawnActor<ATitleLogo>();
		Logo->SetActorLocation({ 300.0f, 0.0f, 0.0f });
		Logo->GetRenderer()->SetSpriteData(4);
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

ATitleGameMode::~ATitleGameMode()
{
}

void ATitleGameMode::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	static float Time = 1.0f;
	static int Index = 0;

	Logo->GetRenderer()->SetSpriteData(Index);

	Time -= 0.0001;

	if (0.0f >= Time)
	{
		++Index;
		Time = 1.0f;
	}
}