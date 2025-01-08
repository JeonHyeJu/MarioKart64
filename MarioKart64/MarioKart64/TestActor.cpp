#include "PreCompile.h"
#include "TestActor.h"
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/CameraActor.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/Collision.h>
#include "ColorRenderer.h"
#include "TriangleRenderer.h"
#include "LineRenderer.h"

TestActor::TestActor()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	/*Player = CreateDefaultSubObject<USpriteRenderer>();
	Player->SetAutoScale(false);
	Player->SetSprite("Mario.png", 0);*/

	Player = CreateDefaultSubObject<ColorRenderer>();
	Player->SetColor({1.f, 1.f, 1.f, 1.f});

	Player->SetupAttachment(RootComponent);
	//Player->SetAutoScaleRatio(5.0f);
	Player->SetScale3D({ 200.f, 200.f, 1.f });
	Player->AddWorldLocation({ -10.f, 500.f, 1.f });

	Triangle = CreateDefaultSubObject<TriangleRenderer>();
	Triangle->SetupAttachment(RootComponent);
	Triangle->SetRelativeScale3D({1000.0f, 1000.0f, 1.0f});
	Triangle->SetRotation({ 90.f, 0.f, 20.f });

	FVector playerScale = Player->GetWorldScale3D();
	Line = CreateDefaultSubObject<LineRenderer>();
	Line->SetupAttachment(RootComponent);

	GetWorld()->GetMainCamera()->AddActorRotation({ -45.f, 0.f, 0.f });
	GetWorld()->GetMainCamera()->AddActorLocation({ 0.f, 100.f, 0.f });

	//GetWorld()->GetMainCamera()->AttachToActor(this);
	//GetWorld()->GetMainCamera()->GetCameraComponent()->SetZSort(0, true);
}

TestActor::~TestActor()
{
}

void TestActor::BeginPlay()
{
	AActor::BeginPlay();
}

void TestActor::Tick(float _DeltaTime)
{
	AActor::Tick(_DeltaTime);

	float fDist = 0.f;
	FVector playerScale = Player->GetWorldScale3D();
	float halfY = playerScale.Y * .5f + 1.f;
	FTransform& playerTrfm = Player->GetTransformRef();
	DirectX::XMVECTOR layOrg = playerTrfm.Location.DirectVector;

	FVector dirVec = playerTrfm.GetWorldUp();
	dirVec.Y *= -1;
	DirectX::XMVECTOR layDst = dirVec.DirectVector;

	// Temp
	FTransform& triTrfm = Triangle->GetTransformRef();
	std::vector<FEngineVertex> vertices;
	vertices.resize(3);
	vertices[0] = FEngineVertex{ FVector(-0.5f, -0.5f, 0.0f), {0.0f , 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} };
	vertices[1] = FEngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {1.0f , 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} };
	vertices[2] = FEngineVertex{ FVector(0.5f, 0.5f, 0.0f), {0.0f , 1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} };

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	FTransform camTrfm = Camera->GetTransform();

	DirectX::XMVECTOR v1 = (vertices[0].POSITION * triTrfm.ScaleMat * triTrfm.RotationMat * triTrfm.LocationMat).DirectVector;
	DirectX::XMVECTOR v2 = (vertices[1].POSITION * triTrfm.ScaleMat * triTrfm.RotationMat * triTrfm.LocationMat).DirectVector;
	DirectX::XMVECTOR v3 = (vertices[2].POSITION * triTrfm.ScaleMat * triTrfm.RotationMat * triTrfm.LocationMat).DirectVector;

	DirectX::XMVECTOR PlayerLay = DirectX::XMVectorSubtract(layDst, layOrg);
	DirectX::XMVECTOR normalVec = DirectX::XMVector4Cross(v3, v2, v1);
	DirectX::XMVECTOR angle = DirectX::XMVector2AngleBetweenVectors(PlayerLay, normalVec);
	DirectX::XMFLOAT3 angleData;
	DirectX::XMStoreFloat3(&angleData, angle);

	const float R2D = 180.f / 3.14f;
	//OutputDebugStringA(("angleData.x: " + std::to_string(angleData.x * R2D) + ", " + std::to_string(angleData.y * R2D) + ", " + std::to_string(angleData.z * R2D) + "\n").c_str());

	bool isCollided = DirectX::TriangleTests::Intersects(layOrg, layDst, v1, v2, v3, fDist);

	std::string log = "";
	if (isCollided)
	{
		log = "isCollided : TRUE.. fDist: " + std::to_string(fDist) + "\n";
	}
	else
	{
		log = "isCollided : FALSE.. fDist: " + std::to_string(fDist) + "\n";
	}
	OutputDebugStringA(log.c_str());

	UEngineCore::GetMainWindow().GetMousePos();

	if (UEngineInput::IsPress('A'))
	{
		Player->AddRelativeLocation(FVector{ -100.0f * _DeltaTime, 0.0f, 0.0f });
	}
	if (UEngineInput::IsPress('D'))
	{
		Player->AddRelativeLocation(FVector{ 100.0f * _DeltaTime, 0.0f, 0.0f });
	}

	FVector Test = GetActorForwardVector();

	if (UEngineInput::IsPress('W'))
	{
		Player->AddRelativeLocation(FVector{ 0.0f, 0.f, 100.0f * _DeltaTime });
	}

	if (UEngineInput::IsPress('S'))
	{
		Player->AddRelativeLocation(FVector{ 0.0f, 0.0f, -100.0f * _DeltaTime });
	}

	if (UEngineInput::IsPress('Q'))
	{
		Player->AddRelativeLocation({ 0.f, -100.f * _DeltaTime, 0.f });
	}

	if (UEngineInput::IsPress('F'))
	{
		//LogoRenderer->ColorData.MulColor = float4(1.0f, 0.0f, 0.0f, 1.0f);
	}

	if (UEngineInput::IsPress('E'))
	{
		Player->AddRelativeLocation({ 0.f, 100.f * _DeltaTime, 0.f });
	}

	if (UEngineInput::IsPress('R'))
	{
		/*LogoRenderer->ColorData.PlusColor -= float4(1.0f, 1.0f, 1.0f, 1.0f) * _DeltaTime;
		LogoRenderer->ColorData.PlusColor.W -= _DeltaTime;*/

		// Child->SetWorldLocation(FVector{ 100.0f, 0.0f , 0.0f });
	}

	const float BASE_DIST = halfY;
	const float GRAVITY_FORCE = -500.f;
	if (isCollided)
	{
		if (fDist > BASE_DIST)
		{
			float downVal = fDist - BASE_DIST;
			if (downVal < GRAVITY_FORCE)
			{
				Player->AddRelativeLocation({ 0.f, downVal, 0.f });
			}
			else
			{
				Player->AddRelativeLocation({ 0.f, GRAVITY_FORCE * _DeltaTime, 0.f });
			}
		}
		else
		{
			float upVal = BASE_DIST - fDist;
			Player->AddRelativeLocation({ 0.f, upVal * _DeltaTime, 0.f });
		}
	}
	else
	{
		Player->AddRelativeLocation({ 0.f, GRAVITY_FORCE * _DeltaTime, 0.f });
	}

	float dirHalf = (playerScale.Y * .25f + 1.f * dirVec.Y) * -1.f;
	Line->SetScale3D({ 1.f, playerScale.Y * .5f, 0.0f });
	Line->SetRelativeLocation(playerTrfm.Location + FVector{ 0.f, dirHalf, -1.f });
}