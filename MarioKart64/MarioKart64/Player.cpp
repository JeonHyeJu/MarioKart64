#include "PreCompile.h"
#include "Player.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EnginePlatform/EngineInput.h>

APlayer::APlayer()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Renderer = CreateDefaultSubObject<USpriteRenderer>();
	Renderer->SetRelativeLocation({ 0.f, -90.f, -200.f });
	Renderer->SetOrder(0);

	Renderer->SetAutoScale(false);
	Renderer->ColorData.PlusColor = {1.f, 1.f, 1.f};	// Temp
	Renderer->SetRelativeScale3D({1.f, 39.f, 1.f});

	// Temp
	const int TURN_SIZE = 31;
	std::vector<int> idxs(TURN_SIZE, 0);
	std::vector<float> times(TURN_SIZE, 0.2f);
	for (size_t i = 0; i < TURN_SIZE; ++i)
	{
		idxs[i] = static_cast<int>(i * 36);
	}

	Renderer->CreateAnimation("Idle", "Mario.png", 0, 3, .3f);
	Renderer->CreateAnimation("TurnL", "Mario.png", idxs, times, true);
	Renderer->CreateAnimation("TurnR", "Mario.png", idxs, times, true);

	Renderer->ChangeAnimation("Idle");
	Renderer->SetupAttachment(RootComponent);
}

APlayer::~APlayer()
{
}

void APlayer::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
	APawn::Tick(_deltaTime);

	FVector move;
	static FVector accMove;
	bool isInverted = false;

	if (UEngineInput::IsPress(VK_UP))
	{
		if (accMove.Z < 0)
		{
			accMove.Z += .15f;
		}
		else if (accMove.Z < MAX_SPEED)
		{
			accMove.Z += .05f;
		}
		move.Z = accMove.Z;
	}
	else if (UEngineInput::IsPress(VK_DOWN))
	{
		if (accMove.Z > 0)
		{
			accMove.Z -= .15f;
		}
		else if (accMove.Z > -MAX_SPEED)
		{
			isInverted = true;
			accMove.Z -= .05f;
		}
		move.Z = accMove.Z;
	}
	else if (UEngineInput::IsPress('Z'))
	{
		move.Y += 1.f;
	}
	else if (UEngineInput::IsPress('X'))
	{
		move.Y -= 1.f;
	}
	else
	{
		if (accMove.Z > 0)
		{
			accMove.Z -= FRICTIONAL_FORCE;
		}
		else if (accMove.Z < 0)
		{
			accMove.Z += FRICTIONAL_FORCE;
		}
		move.Z = accMove.Z;
	}

	const float ROT_VAL = 30.f * _deltaTime;
	if (UEngineInput::IsPress(VK_LEFT))
	{
		//{
		//	const FTransform& trfm = GetTransform();
		//	FVector temp = trfm.Location;
		//	temp.Z = move.Z;

		//	std::string log = "1. Rot: " + std::to_string(temp.X) + ", " + std::to_string(temp.Y) + ", " + std::to_string(temp.Z) + "\n";
		//	OutputDebugStringA(log.c_str());
		//	temp.RotationYRad(-ROT_VAL);
		//	std::string log2 = "2. Rot: " + std::to_string(temp.X) + ", " + std::to_string(temp.Y) + ", " + std::to_string(temp.Z) + "\n";
		//	OutputDebugStringA(log2.c_str());
		//}

		//move.X = accMove.X;

		if (isInverted)
		{
			Renderer->ChangeAnimation("TurnR");
			Renderer->SetRotation({ 0.f, -180.f, 0.f });
			AddActorRotation({ 0.f, ROT_VAL, 0.f });
		}
		else
		{
			Renderer->ChangeAnimation("TurnL");
			Renderer->SetRotation({ 0.f, 0.f, 0.f });
			AddActorRotation({ 0.f, -ROT_VAL, 0.f });
		}
	}
	else if (UEngineInput::IsPress(VK_RIGHT))
	{
		if (accMove.X < MAX_TURN)
		{
			accMove.X += ROT_VAL;
		}

		//move.X = accMove.X;

		if (isInverted)
		{
			Renderer->ChangeAnimation("TurnL");
			Renderer->SetRotation({ 0.f, 0.f, 0.f });
			AddActorRotation({ 0.f, -ROT_VAL, 0.f });
		}
		else
		{
			Renderer->ChangeAnimation("TurnR");
			Renderer->SetRotation({ 0.f, -180.f, 0.f });
			AddActorRotation({ 0.f, ROT_VAL, 0.f });
		}
	}
	else
	{
		Renderer->ChangeAnimation("Idle");

		accMove.X = 0.f;
		//move.X = accMove.X;
	}

	//OutputDebugStringA((std::to_string(accMove.X) + ", " + std::to_string(accMove.Z) + "\n").c_str());

	// temp
	FVector dir = GetActorForwardVector();
	dir.Normalize();

	FVector last = dir* move.Z;
	last.Y = move.Y;

	//Renderer->AddRelativeLocation(move);
	AddRelativeLocation(last);
}