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
	Player->SetActorLocation({ 0.0f, 100.0f, 1000.0f });

	TestMapPtr->SetActorRelativeScale3D({ 2.f, 2.f, 2.f });
	TestMapPtr->SetActorLocation({ 60.0f, 0.f, 0.f });

	Camera = GetWorld()->GetMainCamera();
	Camera->GetCameraComponent()->SetZSort(1, true);

	Camera->AddActorRotation({ 0.f, 0.f, 0.f });
	Camera->AddRelativeLocation({ 0.f, 200.f, -500.f });
	Camera->AttachToActor(Player.get());

	CheckCollisionOfAllMap();
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
	//OutputDebugStringA(("fps: " + std::to_string(1.f / _deltaTime) + "\n").c_str());

	AActor::Tick(_deltaTime);

	CheckCollision(_deltaTime);

	// temp. for test
	if (UEngineInput::IsPress('R'))
	{
		TestMapPtr->AddActorRotation({ -1.f, 0.f, 0.f });
	}
	else if (UEngineInput::IsPress('T'))
	{
		TestMapPtr->AddActorRotation({ 1.f, 0.f, 0.f });
	}
}

void APlayGameMode::CheckCollision(float _deltaTime)
{
	FTransform trfmPlayer = Player->GetActorTransform();
	FTransform trfmObj = TestMapPtr->GetActorTransform();

	const float GRAVITY_FORCE = -500.f;
	float gravityForce = GRAVITY_FORCE * _deltaTime;

	int navIdx = TestMapPtr->GetNavIndex();
	if (navIdx == -1)
	{
		Player->AddActorLocation({ 0.f, gravityForce, 0.f });
		CheckCollisionOfAllMap();
		return;
	}

	bool isCollided = false;
	float fDist = 0.f;
	const std::vector<NavData> navDatas = TestMapPtr->GetNavData();
	NavData nd = navDatas[navIdx];
	isCollided = nd.Intersects(trfmPlayer.Location, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDist);

	if (!isCollided)
	{
		for (int linkedIdx : nd.LinkData)
		{
			isCollided = navDatas[linkedIdx].Intersects(trfmPlayer.Location, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDist);
			if (isCollided)
			{
				OutputDebugStringA(("[First coliided idx]: " + std::to_string(linkedIdx) + "\n").c_str());
				TestMapPtr->SetNavIndex(linkedIdx);
				navIdx = linkedIdx;
				break;
			}
		}
	}

	// Temporary gravity
	if (isCollided)
	{
		Player->AddActorLocation({ 0.f, fDist, 0.f });
	}
	else
	{
		FTransform trfmFuture = trfmPlayer;
		trfmFuture.Location.Y += gravityForce;

		float fDistTemp = 0.f;
		nd = navDatas[navIdx];
		isCollided = nd.Intersects(trfmFuture.Location, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDistTemp);
		if (isCollided)
		{
			OutputDebugStringA(("[Second coliided idx]: " + std::to_string(navIdx) + "\n").c_str());
		}
		else
		{
			for (int linkedIdx : nd.LinkData)
			{
				isCollided = navDatas[linkedIdx].Intersects(trfmFuture.Location, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDistTemp);
				if (isCollided)
				{
					OutputDebugStringA(("[Third coliided idx]: " + std::to_string(linkedIdx) + "\n").c_str());
					TestMapPtr->SetNavIndex(linkedIdx);
					navIdx = linkedIdx;
					break;
				}
			}
		}

		if (isCollided)
		{
			OutputDebugStringA(("- gravityForce: " + std::to_string(gravityForce) + ", fDistTemp: " + std::to_string(fDistTemp) + "\n").c_str());
			Player->AddActorLocation({ 0.f, gravityForce + fDistTemp, 0.f });
		}
		else
		{
			Player->AddActorLocation({ 0.f, gravityForce, 0.f });
		}
	}

	// Temporary physics of slope
	if (isCollided)
	{
		nd = navDatas[navIdx];

		TestMapPtr->GetActorTransform();
		nd.Vertex[0] = nd.Vertex[0] * trfmObj.ScaleMat * trfmObj.RotationMat * trfmObj.LocationMat;
		nd.Vertex[1] = nd.Vertex[1] * trfmObj.ScaleMat * trfmObj.RotationMat * trfmObj.LocationMat;
		nd.Vertex[2] = nd.Vertex[2] * trfmObj.ScaleMat * trfmObj.RotationMat * trfmObj.LocationMat;
		float4 v1 = nd.Vertex[2] - nd.Vertex[0];
		float4 v2 = nd.Vertex[1] - nd.Vertex[0];
		v1.Normalize();
		v2.Normalize();

		FVector normalV = FVector::Cross(v1, v2);
		normalV.Normalize();
		FVector playerUp = trfmPlayer.GetWorldUp();
		FVector dirV = playerUp - normalV;
		float angle = FVector::GetVectorAngleDeg(normalV, playerUp);

		OutputDebugStringA(("- angle: " + std::to_string(angle) + "\n").c_str());

		if (angle > 1)
		{
			Player->AddActorLocation(-dirV * angle);
		}
		else if (angle < -1)
		{
			Player->AddActorLocation(dirV * angle);
		}
	}
}

void APlayGameMode::CheckCollisionOfAllMap()
{
	const FTransform& tfrmPlayer = Player->GetTransform();
	const std::vector<NavData>& navDatas = TestMapPtr->GetNavData();

	// TODO: Important.. This doesn't take into account children
	for (size_t i = 0, size = navDatas.size(); i < size; ++i)
	{
		const FTransform& trfmObj = TestMapPtr->GetTransform();
		const NavData& nd = navDatas[i];

		float fDist = 0.f;
		bool isCollided = nd.Intersects(tfrmPlayer.Location, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDist);
		if (isCollided)
		{
			TestMapPtr->SetNavIndex(nd.Index);
			OutputDebugStringA(("CheckCollisionAll index: " + std::to_string(nd.Index) + "\n").c_str());
			break;
		}
	}
}