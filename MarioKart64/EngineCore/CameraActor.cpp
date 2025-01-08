#include "PreCompile.h"
#include "CameraActor.h"
#include "EngineCamera.h"
#include <EnginePlatform/EngineInput.h>

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

	ScreenPos = UEngineCore::GetMainWindow().GetMousePos();
	FVector ScreenDir = PrevScreenPos - ScreenPos;
	ScreenDir.Normalize();
	PrevScreenPos = ScreenPos;

	if (UEngineInput::IsDown('C'))
	{
		FreeCameraSwitch();
	}

	if (true == IsFreeCameraValue)
	{
		if (UEngineInput::IsDown('O'))
		{
			switch (CameraComponent->ProjectionType)
			{
			case EProjectionType::Perspective:
				CameraComponent->ProjectionType = EProjectionType::Orthographic;
				break;
			case EProjectionType::Orthographic:
				CameraComponent->ProjectionType = EProjectionType::Perspective;
				break;
			default:
				break;
			}
		}

		float Speed = FreeSpeed;

		if (UEngineInput::IsPress(VK_LSHIFT))
		{
			Speed *= 5.0f;
		}

		if (UEngineInput::IsPress('A'))
		{
			AddActorLocation(-GetActorTransform().GetWorldRight() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress('D'))
		{
			AddActorLocation(GetActorTransform().GetWorldRight() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress('W'))
		{
			AddActorLocation(GetActorTransform().GetWorldFoward() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress('S'))
		{
			AddActorLocation(-GetActorTransform().GetWorldFoward() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress('Q'))
		{
			AddActorLocation(GetActorTransform().GetWorldUp() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress('E'))
		{
			AddActorLocation(-GetActorTransform().GetWorldUp() * _DeltaTime * Speed);
		}

		if (UEngineInput::IsPress(VK_RBUTTON))
		{
			AddActorRotation({ -ScreenDir.Y * RotSpeed * _DeltaTime, -ScreenDir.X * RotSpeed * _DeltaTime });
		}
	}

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



void ACameraActor::FreeCameraOn()
{
	IsFreeCameraValue = true;
	FreeCameraCheck();
}

void ACameraActor::FreeCameraOff()
{
	IsFreeCameraValue = false;
}

void ACameraActor::FreeCameraSwitch()
{
	IsFreeCameraValue = !IsFreeCameraValue;
	FreeCameraCheck();
}

void ACameraActor::FreeCameraCheck()
{
	if (true == IsFreeCameraValue)
	{
		PrevTrans = GetActorTransform();
		PrevProjectionType = GetCameraComponent()->ProjectionType;
		GetCameraComponent()->ProjectionType = EProjectionType::Perspective;
	}
	else 
	{
		SetActorTransform(PrevTrans);
		GetCameraComponent()->ProjectionType = PrevProjectionType;
	}
}