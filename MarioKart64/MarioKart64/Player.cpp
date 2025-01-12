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
	bool isLog = true;
	float x = 0.f;
	float dx = 0.f;
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
	OutputDebugStringA(("deltaTime: " + std::to_string(_deltaTime) + ", nd.LinkData: " + std::to_string(nd.LinkData.size()) + "\n").c_str());

	// check collision with link datas
	if (!isCollided)
	{
		OutputDebugStringA("1. collision: false\n");
		for (int linkedIdx : nd.LinkData)
		{
			isCollided = navDatas[linkedIdx].Intersects(trfmPlayer.Location, UpVector, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDist);
			if (isCollided)
			{
				if (isLog) OutputDebugStringA(("[First coliided idx]: " + std::to_string(linkedIdx) + "\n").c_str());
				TestMapPtr->SetNavIndex(linkedIdx);
				navIdx = linkedIdx;
				break;
			}
		}
	}
	else
	{
		OutputDebugStringA("1. collision: true\n");
	}

	FVector lastVec;
	FVector lastRot;

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

	if (UEngineInput::IsPress(VK_LSHIFT))
	{
		Velocity = MAX_VELOCITY;
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
	lastVec = dir;
	lastRot.Y = rotVal;

	// Temporary gravity
	if (isCollided)
	{
		lastVec.Y += fDist;
	}
	else
	{
		//FVector forward = RendererDebug->GetTransformRef().GetWorldFoward();	// option2.
		//forward.Y *= dy;
		//lastVec.Y += forward.Y + gravityForce;
		lastVec.Y += gravityForce;

		/*OutputDebugStringA(("- gravityForce: " + std::to_string(gravityForce) + ", dx: " + std::to_string(dx) + ", dy: " + std::to_string(dy) + ", VelocityV: " + std::to_string(VelocityV) + "\n").c_str());
		OutputDebugStringA(("- forward: " + std::to_string(forward.X) + ", " + std::to_string(forward.Y) + ", " + std::to_string(forward.Z) + "\n").c_str());
		OutputDebugStringA(("- lastVec: " + std::to_string(lastVec.X) + ", " + std::to_string(lastVec.Y) + ", " + std::to_string(lastVec.Z) + "\n").c_str());*/

		// check fucture location
		FVector locFuture = trfmPlayer.Location + lastVec;
		float fDistTemp = 0.f;
		nd = navDatas[navIdx];

		isCollided = nd.Intersects(locFuture, UpVector, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDistTemp);
		if (isCollided)
		{
			if (isLog) OutputDebugStringA(("[Second coliided idx]: " + std::to_string(navIdx) + "\n").c_str());
		}
		else
		{
			for (int linkedIdx : nd.LinkData)
			{
				isCollided = navDatas[linkedIdx].Intersects(locFuture, UpVector, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDistTemp);
				if (isCollided)
				{
					if (isLog) OutputDebugStringA(("[Third coliided idx]: " + std::to_string(linkedIdx) + "\n").c_str());
					TestMapPtr->SetNavIndex(linkedIdx);
					navIdx = linkedIdx;
					break;
				}
			}
		}

		if (isCollided)
		{
			lastVec.Y += fDistTemp;
		}
	}

	if (isCollided)
	{
		// Temporary physics of slope
		nd = navDatas[navIdx];
		FVector vertex0 = nd.Vertex[0] * trfmObj.ScaleMat * trfmObj.RotationMat * trfmObj.LocationMat;
		FVector vertex1 = nd.Vertex[1] * trfmObj.ScaleMat * trfmObj.RotationMat * trfmObj.LocationMat;
		FVector vertex2 = nd.Vertex[2] * trfmObj.ScaleMat * trfmObj.RotationMat * trfmObj.LocationMat;
		float4 v1 = vertex2 - vertex0;
		float4 v2 = vertex1 - vertex0;
		v1.Normalize();
		v2.Normalize();

		FVector normalV = FVector::Cross(v1, v2);
		normalV.Normalize();

		float crossV = FVector::Cross(normalV, FVector::UP).X;
		float slopeAngle = FVector::GetVectorAngleDeg(normalV, FVector::UP);
		if (crossV > 0)
		{
			slopeAngle *= -1;
		}

		//OutputDebugStringA(("- normalV: " + std::to_string(normalV.X) + ", " + std::to_string(normalV.Y) + ", " + std::to_string(normalV.Z) + "\n").c_str());
		if (isLog) OutputDebugStringA(("- slopeAngle: " + std::to_string(slopeAngle) + ", crossV: " + std::to_string(crossV) + "\n").c_str());

		if (!std::isnan(slopeAngle))
		{
	//		/*float tempForce = -gravityForce * slopeAngle;
	//		Velocity = FPhysics::GetVf(Velocity, tempForce, _deltaTime);
	//		float dx = FPhysics::GetDeltaX(Velocity, tempForce, _deltaTime);
	//		OutputDebugStringA(("tempForce: " + std::to_string(tempForce) + ", Velocity: " + std::to_string(Velocity) + ", dx : " + std::to_string(dx) + "\n").c_str());*/

			//lastVec += FVector{ 0.f, 0.f, 1.f } * slopeAngle * .8f;
		}

		// option2. rotated hidden renderer
		RendererDebug->SetRotation({ slopeAngle, 0.f, 0.f });
	}

	AddActorRotation(lastRot);
	AddActorLocation(lastVec);

	FTransform temp = GetTransform();
	OutputDebugStringA(("set Location: " + std::to_string(temp.Location.X) + ", " + std::to_string(temp.Location.Y) + ", " + std::to_string(temp.Location.Z) + "\n").c_str());

	OutputDebugStringA("------------------------------------------\n");
}

void APlayer::SetMap(class ATestMap* _ptr)
{
	TestMapPtr = _ptr;
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