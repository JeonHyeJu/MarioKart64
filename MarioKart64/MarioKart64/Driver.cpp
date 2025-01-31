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
	//RendererDebug->ColorData.MulColor = { 0.f, 0.f, 0.f, 0.f };

	DebugItem = CreateDefaultSubObject<USpriteRenderer>();
	DebugItem->SetOrder(0);
	DebugItem->SetupAttachment(RootComponent);
	DebugItem->SetSprite("Items.png", static_cast<int>(EItemType::SIZE));
	DebugItem->SetRelativeLocation({ 30.f, 50.f, 0.f });

	for (size_t i = 0, size = TempRouteIdxInit.size(); i < size; ++i)
	{
		TempRouteIdx[TempRouteIdxInit[i]] = static_cast<int>(i);
	}
	LastIdx = TempRouteIdxInit.back();
	TempRouteIdxInit.clear();

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
	const char* SPRITE_NAME = RENDER_SPRITES[static_cast<int>(_character)];
	Renderer->CreateAnimation("Idle", SPRITE_NAME, 0, 3, .3f);

	{
		const int TURN_SIZE = 25;
		std::vector<int> idxs(TURN_SIZE, 1008);
		std::vector<float> times(TURN_SIZE, .05f);
		for (size_t i = 0; i < TURN_SIZE; ++i)
		{
			idxs[i] += static_cast<int>(i);
		}

		Renderer->CreateAnimation("Spin", SPRITE_NAME, idxs, times, false);
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

		Renderer->CreateAnimation("TurnL", SPRITE_NAME, idxs, times, false);
		Renderer->CreateAnimation("TurnR", SPRITE_NAME, idxs, times, false);
	}

	Renderer->ChangeAnimation("Idle");

	float playerWidth = Renderer->GetWorldScale3D().X;

	CollisionItem = CreateDefaultSubObject<UCollision>();
	CollisionItem->SetCollisionType(ECollisionType::Sphere);
	CollisionItem->SetupAttachment(RootComponent);
	CollisionItem->SetCollisionProfileName("Player");
	CollisionItem->SetScale3D({ playerWidth, playerWidth, playerWidth });
	CollisionItem->AddRelativeLocation({ 0.f, playerWidth * .5f, 0.f });
	CollisionItem->SetCollisionEnter(std::bind(&ADriver::OnCollisionEnter, this, std::placeholders::_1, std::placeholders::_2));
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
	if (IsBoost)
	{
		BoostVal += 2000.f * _deltaTime;
	}
	else
	{
		if (BoostVal > 0)
		{
			BoostVal -= 350.f * _deltaTime;
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
	bool isCol = CheckCollision(futureDir, FutureNavIdxs[i], FutureNavIdxs[i], tempFDist);
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

				isCol = CheckCollision(curLoc + newDir, FutureNavIdxs[i], FutureNavIdxs[i], tempFDist);
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

			isCol = CheckCollision(curLoc + newDir, FutureNavIdxs[i], FutureNavIdxs[i], tempFDist);
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

	OutputDebugStringA(("Move: " + trfmPlayer.Location.ToString() + "\n").c_str());

	/* for debug start */
	if (UEngineInput::IsPress(VK_LCONTROL))
	{
		SetActorLocation({ 4142.233887f, 70.f, 8755.953125f });
		SetActorRotation({ 0.f, -117.5f, 0.f });
		NavIdx = -1;
		return;
	}
	if (trfmPlayer.Location.Y < -1000)
	{
		Velocity = 0.f;
		BoostVal = 0.f;
		SetActorLocation({ 4142.233887f, 70.f, 8755.953125f });
		SetActorRotation({ 0.f, -117.5f, 0.f });
		NavIdx = -1;
		return;
	}
	if (UEngineInput::IsPress(VK_LSHIFT))
	{
		Velocity = MAX_VELOCITY;
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
	std::map<int, int>::iterator it = TempRouteIdx.find(NavIdx);
	std::map<int, int>::iterator itEnd = TempRouteIdx.end();
	if (it != itEnd)
	{
		int _idx = it->second;
		if (isLog) OutputDebugStringA(("navIdx: " + std::to_string(NavIdx)+", _idx: " + std::to_string(_idx) + "\n").c_str());
		if (CurRouteIdx > _idx)
		{
			//OutputDebugStringA("REVERSE!!!!!!!\n");
		}
		else if (CurRouteIdx < _idx)
		{
			CurRouteIdx = _idx;
		}
	}
	else
	{
		//OutputDebugStringA("EMPTY!!!!!!!\n");
	}

	if (TempPrevIdx != NavIdx)
	{
		//OutputDebugStringA(("navIdx: " + std::to_string(navIdx) + "\n").c_str());
		TempPrevIdx = NavIdx;
		//FileTemp << std::to_string(navIdx) << ",";
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
	GetForwardPhysics(_deltaTime, dx, true);

	if (IsAutomative)
	{
		GetHandleRotationAuto(_deltaTime, dir, rotVal);
	}
	else
	{
		GetHandleRotation(_deltaTime, rotVal);
	}

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
		CheckLab();
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
			if (MapPtr->GetNavData(PrevIdx).FloorType != ENavType::FLATE_FASTER)
			{
				Velocity *= .5f;

				FVector curLoc = GetActorLocation();
				FVector subLoc = PrevLoc - curLoc;
				subLoc.Normalize();
				lastVec = subLoc * Velocity * .5f;
			}
			else
			{
				IsTouchLastTriangle = true;

				// Adjust gravity
				//GetForwardPhysics(_deltaTime, dx);

				VelocityV = FPhysics::GetVf(VelocityV, gravityY * 50.f, _deltaTime);
				float dy = FPhysics::GetDeltaX(VelocityV, gravityY * 50.f, _deltaTime);

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

	//OutputDebugStringA("------------------------------------------\n");
}

void ADriver::SetMap(ABaseMap* _ptr)
{
	MapPtr = _ptr;
}

void ADriver::CheckLab()
{
	if (!IsTouchLastTriangle) return;

	SNavData nd = MapPtr->GetNavData(NavIdx);
	if (nd.FloorType == ENavType::START_POINT)
	{
		++Lab;
		OutputDebugStringA(("==================== ++Lab: " + std::to_string(Lab) + "\n").c_str());
		IsTouchLastTriangle = false;
	}

	//if (Lab == ALL_LAB)
	//{
	//	// change state
	//}
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
		acc = ACCELERATION - FRICTION_FORCE;
	}
	else if (UEngineInput::IsPress(VK_DOWN))
	{
		DirVTrain = 2;
		acc = -ACCELERATION + FRICTION_FORCE;
	}
	else
	{
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

bool ADriver::CheckCollision(const FVector& _loc, int _startIdx, int& _refIdx, float& _refDist)
{
	bool isCollided = false;
	const FTransform& trfmObj = MapPtr->GetTransform();

	// TODO: Check size
	const std::vector<SNavData>& navDatas = MapPtr->GetNavData();
	SNavData nd = navDatas[_startIdx];
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

	if (name == "ITEM")
	{
		_other->GetActor()->Destroy();

		IsSpin = true;
	}
	else if (name == "ITEMBOX")
	{
		_other->GetActor()->SetActive(false);

		ItemRoulette.Reset();
		IsPickingItem = true;
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
		GameData::GetInstance()->SetItem(0, static_cast<EItemType>(itemIdx));
		DebugItem->SetSprite("Items.png", itemIdx);	// Temp
	}
}

void ADriver::TickItem(float _deltaTime)
{
	static const int NONE = static_cast<int>(EItemType::SIZE);
	if (ItemIndex >= NONE) return;

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

	static const int NONE = static_cast<int>(EItemType::SIZE);
	ItemIndex = NONE;
	DebugItem->SetSprite("Items.png", NONE);
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
	item->Init(_itemType);
	item->SetActorRotation(GetActorRotation());
	item->SetActorLocation(GetActorLocation() + (forward * item->Size));
	item->SetInitVelocity(Velocity + 1000.f);
	item->SetDirection(forward);
}

void ADriver::UseItem_Mushroom(const EItemType& _itemType)
{
	Velocity += 100.f;
}

void ADriver::UseItem_Banana(const EItemType& _itemType)
{

}

void ADriver::UseItem_Star(const EItemType& _itemType)
{
}

void ADriver::UseItem_Thunder(const EItemType& _itemType)
{
}

void ADriver::UseItem_Ghost(const EItemType& _itemType)
{
}

void ADriver::UseItem_FakeItemBox(const EItemType& _itemType)
{
}
