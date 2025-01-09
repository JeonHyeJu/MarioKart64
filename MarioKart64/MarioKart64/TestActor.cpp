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

#include "TestVertex.h"	// for test

TestActor::TestActor()
{
	//InitTestSingleTriangle();
	InitTestMultipleTriangle();
	InitNavMesh();
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

	//RunTestSingleTriangle(_DeltaTime);
	RunTestMultipleTriangle(_DeltaTime);
}

void TestActor::InitTestSingleTriangle()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	/*Player = CreateDefaultSubObject<USpriteRenderer>();
	Player->SetAutoScale(false);
	Player->SetSprite("Mario.png", 0);*/

	Player = CreateDefaultSubObject<ColorRenderer>();
	Player->SetColor({ 1.f, 1.f, 1.f, 1.f });

	Player->SetupAttachment(RootComponent);
	//Player->SetAutoScaleRatio(5.0f);
	Player->SetScale3D({ 200.f, 200.f, 1.f });
	Player->AddWorldLocation({ -10.f, 500.f, 1.f });

	Triangle = CreateDefaultSubObject<TriangleRenderer>();
	Triangle->SetupAttachment(RootComponent);
	Triangle->SetRelativeScale3D({ 1000.0f, 1000.0f, 1.0f });
	Triangle->SetRotation({ 70.f, 0.f, 20.f });

	Line = CreateDefaultSubObject<LineRenderer>();
	Line->SetupAttachment(RootComponent);

	//GetWorld()->GetMainCamera()->AttachToActor(this);
}

void TestActor::RunTestSingleTriangle(float _DeltaTime)
{
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

	std::shared_ptr<ACameraActor> Camera = GetWorld()->GetMainCamera();
	FTransform camTrfm = Camera->GetTransform();

	DirectX::XMVECTOR v1 = (SingleTriangle[0].POSITION * triTrfm.ScaleMat * triTrfm.RotationMat * triTrfm.LocationMat).DirectVector;
	DirectX::XMVECTOR v2 = (SingleTriangle[1].POSITION * triTrfm.ScaleMat * triTrfm.RotationMat * triTrfm.LocationMat).DirectVector;
	DirectX::XMVECTOR v3 = (SingleTriangle[2].POSITION * triTrfm.ScaleMat * triTrfm.RotationMat * triTrfm.LocationMat).DirectVector;

	DirectX::XMVECTOR PlayerLay = DirectX::XMVectorSubtract(layDst, layOrg);
	DirectX::XMVECTOR normalVec = DirectX::XMVector4Cross(v1, v2, v3);

	DirectX::XMFLOAT3 normalDat;
	DirectX::XMStoreFloat3(&normalDat, normalVec);

	DirectX::XMVECTOR angle = DirectX::XMVector2AngleBetweenVectors(PlayerLay, normalVec);
	DirectX::XMFLOAT3 angleData;
	DirectX::XMStoreFloat3(&angleData, angle);

	const float R2D = 180.f / 3.14f;
	//OutputDebugStringA(("angleData.x: " + std::to_string(angleData.x * R2D) + ", " + std::to_string(angleData.y * R2D) + ", " + std::to_string(angleData.z * R2D) + "\n").c_str());

	FVector Vector = layOrg;
	//Vector.Z = 0.0f;

	bool isCollided = DirectX::TriangleTests::Intersects(Vector.DirectVector, FVector::DOWN.DirectVector, v1, v2, v3, fDist);

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

	/*Player->SetRelativeLocation(pos);
	Player->AddRelativeLocation({ 0.f, GRAVITY_FORCE * _DeltaTime, 0.f });*/

	float dirHalf = (playerScale.Y * .25f + 1.f * dirVec.Y) * -1.f;
	Line->SetScale3D({ 1.f, playerScale.Y * .5f, 0.0f });
	Line->SetRelativeLocation(playerTrfm.Location + FVector{ 0.f, dirHalf, -1.f });
}

void TestActor::InitTestMultipleTriangle()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	/*Player = CreateDefaultSubObject<USpriteRenderer>();
	Player->SetAutoScale(false);
	Player->SetSprite("Mario.png", 0);*/

	Player = CreateDefaultSubObject<ColorRenderer>();
	Player->SetColor({ 1.f, 1.f, 1.f, 1.f });

	Player->SetupAttachment(RootComponent);
	//Player->SetAutoScaleRatio(5.0f);
	Player->SetScale3D({ 100.f, 100.f, 1.f });
	Player->AddWorldLocation({ 0.f, 200.f, 1.f });

	Triangle = CreateDefaultSubObject<TriangleRenderer>();
	Triangle->SetupAttachment(RootComponent);
	Triangle->SetRelativeScale3D({ 3.f, 3.f, 3.f });
	Triangle->SetRelativeLocation({ 300.f, 300.f, 1000.f });
	Triangle->SetRotation({ -10.f, 0.f, 0.f });

	FVector playerScale = Player->GetWorldScale3D();
	Line = CreateDefaultSubObject<LineRenderer>();
	Line->SetupAttachment(RootComponent);

	//GetWorld()->GetMainCamera()->AttachToActor(this);
}

void TestActor::RunTestMultipleTriangle(float _DeltaTime)
{
	float fDist = 0.f;
	FVector playerScale = Player->GetWorldScale3D();
	float halfY = playerScale.Y * .5f + 1.f;
	FTransform& playerTrfm = Player->GetTransformRef();
	FTransform& triTrfm = Triangle->GetTransformRef();

	//bool isCollided = false;
	/*for (size_t i = 0, size = MultipleTriangles.size(); i < size; i+=3)
	{
		DirectX::XMVECTOR v1 = (MultipleTriangles[i].POSITION * triTrfm.ScaleMat * triTrfm.RotationMat * triTrfm.LocationMat).DirectVector;
		DirectX::XMVECTOR v2 = (MultipleTriangles[i+1].POSITION * triTrfm.ScaleMat * triTrfm.RotationMat * triTrfm.LocationMat).DirectVector;
		DirectX::XMVECTOR v3 = (MultipleTriangles[i+2].POSITION * triTrfm.ScaleMat * triTrfm.RotationMat * triTrfm.LocationMat).DirectVector;
		isCollided = DirectX::TriangleTests::Intersects(playerTrfm.Location.DirectVector, FVector::UP.DirectVector, v1, v2, v3, fDist);

		if (isCollided)
		{
			break;
		}
	}*/

	const float BASE_DIST = halfY;
	const float GRAVITY_FORCE = -500.f;
	float gravityForce = GRAVITY_FORCE * _DeltaTime;

	if (CollidedNavIndex == -1)
	{
		Player->AddRelativeLocation({ 0.f, gravityForce, 0.f });
		CheckAllMesh();
		return;
	}

	const NavData& nd = NavDatas[CollidedNavIndex];
	bool isCollided = nd.Intersects(playerTrfm.Location, FVector::UP, triTrfm.ScaleMat, triTrfm.RotationMat, triTrfm.LocationMat, fDist);

	// for logging
	std::string log = "";
	if (isCollided)
	{
		log = "isCollided : TRUE.. fDist: " + std::to_string(fDist) + "\n";
	}
	else
	{
		log = "isCollided : FALSE.. fDist: " + std::to_string(fDist) + "\n";
	}
	//OutputDebugStringA(log.c_str());

	if (!isCollided)
	{
		for (int linkedIdx : nd.LinkData)
		{
			isCollided = NavDatas[linkedIdx].Intersects(playerTrfm.Location, FVector::UP, triTrfm.ScaleMat, triTrfm.RotationMat, triTrfm.LocationMat, fDist);
			if (isCollided)
			{
				OutputDebugStringA(("######## Seconds coliided idx: " + std::to_string(linkedIdx) + "\n").c_str());
				CollidedNavIndex = linkedIdx;
				break;
			}
		}
	}

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

	if (UEngineInput::IsPress('E'))
	{
		Player->AddRelativeLocation({ 0.f, 100.f * _DeltaTime, 0.f });
	}

	if (isCollided)
	{
		FTransform& tempTfrm = Player->GetTransformRef();
		playerTrfm.Location.Y += fDist;
		Player->SetRelativeLocation(playerTrfm.Location);
	}
	else
	{
		const FTransform& _tempFfrm = Player->GetTransform();
		FTransform asumeTfrm = _tempFfrm;
		asumeTfrm.Location.Y += gravityForce;

		float fDistTemp = 0.f;

		isCollided = NavDatas[CollidedNavIndex].Intersects(asumeTfrm.Location, FVector::UP, triTrfm.ScaleMat, triTrfm.RotationMat, triTrfm.LocationMat, fDistTemp);
		if (isCollided)
		{
			OutputDebugStringA(("!!!!!!!!!! Seconds coliided idx: " + std::to_string(CollidedNavIndex) + "\n").c_str());
		}
		else
		{
			for (int linkedIdx : nd.LinkData)
			{
				isCollided = NavDatas[linkedIdx].Intersects(asumeTfrm.Location, FVector::UP, triTrfm.ScaleMat, triTrfm.RotationMat, triTrfm.LocationMat, fDistTemp);
				if (isCollided)
				{
					OutputDebugStringA(("@@@@@@@@@ Seconds coliided idx: " + std::to_string(linkedIdx) + "\n").c_str());
					CollidedNavIndex = linkedIdx;
					break;
				}
			}
		}

		if (isCollided)
		{
			OutputDebugStringA(("gravityForce: " + std::to_string(gravityForce) + ", fDistTemp: " + std::to_string(fDistTemp) + "\n").c_str());
			Player->AddRelativeLocation({ 0.f, gravityForce + fDistTemp, 0.f});
		}
		else
		{
			Player->AddRelativeLocation({ 0.f, gravityForce, 0.f });
		}
	}

	FVector dirVec = playerTrfm.GetWorldUp();
	float dirHalf = (playerScale.Y * .25f + 1.f * dirVec.Y) * -1.f;
	Line->SetScale3D({ 1.f, playerScale.Y * .5f, 0.0f });
	Line->SetRelativeLocation(playerTrfm.Location + FVector{ 0.f, dirHalf, -1.f });
}

void TestActor::InitNavMesh()
{
	NavDatas.reserve(2000);		// TODO: set with mesh size

	int idx = 0;
	for (size_t i = 0, size = MultipleTriangles.size(); i < size; i += 3)
	{
		NavData nd;
		nd.Vertex[0] = MultipleTriangles[i].POSITION;
		nd.Vertex[1] = MultipleTriangles[i+1].POSITION;
		nd.Vertex[2] = MultipleTriangles[i+2].POSITION;
		nd.Index = idx++;

		NavDatas.push_back(nd);
	}

	for (size_t i = 0, size = NavDatas.size(); i < size-1; ++i)
	{
		for (size_t j = i+1; j < size; j++)
		{
			NavData& leftNd = NavDatas[i];
			NavData& rightNd = NavDatas[j];
			if (leftNd.IsAttached(rightNd))
			{
				leftNd.LinkBoth(rightNd);
			}
		}
	}

	// find very first collided mesh
	CheckAllMesh();
}

void TestActor::CheckAllMesh()
{
	for (size_t i = 0, size = NavDatas.size(); i < size; ++i)
	{
		const FTransform& playerTfrm = Player->GetTransform();
		const FTransform& triangleTfrm = Triangle->GetTransform();
		const NavData& nd = NavDatas[i];

		float fDist = 0.f;
		bool isCollided = nd.Intersects(playerTfrm.Location, FVector::UP, triangleTfrm.ScaleMat, triangleTfrm.RotationMat, triangleTfrm.LocationMat, fDist);
		if (isCollided)
		{
			CollidedNavIndex = nd.Index;
			OutputDebugStringA(("First collided index: " + std::to_string(nd.Index) + "\n").c_str());
			break;
		}
	}
}