#include "PreCompile.h"
#include "Driver.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/CameraActor.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/Collision.h>
#include "LineRenderer.h"	// for test
#include "BaseMap.h"
#include "GameData.h"
#include "CData.h"
#include "Item.h"
#include "CGlobal.h"

//#define _ITEM_DEBUG

ADriver::ADriver()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Renderer = CreateDefaultSubObject<USpriteRenderer>();
	Renderer->SetOrder(0);
	Renderer->SetupAttachment(RootComponent);

	RendererDebug = CreateDefaultSubObject<USpriteRenderer>();
	RendererDebug->CreateAnimation("Idle", "Mario.png", 0, 3, .3f);
	RendererDebug->ChangeAnimation("Idle");
	RendererDebug->SetupAttachment(RootComponent);
	RendererDebug->SetActive(false);

#ifdef _ITEM_DEBUG
	DebugItem = CreateDefaultSubObject<USpriteRenderer>();
	DebugItem->SetOrder(0);
	DebugItem->SetupAttachment(RootComponent);
	DebugItem->SetSprite("Items.png", static_cast<int>(EItemType::SIZE));
	DebugItem->SetRelativeLocation({ 30.f, 50.f, 0.f });
#endif

	FutureAngles.reserve(40);
	for (int i = 1; i < 25; ++i)
	{
		FutureAngles.push_back(i * 5.f);
		FutureAngles.push_back(i * -5.f);
	}
}

ADriver::~ADriver()
{
}

void ADriver::BeginPlay()
{
	APawn::BeginPlay();

	//CheckFutureCollisionOfAllMap();
}

void ADriver::Tick(float _deltaTime)
{
	APawn::Tick(_deltaTime);

	if (!IsStarted)
	{
		return;
	}

	if (!IsInitSound)
	{
		IsInitSound = true;
		CarSP = UEngineSound::Play("CarNormal.wav");
		CarSP.Loop(99999);
		CarSP.SetVolume(.05f);
	}

	if (IsSpin)
	{
		Spin();
		return;
	}

	Move(_deltaTime);

	if (IsPickingItem)
	{
		PickItem(_deltaTime);
	}
	else
	{
		TickItem(_deltaTime);
	}
}

void ADriver::SetStart(bool _val)
{
	IsStarted = _val;
}

void ADriver::InitCharacter(ECharacter _character)
{
	SpriteName = RENDER_SPRITES[static_cast<int>(_character)];
	Renderer->CreateAnimation("Idle", SpriteName, 0, 3, .3f);

	{
		const int TURN_SIZE = 25;
		std::vector<int> idxs(TURN_SIZE, 1008);
		std::vector<float> times(TURN_SIZE, .05f);
		for (size_t i = 0; i < TURN_SIZE; ++i)
		{
			idxs[i] += static_cast<int>(i);
		}

		Renderer->CreateAnimation("Spin", SpriteName, idxs, times, false);
	}

	// Temp
	{
		const int TURN_SIZE = 31;
		std::vector<int> idxs(TURN_SIZE, 0);
		std::vector<float> times(TURN_SIZE, .1f);
		for (size_t i = 0; i < TURN_SIZE; ++i)
		{
			idxs[i] = static_cast<int>(i * 36);
		}

		Renderer->CreateAnimation("TurnL", SpriteName, idxs, times, false);
		Renderer->CreateAnimation("TurnR", SpriteName, idxs, times, false);
	}

	Renderer->ChangeAnimation("Idle");

	RendererSize = Renderer->GetWorldScale3D();

	Collision = CreateDefaultSubObject<UCollision>();
	Collision->SetCollisionType(ECollisionType::Sphere);
	Collision->SetupAttachment(RootComponent);
	Collision->SetCollisionProfileName("PLAYER");
	Collision->SetScale3D({ RendererSize.X, RendererSize.Y, RendererSize.X });
	Collision->AddRelativeLocation({ 0.f, RendererSize.X * .5f, 0.f });
	Collision->SetCollisionEnter(std::bind(&ADriver::OnCollisionEnter, this, std::placeholders::_1, std::placeholders::_2));
}

void ADriver::InitRouteIndex(ECircuit _map)
{
	switch (_map)
	{
	case ECircuit::LUIGI_RACEWAY:
		CGlobal::GetRouteIdxLuigi(RouteIdx);
		break;
	case ECircuit::KOOPA_TROOPA_BEACH:
		CGlobal::GetRouteIdxKoopa(RouteIdx);
		break;
	case ECircuit::MARIO_RACEWAY:
		CGlobal::GetRouteIdxMario(RouteIdx);
		break;
	case ECircuit::WARIO_STADIUM:
		CGlobal::GetRouteIdxWario(RouteIdx);
		break;
	case ECircuit::SHERBET_LAND:
		CGlobal::GetRouteIdxSherbet(RouteIdx);
		break;
	case ECircuit::ROYAL_RACEWAY:
		CGlobal::GetRouteIdxRoyal(RouteIdx);
		break;
	case ECircuit::BOWSERS_CASTLE:
		CGlobal::GetRouteIdxBowsers(RouteIdx);
		break;
	case ECircuit::RAINBOW_ROAD:
		CGlobal::GetRouteIdxRainbow(RouteIdx);
		break;
	}
}

void ADriver::Spin()
{
	const std::string& name = Renderer->GetCurAnimName();

	if (name == "Idle")
	{
		Renderer->ChangeAnimation("Spin");
	}
	else if (name == "Spin")
	{
		if (Renderer->IsCurAnimationEnd())
		{
			if (SpinCount == 0)
			{
				SpinCount++;
				Renderer->ChangeAnimation("Spin", true);
			}
			else
			{
				Velocity = 0.f;
				VelocityV = 0.f;
				BoostVal = 0.f;

				SpinCount = 0;
				IsSpin = false;
				Renderer->ChangeAnimation("Idle");
			}
		}
	}
}

void ADriver::TickBoost(float _deltaTime)
{
	float mulVal = 1.f;
	if (CrashBoostCnt > 0)
	{
		--CrashBoostCnt;
		IsBoost = true;
		mulVal = .5f;
	}
	if (MushroomBoostCnt > 0)
	{
		--MushroomBoostCnt;
		IsBoost = true;
		mulVal = .5f;
	}

	if (IsBoost)
	{
		BoostVal += (BOOST_SPEED_UP * mulVal) * _deltaTime;
	}
	else
	{
		if (BoostVal > 0)
		{
			BoostVal -= BOOST_SPEED_DOWN * _deltaTime;
			if (BoostVal < 0) BoostVal = 0.f;
		}
	}
	//OutputDebugStringA(("BoostVal:" + std::to_string(BoostVal) + "\n").c_str());

	IsBoost = false;
}

void ADriver::GetHandleRotationAuto(float _deltaTime, const FVector& _dir, float& _refRot)
{
	FVector curLoc = GetActorLocation();

	float tempFDist = 0.f;
	bool isRotate = false;

	FVector futureDir = curLoc + (_dir * STRIDE);
	futureDir.Y = -1000.f;

	int i = 0;
	bool isCol = CheckCollision(futureDir, FutureNavIdxs[i], tempFDist);
	if (isCol)
	{
		if (isLog) OutputDebugStringA((std::to_string(i) + ": AAAAAAAA").c_str());

		const SNavData& _nd1 = MapPtr->GetNavData(FutureNavIdxs[i]);
		if (_nd1.FloorType == ENavType::ROAD || _nd1.FloorType == ENavType::START_POINT || _nd1.FloorType == ENavType::FLATE_FASTER)
		{
			if (isLog) OutputDebugStringA("---> ROAD\n");
		}
		else
		{
			if (isLog) OutputDebugStringA("---> NO ROAD\n");
			for (int j = 0; j < FutureAngles.size(); ++j)
			{
				FVector newDir = _dir;
				float _angle = FutureAngles[j];
				newDir.Normalize();
				newDir.RotationYDeg(_angle);
				newDir *= STRIDE;
				float lsize = newDir.Length();
				newDir.Y = -1000.f;

				if (isLog) OutputDebugStringA(("newDir:" + std::to_string(newDir.X) + ", " + std::to_string(newDir.Y) + ", " + std::to_string(newDir.Z) + "\n").c_str());

				isCol = CheckCollision(curLoc + newDir, FutureNavIdxs[i], tempFDist);
				if (isCol)
				{
					const SNavData& _nd2 = MapPtr->GetNavData(FutureNavIdxs[i]);
					if (_nd2.FloorType == ENavType::ROAD || _nd2.FloorType == ENavType::START_POINT || _nd2.FloorType == ENavType::FLATE_FASTER)
					{
						if (_angle <= -1)
						{
							_refRot = 100.f * _deltaTime;
						}
						else if (_angle >= 1)
						{
							_refRot = -100.f * _deltaTime;
						}

						isRotate = true;
						break;
					}
					else
					{
						continue;
					}
				}
			}
		}
	}
	else
	{
		if (isLog) OutputDebugStringA((std::to_string(i) + ": BBBBBBBB").c_str());

		for (int j = 0; j < FutureAngles.size(); ++j)
		{
			FVector newDir = _dir;
			float _angle = FutureAngles[j];
			newDir.Normalize();
			newDir.RotationYDeg(_angle);
			newDir *= STRIDE;
			float lsize = newDir.Length();
			newDir.Y = -1000.f;

			if (isLog) OutputDebugStringA(("newDir:" + std::to_string(newDir.X) + ", " + std::to_string(newDir.Y) + ", " + std::to_string(newDir.Z) + "\n").c_str());

			isCol = CheckCollision(curLoc + newDir, FutureNavIdxs[i], tempFDist);
			if (isCol)
			{
				if (isLog) OutputDebugStringA(("---> COL (" + std::to_string(_angle) + ")").c_str());
				const SNavData& _nd2 = MapPtr->GetNavData(FutureNavIdxs[i]);
				if (_nd2.FloorType == ENavType::ROAD || _nd2.FloorType == ENavType::START_POINT || _nd2.FloorType == ENavType::FLATE_FASTER)
				{
					if (isLog) OutputDebugStringA(" [ROAD]");
					if (_angle <= -1)
					{
						_refRot = 100.f * _deltaTime;
					}
					else if (_angle >= 1)
					{
						_refRot = -100.f * _deltaTime;
					}

					isRotate = true;
					break;
				}
				else
				{
					if (isLog) OutputDebugStringA(" [NO ROAD]");
					continue;
				}
			}
			else
			{
				if (isLog) OutputDebugStringA("---> NO COL ");
			}

			if (isRotate) break;
		}

		if (isLog) OutputDebugStringA("\n");
	}
	//OutputDebugStringA(("FutureIdxs:" + std::to_string(FutureNavIdxs[0]) + ", " + std::to_string(FutureNavIdxs[1]) + ", " + std::to_string(FutureNavIdxs[2]) + "\n").c_str());
	//OutputDebugStringA(("rotVal:" + std::to_string(rotVal) + "\n").c_str());
}

void ADriver::Move(float _deltaTime)
{
	float gravityY = GRAVITY_FORCE * _deltaTime;
	FTransform trfmPlayer = GetActorTransform();
	FTransform trfmObj = MapPtr->GetActorTransform();

	/* for debug start */
	/*if (UEngineInput::IsPress(VK_LCONTROL))
	{
		SetActorLocation({ 4142.233887f, 70.f, 8755.953125f });
		SetActorRotation({ 0.f, -117.5f, 0.f });
		NavIdx = -1;
		return;
	}*/
	if (trfmPlayer.Location.Y < -1000)
	{
		Velocity = 0.f;
		BoostVal = 0.f;
		SetActorLocation({ 4142.233887f, 70.f, 8755.953125f });
		SetActorRotation({ 0.f, -117.5f, 0.f });
		NavIdx = -1;
		return;
	}
	/* for debug end */


	TickBoost(_deltaTime);

	if (NavIdx == -1)
	{
		AddActorLocation({ 0.f, gravityY, 0.f });
		CheckCollisionOfAllMap();
		CheckFutureCollisionOfAllMap();
		return;
	}

	float fDist = 0.f;
	bool isCollided = CheckCollision(trfmPlayer.Location, NavIdx, fDist);

	// Temp start
	bool isReverse = false;
	std::map<int, int>::iterator it = RouteIdx.find(NavIdx);
	std::map<int, int>::iterator itEnd = RouteIdx.end();
	if (it != itEnd)
	{
		int _idx = it->second;
		if (IsCheckLap == false && _idx == 0)
		{
			IsCheckLap = true;
		}

		if (IsCheckLap)
		{
			//if (isLog) OutputDebugStringA(("navIdx: " + std::to_string(NavIdx)+", _idx: " + std::to_string(_idx) + "\n").c_str());
			//OutputDebugStringA(("CurRouteIdx : " + std::to_string(CurRouteIdx) + ", _idx: " + std::to_string(_idx) + ", NavIdx: " + std::to_string(NavIdx) + "\n").c_str());
			if (CurRouteIdx > _idx)
			{
				isReverse = true;
				OutputDebugStringA("REVERSE!!!!!!!\n");
			}
			else if (CurRouteIdx < _idx)
			{
				//OutputDebugStringA("RIGHT!!!!!!!\n");
				CurRouteIdx = _idx;
			}
		}
	}
	else
	{
		//OutputDebugStringA("EMPTY!!!!!!!\n");
	}

	// Temp end

	FVector lastRot;
	FVector lastVec;
	FVector dir = GetActorForwardVector();
	dir.Normalize();

	float dx = 0.f;
	float rotVal = 0.f;
	float slopeAngle = 0.f;

	/* Test start */
	if (IsAutomative)
	{
		GetHandleRotationAuto(_deltaTime, dir, rotVal);
	}
	else
	{
		GetHandleRotation(_deltaTime, rotVal);
	}

	if (fabs(rotVal) > .1f)
	{
		AdditionalFriction = 10.f;
	}

	GetForwardPhysics(_deltaTime, dx, true);

	if (isCollided)
	{
		const SNavData& _nd = MapPtr->GetNavData(NavIdx);
		if (_nd.FloorType == ENavType::FLATE_FASTER)
		{
			IsBoost = true;
		}

		MapPtr->SetDebugLocation(float4{ static_cast<float>(NavIdx), 0.f, 0.f, 1.f });

		// Temporary physics of slope
		{
			slopeAngle = GetSlope();

			FVector backVec = GetActorForwardVector();
			backVec.X *= -1;
			backVec.Z *= -1;

			if (!std::isnan(slopeAngle))
			{
				//lastVec += FVector{ 0.f, 0.f, 1.f } * slopeAngle * .8f;
				//lastVec -= backVec * slopeAngle * .8f;
			}

			RendererDebug->SetRotation({ slopeAngle, 0.f, 0.f });
		}

		VelocityV = Velocity * sinf(-slopeAngle * UEngineMath::D2R);
		if (isLog) OutputDebugStringA(("VelocityV: " + std::to_string(VelocityV) + ", sinf: " + std::to_string(sinf(-slopeAngle * UEngineMath::D2R)) + "\n").c_str());
		PrevLoc = GetActorLocation();
		CheckLap(isReverse);
	}
	else
	{
		// I didn't multiply deltaTime intentionally.
		bool hasFloor = CheckCollision(trfmPlayer.Location + FVector{ 0.f, GRAVITY_FORCE, 0.f }, NavIdx, fDist);

		if (hasFloor)
		{
			// Adjust gravity
			//GetForwardPhysics(_deltaTime, dx);
			//GetHandleRotation(_deltaTime, rotVal);

			// for fDist. Do not remove
			CheckCollision(trfmPlayer.Location + FVector{ 0.f, gravityY, 0.f }, NavIdx, fDist);

			dir *= dx;
			lastVec = dir;

			lastRot.Y = rotVal;
			lastVec.Y += gravityY + fDist;
		}
		else
		{
			// Block
			ENavType floorType = MapPtr->GetNavData(PrevIdx).FloorType;
			if (floorType != ENavType::FLATE_FASTER && floorType != ENavType::FLATE_JUMP)
			{
				Velocity *= .5f;

				FVector curLoc = GetActorLocation();
				FVector subLoc = PrevLoc - curLoc;
				subLoc.Normalize();
				lastVec = subLoc * Velocity * .5f;
			}
			else
			{
				// Adjust gravity
				//GetForwardPhysics(_deltaTime, dx);

				CheckCollisionOfAllMap();

				VelocityV = FPhysics::GetVf(VelocityV, gravityY * 250.f, _deltaTime);
				float dy = FPhysics::GetDeltaX(VelocityV, gravityY * 250.f, _deltaTime);

				dir *= dx;
				lastVec = dir;

				lastVec.Y += dy;
				if (isLog) OutputDebugStringA(("VelocityV: " + std::to_string(VelocityV) + ", gravityForce: " + std::to_string(gravityY) + ", dy: " + std::to_string(dy) + "\n").c_str());
			}
		}

		AddActorRotation(lastRot);
		AddActorLocation(lastVec);
		return;
	}

	dir *= dx;
	lastVec = dir;

	lastRot.Y = rotVal;
	lastVec.Y += fDist;

	/* Test end */

	AddActorRotation(lastRot);
	AddActorLocation(lastVec);

	//FTransform temp = GetTransform();
	//OutputDebugStringA(("Last location: " + std::to_string(temp.Location.X) + ", " + std::to_string(temp.Location.Y) + ", " + std::to_string(temp.Location.Z) + "\n").c_str());
	//OutputDebugStringA(("player rotation: " + std::to_string(temp.Rotation.X) + ", " + std::to_string(temp.Rotation.Y) + ", " + std::to_string(temp.Rotation.Z) + "\n").c_str());

	//OutputDebugStringA("------------------------------------------\n");
}

void ADriver::SetMap(ABaseMap* _ptr)
{
	MapPtr = _ptr;
}

void ADriver::CheckLap(bool _isReverse)
{
	if (_isReverse) return;
	if (IsCheckLap == false) return;

	if (PrevNavIdx != NavIdx)
	{
		SNavData prevNd = MapPtr->GetNavData(PrevNavIdx);
		SNavData nd = MapPtr->GetNavData(NavIdx);
		if (prevNd.FloorType != ENavType::START_POINT && nd.FloorType == ENavType::START_POINT)
		{
			++Lap;
			IsCheckLap = false;
			CurRouteIdx = 0;
			OutputDebugStringA(("==================== ++Lab: " + std::to_string(Lap) + ", NavIdx: " + std::to_string(NavIdx) + "\n").c_str());
			OnChangeLap(Lap);

			if (Lap == ALL_LAB)
			{
				EndLap();
				OutputDebugStringA("GOAL IN!!\n");
				IsFinished = true;
				CarSP.Stop();
				CarAccel.Stop();
			}
		}

		OutputDebugStringA(("NavIdx: " + std::to_string(NavIdx) + "\n").c_str());
		PrevNavIdx = NavIdx;
		FileTemp << std::to_string(NavIdx) << ",";
	}
}

float ADriver::GetSlope()
{
	float slopeAngle = 0.f;
	if (NavIdx < 0) return 0.f;

	const SNavData& nd = MapPtr->GetNavData(NavIdx);
	const FTransform& trfmObj = MapPtr->GetTransform();

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
	slopeAngle = FVector::GetVectorAngleDeg(normalV, FVector::UP);
	if (crossV < 0)
	{
		slopeAngle *= -1;
	}
	//OutputDebugStringA(("- slopeAngle: " + std::to_string(slopeAngle) + ", crossV: " + std::to_string(crossV) + "\n").c_str());
	return slopeAngle;
}

void ADriver::GetHandleRotation(float _deltaTime, float& _refRot)
{
	if (UEngineInput::IsPress(VK_LEFT))
	{
		_refRot = -65.f * _deltaTime;
	}
	else if (UEngineInput::IsPress(VK_RIGHT))
	{
		_refRot = 65.f * _deltaTime;
	}
	//OutputDebugStringA(("rotVal: " + std::to_string(_refRot) + "\n").c_str());
}

void ADriver::GetForwardPhysics(float _deltaTime, float& _refDx, bool _isCollided)
{
	float acc = 0.f;
	float dx = 0.f;
	bool isPushed = true;

	if (UEngineInput::IsPress(VK_UP) || IsAutomative)
	{
		DirVTrain = 1;
		acc = ACCELERATION - FRICTION_FORCE - AdditionalFriction;

		/*if (CarAccel.IsInited() == false)
		{
			CarAccel = UEngineSound::Play("CarAccel.wav");
			CarAccel.Loop(9999);
			CarAccel.SetVolume(.1f);
		}
		else
		{
			CarAccel.Resume();
		}*/
	}
	else if (UEngineInput::IsPress(VK_DOWN))
	{
		//CarAccel.Pause();
		DirVTrain = 2;
		acc = -ACCELERATION + FRICTION_FORCE + AdditionalFriction;
	}
	else
	{
		//CarAccel.Pause();
		isPushed = false;
	}

	if (_isCollided && isPushed)
	{
		Velocity = FPhysics::GetVf(Velocity, acc, _deltaTime);
		if (Velocity > MAX_VELOCITY)
		{
			Velocity = MAX_VELOCITY;
		}
		else if (Velocity < -MAX_VELOCITY)
		{
			Velocity = -MAX_VELOCITY;
		}

		Velocity += BoostVal;
		dx = FPhysics::GetDeltaX(Velocity, acc, _deltaTime);
	}
	else
	{
		if (fabs(Velocity) > 30)
		{
			acc = -Velocity * .75f;
			Velocity = FPhysics::GetVf(Velocity, acc, _deltaTime);

			if (Velocity > MAX_VELOCITY)
			{
				Velocity = MAX_VELOCITY;
			}
			else if (Velocity < -MAX_VELOCITY)
			{
				Velocity = -MAX_VELOCITY;
			}

			Velocity += BoostVal;
			dx = FPhysics::GetDeltaX(Velocity, acc, _deltaTime);
		}
		else
		{
			Velocity = 0.f;
			dx = 0.f;
		}
	}

	//OutputDebugStringA(("Velocity: " + std::to_string(Velocity) + ", acc: " + std::to_string(acc) + ", dx: " + std::to_string(dx) + "\n").c_str());
	_refDx = dx;
}

bool ADriver::CheckCollision(const FVector& _loc, int& _refIdx, float& _refDist)
{
	bool isCollided = false;
	const FTransform& trfmObj = MapPtr->GetTransform();

	const std::vector<SNavData>& navDatas = MapPtr->GetNavData();
	SNavData nd = navDatas[_refIdx];
	isCollided = nd.Intersects(_loc, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, _refDist);
	
	if (isCollided)
	{
		//OutputDebugStringA("CheckCollision first: true\n");
	}
	else
	{
		//OutputDebugStringA("CheckCollision first: false\n");
		for (int linkedIdx : nd.LinkData)
		{
			isCollided = navDatas[linkedIdx].Intersects(_loc, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, _refDist);
			if (isCollided)
			{
				//OutputDebugStringA(("CheckCollision second: true .. " + std::to_string(linkedIdx) + "\n").c_str());
				PrevIdx = _refIdx;
				PrevGroupIdx = navDatas[_refIdx].GroupIndex;

				_refIdx = linkedIdx;
				break;
			}
		}
	}

	return isCollided;
}

void ADriver::CheckCollisionOfAllMap()
{
	const FTransform& tfrmPlayer = GetTransform();
	const std::vector<SNavData>& navDatas = MapPtr->GetNavData();
	const FTransform& trfmObj = MapPtr->GetTransform();

	// TODO: Important.. This doesn't take into account children
	for (size_t i = 0, size = navDatas.size(); i < size; ++i)
	{
		const SNavData& nd = navDatas[i];

		float fDist = 0.f;
		bool isCollided = nd.Intersects(tfrmPlayer.Location, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDist);
		if (isCollided)
		{
			NavIdx = nd.Index;
			OutputDebugStringA(("CheckCollisionAll index: " + std::to_string(NavIdx) + "\n").c_str());
			break;
		}
	}
}

void ADriver::CheckFutureCollisionOfAllMap()
{
	const FVector& tfrmPlayer = GetTransform().Location;
	const std::vector<SNavData>& navDatas = MapPtr->GetNavData();
	const FTransform& trfmObj = MapPtr->GetTransform();

	// TODO: Important.. This doesn't take into account children
	for (size_t i = 0, size = navDatas.size(); i < size; ++i)
	{
		float fDist = 0.f;
		const SNavData& nd = navDatas[i];
		for (int i = 0; i < 3; ++i)
		{
			FVector loc = tfrmPlayer + (GetActorForwardVector() * STRIDE * static_cast<float>(i + 1));

			bool isCollided = nd.Intersects(loc, FVector::UP, trfmObj.ScaleMat, trfmObj.RotationMat, trfmObj.LocationMat, fDist);
			if (isCollided)
			{
				FutureNavIdxs[i] = nd.Index;
				break;
			}
		}
	}
}

void ADriver::OnCollisionEnter(UCollision* _this, UCollision* _other)
{
	const std::string& name = _other->GetCollisionProfileName();

	if (name == "PLAYER")
	{
		AActor* actor = _other->GetActor();
		ADriver* ptr = dynamic_cast<ADriver*>(actor);
		if (ptr != nullptr)
		{
			if (IsCrashFromMe(actor))
			{
				Velocity = Velocity * -.1f;
				ptr->CrashBoostCnt = 1;
				ptr->SetVelocity(ptr->GetVelocity() * 1.5f);
			}
			else
			{
				CrashBoostCnt = 1;
				Velocity = Velocity * 1.5f;
				ptr->SetVelocity(ptr->GetVelocity() * -.1f);
			}
		}
	}
	else if (name == "SHELL")
	{
		_other->GetActor()->Destroy();
		IsSpin = true;
	}
	else if (name == "BANANA")
	{
		_other->GetActor()->Destroy();
		IsSpin = true;
	}
	else if (name == "FAKE_ITEMBOX")
	{
		_other->GetActor()->Destroy();
		IsSpin = true;
	}
	else if (name == "ITEMBOX")
	{
		_other->GetActor()->SetActive(false);

		if (ItemIndex >= ITEM_NONE)
		{
			ItemRoulette.Reset();
			IsPickingItem = true;
		}
	}
}

float ADriver::GetDistFromNextRoute()
{
	return GetDistWithRouteIdx(GetNextRouteIdx());
}

float ADriver::GetDistWithRouteIdx(int _idx)
{
	const SNavData& data = MapPtr->GetNavData(_idx);
	float x = (data.Vertex[0].X + data.Vertex[1].X + data.Vertex[2].X) / 3.f;
	float y = (data.Vertex[0].Y + data.Vertex[1].Y + data.Vertex[2].Y) / 3.f;
	float z = (data.Vertex[0].Z + data.Vertex[1].Z + data.Vertex[2].Z) / 3.f;

	FVector nextCenter{ x, y, z };
	FVector loc = GetActorLocation();
	float len = (nextCenter - loc).Length();
	return len;
}

int ADriver::GetNextRouteIdx()
{
	std::map<int, int>::iterator it = RouteIdx.find(CurRouteIdx + 1);
	std::map<int, int>::iterator itEnd = RouteIdx.end();

	if (it == itEnd)
	{
		it = RouteIdx.find(0);
	}

	if (it != itEnd)
	{
		return it->second;
	}

	return -1;
}

bool ADriver::IsCrashFromMe(AActor* _other)
{
	FVector curForward = GetActorForwardVector();
	AActor* actor = _other;
	const FTransform& curTrfm = GetTransform();
	const FTransform& otherTrfm = actor->GetTransform();
	FVector up = actor->GetActorUpVector();
	FVector right = actor->GetActorRightVector();
	FVector otherLoc = actor->GetActorLocation();

	FVector rt = (up + right) * 1000.f;
	FVector rb{ rt.X, -rt.Y, rt.Z };
	FVector lt{ -rt.X, rt.Y, rt.Z };
	FVector lb{ -rt.X, -rt.Y, rt.Z };

	bool isCol = false;
	float fDist = 0.f;
	{
		DirectX::XMVECTOR v1 = (rt + otherLoc).DirectVector;
		DirectX::XMVECTOR v2 = (rb + otherLoc).DirectVector;
		DirectX::XMVECTOR v3 = (lt + otherLoc).DirectVector;
		/*DirectX::XMVECTOR v1 = (rt * otherTrfm.ScaleMat * otherTrfm.RotationMat * otherTrfm.LocationMat).DirectVector;
		DirectX::XMVECTOR v2 = (rb * otherTrfm.ScaleMat * otherTrfm.RotationMat * otherTrfm.LocationMat).DirectVector;
		DirectX::XMVECTOR v3 = (lt * otherTrfm.ScaleMat * otherTrfm.RotationMat * otherTrfm.LocationMat).DirectVector;*/
		isCol = DirectX::TriangleTests::Intersects(curTrfm.Location.DirectVector, curForward.DirectVector, v1, v2, v3, fDist);
	}

	if (!isCol)
	{
		DirectX::XMVECTOR v1 = (lt + otherLoc).DirectVector;
		DirectX::XMVECTOR v2 = (rb + otherLoc).DirectVector;
		DirectX::XMVECTOR v3 = (lb + otherLoc).DirectVector;
		/*DirectX::XMVECTOR v1 = (lt * otherTrfm.ScaleMat * otherTrfm.RotationMat * otherTrfm.LocationMat).DirectVector;
		DirectX::XMVECTOR v2 = (rb * otherTrfm.ScaleMat * otherTrfm.RotationMat * otherTrfm.LocationMat).DirectVector;
		DirectX::XMVECTOR v3 = (lb * otherTrfm.ScaleMat * otherTrfm.RotationMat * otherTrfm.LocationMat).DirectVector;*/
		isCol = DirectX::TriangleTests::Intersects(curTrfm.Location.DirectVector, curForward.DirectVector, v1, v2, v3, fDist);
	}

	if (isCol)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void ADriver::PickItem(float _deltaTime)
{
	int itemIdx = ItemRoulette.PickItem(_deltaTime);
	if (itemIdx == -1)
	{
		//ItemIndex = static_cast<int>(EItemType::FAKE_ITEMBOX);	// for test
		ItemIndex = PrevItemIdx;
		IsPickingItem = false;
		PrevItemIdx = -1;
		return;
	}

	if (PrevItemIdx != itemIdx)
	{
		PrevItemIdx = itemIdx;
		OnChangeItem(itemIdx);
#ifdef _ITEM_DEBUG
		DebugItem->SetSprite("Items.png", itemIdx);	// Temp
#endif
	}
}

void ADriver::TickItem(float _deltaTime)
{
	if (ItemIndex >= ITEM_NONE) return;

	UseItem();
}

void ADriver::UseItem()
{
	EItemType itemType = static_cast<EItemType>(ItemIndex);
	switch (itemType)
	{
	case EItemType::GREEN_SHELL:
	case EItemType::GREEN_SHELLS:
	case EItemType::RED_SHELL:
	case EItemType::RED_SHELLS:
	case EItemType::BOWSER_SHELL:
		UseItem_Shell(itemType);
		break;
	case EItemType::MUSHROOM:
	case EItemType::MUSHROOM2:
	case EItemType::MUSHROOM3:
	case EItemType::GOLD_MUSHROOM:
		UseItem_Mushroom(itemType);
		break;
	case EItemType::BANANA:
	case EItemType::BANANAS:
		UseItem_Banana(itemType);
		break;
	case EItemType::STAR:
		UseItem_Star(itemType);
		break;
	case EItemType::THUNDER:
		UseItem_Thunder(itemType);
		break;
	case EItemType::GHOST:
		UseItem_Ghost(itemType);
		break;
	case EItemType::FAKE_ITEMBOX:
		UseItem_FakeItemBox(itemType);
		break;
	}

	ItemIndex = ITEM_NONE;
#ifdef _ITEM_DEBUG
	DebugItem->SetSprite("Items.png", ITEM_NONE);
#endif
}

void ADriver::UseItem_Shell(const EItemType& _itemType)
{
	if (_itemType < EItemType::RED_SHELL)
	{
		// TODO
	}
	else if (_itemType < EItemType::BOWSER_SHELL)
	{
		// TODO
	}

	FVector forward = GetActorForwardVector();
	std::shared_ptr<AItem> item = GetWorld()->SpawnActor<AItem>();
	item->Init(_itemType, MapPtr, NavIdx);
	item->SetActorRotation(GetActorRotation());
	item->SetActorLocation(GetActorLocation() + (forward * item->Size * 2));
	item->SetInitVelocity(Velocity + 1000.f);
	item->SetDirection(forward);
}

void ADriver::UseItem_Mushroom(const EItemType& _itemType)
{
	MushroomBoostCnt = 20;
}

void ADriver::UseItem_Banana(const EItemType& _itemType)
{
	FVector backward = GetActorForwardVector() * -1.f;

	std::shared_ptr<AItem> item = GetWorld()->SpawnActor<AItem>();
	item->Init(_itemType, MapPtr, NavIdx);
	item->SetActorLocation(GetActorLocation() + (backward * item->Size * 2));
}

void ADriver::UseItem_Star(const EItemType& _itemType)
{
	// TODO
	MushroomBoostCnt = 20;
}

void ADriver::UseItem_Thunder(const EItemType& _itemType)
{
	// TODO
	MushroomBoostCnt = 20;
}

void ADriver::UseItem_Ghost(const EItemType& _itemType)
{
	// TODO
	MushroomBoostCnt = 20;
}

void ADriver::UseItem_FakeItemBox(const EItemType& _itemType)
{
	FVector backward = GetActorForwardVector() * -1.f;

	std::shared_ptr<AItem> item = GetWorld()->SpawnActor<AItem>();
	item->Init(_itemType, MapPtr, NavIdx);
	item->SetActorLocation(GetActorLocation() + FVector{ 0.f, item->Size, 0.f } + (backward * item->Size * 2));
}