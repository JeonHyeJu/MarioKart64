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

	const FTransform& trfmPlayer = Player->GetTransform();
	const FTransform& trfmObj = TestMapPtr->GetTransform();

	const float GRAVITY_FORCE = -500.f;
	float gravityForce = GRAVITY_FORCE * _deltaTime;

	int navIdx = TestMapPtr->GetNavIndex();
	if (navIdx == -1)
	{
		Player->AddRelativeLocation({ 0.f, gravityForce, 0.f });
		CheckCollisionOfAllMap();
		return;
	}

	bool isCollided = false;
	float fDist = 0.f;
	const std::vector<NavData> navDatas = TestMapPtr->GetNavData();
	const NavData& nd = navDatas[navIdx];
	isCollided = nd.Intersects(trfmPlayer.Location, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDist);

	if (!isCollided)
	{
		for (int linkedIdx : nd.LinkData)
		{
			isCollided = navDatas[linkedIdx].Intersects(trfmPlayer.Location, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDist);
			if (isCollided)
			{
				OutputDebugStringA(("######## Seconds coliided idx: " + std::to_string(linkedIdx) + "\n").c_str());
				TestMapPtr->SetNavIndex(linkedIdx);
				navIdx = linkedIdx;
				break;
			}
		}
	}

	if (isCollided)
	{
		Player->AddActorLocation({ 0.f, fDist, 0.f });
	}
	else
	{
		FTransform trfmFuture = trfmPlayer;
		trfmFuture.Location.Y += gravityForce;

		float fDistTemp = 0.f;
		isCollided = navDatas[navIdx].Intersects(trfmFuture.Location, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDistTemp);
		if (isCollided)
		{
			OutputDebugStringA(("!!!!!!!!!! Seconds coliided idx: " + std::to_string(navIdx) + "\n").c_str());
		}
		else
		{
			for (int linkedIdx : nd.LinkData)
			{
				isCollided = navDatas[linkedIdx].Intersects(trfmFuture.Location, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDistTemp);
				if (isCollided)
				{
					OutputDebugStringA(("@@@@@@@@@ Seconds coliided idx: " + std::to_string(linkedIdx) + "\n").c_str());
					TestMapPtr->SetNavIndex(linkedIdx);
					navIdx = linkedIdx;
					break;
				}
			}
		}

		if (isCollided)
		{
			OutputDebugStringA(("gravityForce: " + std::to_string(gravityForce) + ", fDistTemp: " + std::to_string(fDistTemp) + "\n").c_str());
			Player->AddRelativeLocation({ 0.f, gravityForce + fDistTemp, 0.f });
		}
		else
		{
			Player->AddRelativeLocation({ 0.f, gravityForce, 0.f });
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