#include "PreCompile.h"
#include "TitleLogo.h"
#include <EngineCore/Renderer.h>

ATitleLogo::ATitleLogo()
{
	LogoRenderer = CreateDefaultSubObject<URenderer>();
	SetActorRelativeScale3D({100.0f, 100.0f, 1.0f });
}

ATitleLogo::~ATitleLogo()
{
}

void ATitleLogo::BeginPlay()
{
	AActor::BeginPlay();
}

void ATitleLogo::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
}