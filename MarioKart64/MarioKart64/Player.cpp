#include "PreCompile.h"
#include "Player.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EnginePlatform/EngineInput.h>
#include "TestMap.h"
#include "CData.h"

APlayer::APlayer()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Renderer = CreateDefaultSubObject<USpriteRenderer>();
	Renderer->SetOrder(0);

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


	RendererDebug = CreateDefaultSubObject<USpriteRenderer>();
	RendererDebug->CreateAnimation("Idle", "Mario.png", 0, 3, .3f);
	RendererDebug->ChangeAnimation("Idle");
	RendererDebug->SetupAttachment(RootComponent);
	RendererDebug->ColorData.MulColor = { 0.f, 0.f, 0.f, 0.f };
}

APlayer::~APlayer()
{
}

void APlayer::Tick(float _deltaTime)
{
	APawn::Tick(_deltaTime);

	Move(_deltaTime);
}

void APlayer::Move(float _deltaTime)
{
	bool isLog = false;
	float x = 0.f;
	float dx = 0.f;
	FTransform trfmPlayer = GetActorTransform();
	float4 orgLoc = trfmPlayer.Location;

	if (UEngineInput::IsPress(VK_UP))
	{
		Velocity = FPhysics::GetVf(Velocity, ACCELERATION - FRICTION_FORCE, _deltaTime);
		if (Velocity > MAX_VELOCITY)
		{
			Velocity = MAX_VELOCITY;
		}

		dx = FPhysics::GetDeltaX(Velocity, ACCELERATION - FRICTION_FORCE, _deltaTime);
		if (isLog) OutputDebugStringA(("1. Velocity: " + std::to_string(Velocity) + ", dx: " + std::to_string(dx) + "\n").c_str());
	}
	else if (UEngineInput::IsPress(VK_DOWN))
	{
		Velocity = FPhysics::GetVf(Velocity, -ACCELERATION + FRICTION_FORCE, _deltaTime);
		if (Velocity < -MAX_VELOCITY)
		{
			Velocity = -MAX_VELOCITY;
		}

		dx = FPhysics::GetDeltaX(Velocity, -ACCELERATION + FRICTION_FORCE, _deltaTime);
		if (isLog) OutputDebugStringA(("2. Velocity: " + std::to_string(Velocity) + ", dx: " + std::to_string(dx) + "\n").c_str());
	}
	else
	{
		if (Velocity > 0)
		{
			Velocity = FPhysics::GetVf(Velocity, -FRICTION_FORCE, _deltaTime);
			if (Velocity < 0)
			{
				Velocity = 0;
			}

			dx = FPhysics::GetDeltaX(Velocity, -FRICTION_FORCE, _deltaTime);
			if (isLog) OutputDebugStringA(("3. Velocity: " + std::to_string(Velocity) + ", dx: " + std::to_string(dx) + "\n").c_str());
		}
		else if (Velocity < 0)
		{
			Velocity = FPhysics::GetVf(Velocity, FRICTION_FORCE, _deltaTime);
			if (Velocity > 0)
			{
				Velocity = 0;
			}

			dx = FPhysics::GetDeltaX(Velocity, FRICTION_FORCE, _deltaTime);
			if (isLog) OutputDebugStringA(("4. Velocity: " + std::to_string(Velocity) + ", dx: " + std::to_string(dx) + "\n").c_str());
		}
	}

	float rotVal = 0.f;
	if (UEngineInput::IsPress(VK_LEFT))
	{
		rotVal = -1.5f;
	}
	else if (UEngineInput::IsPress(VK_RIGHT))
	{
		rotVal = 1.5f;
	}

	FVector dir = GetActorForwardVector();
	dir.Normalize();
	dir = dir * dx;

	FVector rot{ 0.f, rotVal, 0.f };

	CheckCollision(rot, dir, _deltaTime);
}

void APlayer::SetMap(class ATestMap* _ptr)
{
	TestMapPtr = _ptr;
}

void APlayer::CheckCollision(const FVector& _rot, const FVector _forward, float _deltaTime)
{
	bool isCollisionLog = true;

	FTransform trfmPlayer = GetActorTransform();
	FTransform trfmObj = TestMapPtr->GetActorTransform();
	FVector UpVector = FVector::UP;

	const float GRAVITY_FORCE = -300.f;
	float gravityForce = GRAVITY_FORCE * _deltaTime;

	int navIdx = TestMapPtr->GetNavIndex();
	if (navIdx == -1)
	{
		AddActorLocation({ 0.f, gravityForce, 0.f });
		CheckCollisionOfAllMap();
		return;
	}

	bool isCollided = false;
	float fDist = 0.f;
	const std::vector<NavData> navDatas = TestMapPtr->GetNavData();
	NavData nd = navDatas[navIdx];
	isCollided = nd.Intersects(trfmPlayer.Location, UpVector, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDist);

	FVector lastVec = _forward;
	FVector lastRot = _rot;

	// check collision with link datas
	if (!isCollided)
	{
		for (int linkedIdx : nd.LinkData)
		{
			isCollided = navDatas[linkedIdx].Intersects(trfmPlayer.Location, UpVector, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDist);
			if (isCollided)
			{
				if (isCollisionLog) OutputDebugStringA(("[First coliided idx]: " + std::to_string(linkedIdx) + "\n").c_str());
				TestMapPtr->SetNavIndex(linkedIdx);
				navIdx = linkedIdx;
				break;
			}
		}
	}

	// Temporary gravity
	if (isCollided)
	{
		lastVec.Y += fDist;
	}
	else
	{
		FVector forward = GetActorForwardVector();	// option1. not working
		//FVector forward = RendererDebug->GetTransformRef().GetLocalFoward();	// option2.
		lastVec.Y = forward.Y + gravityForce;

		OutputDebugStringA(("- gravityForce: " + std::to_string(gravityForce) + "\n").c_str());
		OutputDebugStringA(("- forward: " + std::to_string(forward.X) + ", " + std::to_string(forward.Y) + ", " + std::to_string(forward.Z) + "\n").c_str());
		OutputDebugStringA(("- _forward: " + std::to_string(_forward.Length()) + " .. " + std::to_string(_forward.X) + ", " + std::to_string(_forward.Y) + ", " + std::to_string(_forward.Z) + "\n").c_str());

		// check fucture location
		FVector locFuture = trfmPlayer.Location + lastVec;
		float fDistTemp = 0.f;
		nd = navDatas[navIdx];

		isCollided = nd.Intersects(locFuture, UpVector, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDistTemp);
		if (isCollided)
		{
			if (isCollisionLog) OutputDebugStringA(("[Second coliided idx]: " + std::to_string(navIdx) + "\n").c_str());
		}
		else
		{
			for (int linkedIdx : nd.LinkData)
			{
				isCollided = navDatas[linkedIdx].Intersects(locFuture, UpVector, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDistTemp);
				if (isCollided)
				{
					if (isCollisionLog) OutputDebugStringA(("[Third coliided idx]: " + std::to_string(linkedIdx) + "\n").c_str());
					TestMapPtr->SetNavIndex(linkedIdx);
					navIdx = linkedIdx;
					break;
				}
			}
		}

		if (isCollided)
		{
			if (isCollisionLog) OutputDebugStringA(("fDistTemp: " + std::to_string(fDistTemp) + "\n").c_str());
			lastVec.Y += fDistTemp;
		}
	}

	// Temporary physics of slope
	float slopeAngle = GetActorTransform().Rotation.X;
	if (isCollided)
	{
		nd = navDatas[navIdx];
		nd.Vertex[0] = nd.Vertex[0] * trfmObj.ScaleMat * trfmObj.RotationMat * trfmObj.LocationMat;
		nd.Vertex[1] = nd.Vertex[1] * trfmObj.ScaleMat * trfmObj.RotationMat * trfmObj.LocationMat;
		nd.Vertex[2] = nd.Vertex[2] * trfmObj.ScaleMat * trfmObj.RotationMat * trfmObj.LocationMat;
		float4 v1 = nd.Vertex[2] - nd.Vertex[0];
		float4 v2 = nd.Vertex[1] - nd.Vertex[0];
		v1.Normalize();
		v2.Normalize();

		FVector normalV = FVector::Cross(v1, v2);
		normalV.Normalize();
		FVector upVec = FVector::UP;
		FVector dir = upVec - normalV;
		dir.Normalize();

		slopeAngle = FVector::GetVectorAngleDeg(normalV, FVector::UP);

		//OutputDebugStringA(("- normalV: " + std::to_string(normalV.X) + ", " + std::to_string(normalV.Y) + ", " + std::to_string(normalV.Z) + "\n").c_str());
		if (isCollisionLog) OutputDebugStringA(("- slopeAngle: " + std::to_string(slopeAngle) + ", dir: " + std::to_string(dir.X) + ", " + std::to_string(dir.Y) + ", " + std::to_string(dir.Z) + "\n").c_str());
		if (!std::isnan(slopeAngle))
		{
			/*float tempForce = gravityForce * slopeAngle * .5f;
			Velocity = FPhysics::GetVf(Velocity, tempForce, _deltaTime);
			float dx = FPhysics::GetDeltaX(Velocity, tempForce, _deltaTime);
			OutputDebugStringA(("tempForce: " + std::to_string(tempForce) + ", Velocity: " + std::to_string(Velocity) + ", dx : " + std::to_string(dx) + "\n").c_str());*/

			if (slopeAngle > 1.f)
			{
				lastVec -= dir * slopeAngle;
			}
			else if (slopeAngle < -1.f)
			{
				lastVec += dir * slopeAngle;
			}
		}
	}

	lastRot += trfmPlayer.Rotation;
	// option1. not working
	//lastRot.X = slopeAngle;

	// option2. rotated hidden renderer
	//RendererDebug->AddLocalRotation({ slopeAngle, 0.f, 0.f });

	OutputDebugStringA("------------------------------------------\n");

	SetActorRotation(lastRot);
	AddActorLocation(lastVec);
}

void APlayer::CheckCollisionOfAllMap()
{
	const FTransform& tfrmPlayer = GetTransform();
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