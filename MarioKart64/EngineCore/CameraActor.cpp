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
	//OutputDebugStringA(("ACameraActor::Tick --------- " + std::to_string(_DeltaTime) + "\n").c_str());
	AActor::Tick(_DeltaTime);

	ScreenPos = UEngineCore::GetMainWindow().GetMousePos();
	FVector ScreenDir = PrevScreenPos - ScreenPos;
	ScreenDir.Normalize();
	PrevScreenPos = ScreenPos;

	if (UEngineInput::IsDown('C'))
	{
		OutputDebugString("DOWN C\n");
		//FreeCameraSwitch();
		FreeCameraOn();
	}
	else if (UEngineInput::IsDown('V'))
	{
		FreeCameraOff();
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

FVector ACameraActor::ScreenMousePosToWorldPosWithOutPos(float _PosZ)
{
	 FVector MousePos = UEngineCore::GetMainWindow().GetMousePos();

	 return ScreenPosToWorldPosWithOutPos(MousePos, _PosZ);
}

FVector ACameraActor::ScreenPosToWorldPosWithOutPos(FVector _Pos, float _PosZ)
{
	 FVector Size = UEngineCore::GetMainWindow().GetWindowSize();

	 float4x4 ViewPort;
	 ViewPort.ViewPort(Size.X, Size.Y, 0.0f, 0.0f, 0.0f, 1.0f);

	 FTransform CameraTransform = GetActorTransform();

	 float4x4 ViewMat = CameraTransform.View;
	 ViewMat.ArrVector[3] = FVector::ZERO;

	 _Pos = _Pos * ViewPort.InverseReturn();
	 _Pos = _Pos * CameraTransform.Projection.InverseReturn();
	 _Pos = _Pos * ViewMat.InverseReturn();

	 float Ratio = Size.X / Size.Y;

	 _Pos.Y *= Ratio;

	 // -668;

	 float FOV = GetCameraComponent()->FOV * 0.5f * UEngineMath::D2R;

	 // ³ôÀÌ / ¹Øº¯ 
	 FVector ZDisScreenScale;
	 ZDisScreenScale.X = tanf(FOV * Ratio) * _PosZ * _Pos.X;
	 ZDisScreenScale.Y = tanf(FOV) * _PosZ * _Pos.Y;

	 return _Pos;
}

FVector ACameraActor::ScreenPosToWorldPos(FVector _Pos)
{
	FVector Size = UEngineCore::GetMainWindow().GetWindowSize();

	float4x4 ViewPort;
	ViewPort.ViewPort(Size.X, Size.Y, 0.0f, 0.0f, 0.0f, 1.0f);

	FTransform CameraTransform = GetActorTransform();

	_Pos = _Pos * ViewPort.InverseReturn();
	_Pos = _Pos * CameraTransform.Projection.InverseReturn();
	_Pos = _Pos * CameraTransform.View.InverseReturn();

	return _Pos;
}

FVector ACameraActor::WorldPosToScreenPos(FVector _Pos)
{
	FVector Size = UEngineCore::GetMainWindow().GetWindowSize();

	float4x4 ViewPort;
	ViewPort.ViewPort(Size.X, Size.Y, 0.0f, 0.0f, 0.0f, 1.0f);

	FTransform CameraTransform = GetActorTransform();

	_Pos = _Pos * CameraTransform.View;
	_Pos = _Pos * CameraTransform.Projection;
	_Pos = _Pos * ViewPort;
	return _Pos;
}

FVector ACameraActor::ScreenMousePosToWorldPosWithOutPos()
{
	return FVector();
}

FVector ACameraActor::ScreenMousePosToWorldPos()
{
	FVector MousePos = UEngineCore::GetMainWindow().GetMousePos();

	return ScreenPosToWorldPos(MousePos);
}

void ACameraActor::FreeCameraOn()
{
	IsFreeCameraValue = true;
	FreeCameraCheck();
}

void ACameraActor::FreeCameraOff()
{
	IsFreeCameraValue = false;
	FreeCameraCheck();
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