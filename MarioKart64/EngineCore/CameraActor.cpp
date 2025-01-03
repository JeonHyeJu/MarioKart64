#include "PreCompile.h"
#include "CameraActor.h"
#include "EngineCamera.h"

ACameraActor::ACameraActor()
{
	CameraComponent = CreateDefaultSubObject<UEngineCamera>();
	RootComponent = CameraComponent;
}

ACameraActor::~ACameraActor()
{
	CameraComponent = nullptr;
}

void ACameraActor::BeginPlay()
{
	AActor::BeginPlay();
}

void ACameraActor::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);
	CameraComponent->CalculateViewAndProjection();
}


FVector ACameraActor::ScreenMousePosToWorldPosWithOutPos()
{
	return FVector();
}

FVector ACameraActor::ScreenMousePosToWorldPos()
{
	FVector Size = UEngineCore::GetMainWindow().GetWindowSize();
	FVector MousePos = UEngineCore::GetMainWindow().GetMousePos();

	float4x4 Mat;
	Mat.ViewPort(Size.X, Size.Y, 0.0f, 0.0f, 0.0f, 1.0f);

	FTransform CameraTransform = GetActorTransform();

	MousePos = MousePos * Mat.InverseReturn();
	MousePos = MousePos * CameraTransform.Projection.InverseReturn();
	MousePos = MousePos * CameraTransform.View.InverseReturn();

	return MousePos;
}

