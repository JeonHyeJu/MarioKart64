#include "PreCompile.h"
#include "PlayGameMode.h"
#include "TestMap.h"
#include "Player.h"
#include "Skybox.h"
#include <EngineCore/CameraActor.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/EngineCamera.h>
#include <EngineBase/EngineMath.h>

APlayGameMode::APlayGameMode()
{
	Skybox = GetWorld()->SpawnActor<ASkybox>();
	TestMapPtr = GetWorld()->SpawnActor<ATestMap>();
	Player = GetWorld()->SpawnActor<APlayer>();
	Player->SetActorLocation({ 0.0f, 100.0f, 0.0f });

	TestMapPtr->SetActorRelativeScale3D({ 2.f, 2.f, 2.f });
	TestMapPtr->SetActorLocation({ 60.0f, 0.f, 0.f });

	Camera = GetWorld()->GetMainCamera();
	Camera->GetCameraComponent()->SetZSort(1, true);

	Camera->AddActorRotation({ 10.f, 0.f, 0.f });
	Camera->AddRelativeLocation({ 0.f, 0.f, -500.f });
	Camera->AttachToActor(Player.get());
}

APlayGameMode::~APlayGameMode()
{
}

void APlayGameMode::BeginPlay()
{
	AActor::BeginPlay();
}

void APlayGameMode::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	int testNum = 0;
	if (testNum++ % 100 == 0)
	{
		testNum = 0;

		float fDist = 0.f;
		const FTransform& trfm = Player->GetTransform();
		DirectX::XMVECTOR layOrg = trfm.Location.DirectVector;

		float width = 39.f;	// temp
		FVector dirVec = Player->GetActorUpVector();
		dirVec.Y *= -1;
		DirectX::XMVECTOR layDst = dirVec.DirectVector;
		
		const std::vector<FEngineVertex>& road = TestMapPtr->GetRoadVertecies();

		//OutputDebugStringA(("road size: " + std::to_string(road.size()) + "\n").c_str());

		// asume orginized meshes..
		bool isCollided = false;
		for (size_t i = 0, size = road.size()-3; i < size; i+=3)
		{
			DirectX::XMVECTOR v1 = road[i].POSITION.DirectVector;
			DirectX::XMVECTOR v2 = road[i+1].POSITION.DirectVector;
			DirectX::XMVECTOR v3 = road[i+2].POSITION.DirectVector;
			bool res = DirectX::TriangleTests::Intersects(layOrg, layDst, v1, v2, v3, fDist);
			isCollided |= res;
		}

		if (isCollided)
		{
			//OutputDebugStringA(("isCollided : TRUE.. fDist: " + std::to_string(fDist) + "\n").c_str());
		}
		else
		{
			//OutputDebugStringA(("isCollided : FALSE.. fDist: " + std::to_string(fDist) + "\n").c_str());
		}
	}

	const FTransform& trfmPlayer = Player->GetTransform();
	const FTransform& trfmCamera = Camera->GetTransform();

	/*{
		std::string playerLog = "Player Loc: " + std::to_string(trfmPlayer.Location.X) + ", " + std::to_string(trfmPlayer.Location.Y) + ", " + std::to_string(trfmPlayer.Location.Z);
		std::string cameraLog = "Camera Loc: " + std::to_string(trfmCamera.Location.X) + ", " + std::to_string(trfmCamera.Location.Y) + ", " + std::to_string(trfmCamera.Location.Z);
		OutputDebugStringA((playerLog + "\n" + cameraLog + "\n").c_str());
	}*/

	float speed = 1000.f;

	if (UEngineInput::IsPress('R'))
	{
		Camera->AddActorRotation(FVector{ -180.f * _deltaTime, 0.f, 0.f });
	}
	if (UEngineInput::IsPress('F'))
	{
		Camera->AddActorRotation(FVector{ 180.f * _deltaTime, 0.f, 0.f });
	}
	if (UEngineInput::IsPress('W'))
	{
		Camera->AddRelativeLocation({ 0.f, 0.f, speed * _deltaTime, 1.0f });
	}
	if (UEngineInput::IsPress('S'))
	{
		Camera->AddRelativeLocation({ 0.f, 0.f, -speed * _deltaTime, 1.0f });
	}
	if (UEngineInput::IsPress('A'))
	{
		Camera->AddRelativeLocation({ -speed * _deltaTime, 0.0f, 0.f, 1.0f });
	}
	if (UEngineInput::IsPress('D'))
	{
		Camera->AddRelativeLocation({ speed * _deltaTime, 0.0f, 0.f, 1.0f });
	}
	if (UEngineInput::IsPress('Q'))
	{
		Camera->AddActorRotation({ 0.f, -speed * _deltaTime, 0.f, 1.0f });
	}
	if (UEngineInput::IsPress('E'))
	{
		Camera->AddActorRotation({ 0.f, speed * _deltaTime, 0.f, 1.0f });
	}
}
