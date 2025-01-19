#include "PreCompile.h"
#include "Player.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineInput.h>
#include <EngineCore/Collision.h>
#include "TestMap.h"
#include "GameData.h"
#include "CData.h"
#include "Item.h"
#include "CGlobal.h"

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
	//RendererDebug->ColorData.MulColor = { 0.f, 0.f, 0.f, 0.f };

	float playerWidth = Renderer->GetWorldScale3D().X;

	CollisionItem = CreateDefaultSubObject<UCollision>();
	CollisionItem->SetCollisionType(ECollisionType::Sphere);
	CollisionItem->SetupAttachment(RootComponent);
	CollisionItem->SetCollisionProfileName("Player");
	CollisionItem->SetScale3D({ playerWidth, playerWidth, playerWidth });
	CollisionItem->AddRelativeLocation({0.f, playerWidth*.5f, 0.f });

	CollisionItem->SetCollisionEnter(std::bind(&APlayer::OnCollisionEnter, this, std::placeholders::_1, std::placeholders::_2));

	DebugItem = CreateDefaultSubObject<USpriteRenderer>();
	DebugItem->SetOrder(0);
	DebugItem->SetupAttachment(RootComponent);
	DebugItem->SetSprite("Items.png", static_cast<int>(EItemType::SIZE));
	DebugItem->SetRelativeLocation({ 30.f, 50.f, 0.f });
}

APlayer::~APlayer()
{
}

void APlayer::BeginPlay()
{
	AActor::BeginPlay();
}

void APlayer::Tick(float _deltaTime)
{
	APawn::Tick(_deltaTime);

	Move(_deltaTime);

	/* for debug start */
	if (UEngineInput::IsDown(VK_SPACE))
	{
		std::shared_ptr<AItem> item = GetWorld()->SpawnActor<AItem>();
		item->Init(EItemType::FAKE_ITEMBOX);
		item->SetActorLocation(GetActorLocation() + FVector{ -item->Size * .5f, item->Size, 0.f });
		item->SetDirection(GetActorForwardVector());
	}
	/* for debug end */

	if (IsPickingItem)
	{
		PickItem(_deltaTime);
	}
	else
	{
		CheckUsingItem(_deltaTime);
	}
}

void APlayer::Move(float _deltaTime)
{
	float gravityY = GRAVITY_FORCE * _deltaTime;
	FTransform trfmPlayer = GetActorTransform();
	FTransform trfmObj = TestMapPtr->GetActorTransform();

	/* for debug start */
	if (UEngineInput::IsPress(VK_LCONTROL))
	{
		SetActorLocation({ 307.417f, 159.055f, 3478.909f });
		SetActorRotation({ 0.f, -110.f, 0.f });
		TestMapPtr->SetNavIndex(-1);
		return;
	}
	if (trfmPlayer.Location.Y < -1000)
	{
		Velocity = 0.f;
		SetActorLocation({ 307.417f, 159.055f, 3478.909f });
		SetActorRotation({ 0.f, -110.f, 0.f });
		TestMapPtr->SetNavIndex(-1);
		return;
	}
	if (UEngineInput::IsPress(VK_LSHIFT))
	{
		Velocity = MAX_VELOCITY;
	}
	/* for debug end */

	int navIdx = TestMapPtr->GetNavIndex();
	if (navIdx == -1)
	{
		AddActorLocation({ 0.f, gravityY, 0.f });
		CheckCollisionOfAllMap();
		return;
	}

	float fDist = 0.f;
	bool isCollided = CheckCollision(trfmPlayer.Location, navIdx, fDist);

	FVector lastRot;
	FVector lastVec;
	FVector dir = GetActorForwardVector();
	dir.Normalize();

	float dx = 0.f;
	float rotVal = 0.f;
	float slopeAngle = 0.f;

	/* for debug start */
	/*{
		GetForwardPhysics(_deltaTime, dx);
		GetHandleRotation(_deltaTime, rotVal);

		dir *= dx;
		lastVec = dir;
		lastRot.Y = rotVal;

		float zVal = 0.f;
		if (UEngineInput::IsPress('Z'))
		{
			zVal = 10.f;
		}
		else if (UEngineInput::IsPress('X'))
		{
			zVal = -10.f;
		}
		if (UEngineInput::IsPress('W'))
		{
			lastVec.Z += 500.f * _deltaTime;
		}
		else if (UEngineInput::IsPress('S'))
		{
			lastVec.Z += -500.f * _deltaTime;
		}
		if (UEngineInput::IsPress('A'))
		{
			lastVec.X += -500.f * _deltaTime;
		}
		else if (UEngineInput::IsPress('D'))
		{
			lastVec.X += 500.f * _deltaTime;
		}
		lastVec.Y = zVal;

		AddActorRotation(lastRot);
		AddActorLocation(lastVec);
		return;
	}*/
	/* for debug end */

	if (isCollided)
	{
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

		GetForwardPhysics(_deltaTime, dx);	// Z
		GetHandleRotation(_deltaTime, rotVal);	// X

		dir *= dx;		// Z, X
		lastVec = dir;

		lastRot.Y = rotVal;		// X
		lastVec.Y += fDist;		// Y

		VelocityV = Velocity * .5f * sinf(-slopeAngle * UEngineMath::D2R);

		CheckLab();
	}
	else
	{
		// I didn't multiply deltaTime intentionally.
		bool hasFloor = CheckCollision(trfmPlayer.Location + FVector{ 0.f, GRAVITY_FORCE, 0.f }, navIdx, fDist);

		if (hasFloor)
		{
			// Adjust gravity
			GetForwardPhysics(_deltaTime, dx);
			GetHandleRotation(_deltaTime, rotVal);

			// for fDist. Do not remove
			CheckCollision(trfmPlayer.Location + FVector{ 0.f, gravityY, 0.f }, navIdx, fDist);

			dir *= dx;
			lastVec = dir;

			lastRot.Y = rotVal;
			lastVec.Y += gravityY + fDist;
		}
		else
		{
			// Block
			if (TestMapPtr->GetNavData(PrevIdx).FloorType != NavType::FLATE_FASTER)
			{
				dir.X *= -1;
				dir.Z *= -1;
				dir *= 5000.f * _deltaTime;

				Velocity *= .5f;

				// TODO: handle corner backword
				lastVec = dir;
			}
			else
			{
				IsTouchLastTriangle = true;

				// Adjust gravity
				GetForwardPhysics(_deltaTime, dx);

				VelocityV = FPhysics::GetVf(VelocityV, gravityY * 150.f, _deltaTime);
				float dy = FPhysics::GetDeltaX(VelocityV, gravityY * 150.f, _deltaTime);

				dir *= dx;
				lastVec = dir;

				lastVec.Y += dy;
				OutputDebugStringA(("VelocityV: " + std::to_string(VelocityV) + ", gravityForce: " + std::to_string(gravityY) + ", dy: " + std::to_string(dy) + "\n").c_str());
			}
		}
	}

	AddActorRotation(lastRot);
	AddActorLocation(lastVec);

	//FTransform temp = GetTransform();
	//OutputDebugStringA(("Last location: " + std::to_string(temp.Location.X) + ", " + std::to_string(temp.Location.Y) + ", " + std::to_string(temp.Location.Z) + "\n").c_str());

	//OutputDebugStringA("------------------------------------------\n");
}

void APlayer::SetMap(class ATestMap* _ptr)
{
	TestMapPtr = _ptr;
}

void APlayer::CheckLab()
{
	if (!IsTouchLastTriangle) return;

	NavData nd = TestMapPtr->GetCurNavData();
	if (nd.FloorType == NavType::START_POINT)
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

float APlayer::GetSlope()
{
	float slopeAngle = 0.f;
	int idx = TestMapPtr->GetNavIndex();
	if (idx < 0) return 0.f;

	NavData nd = TestMapPtr->GetCurNavData();
	const FTransform& trfmObj = TestMapPtr->GetTransform();

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

void APlayer::GetHandleRotation(float _deltaTime, float& _refRot)
{
	if (UEngineInput::IsPress(VK_LEFT))
	{
		_refRot = -100.f * _deltaTime;
	}
	else if (UEngineInput::IsPress(VK_RIGHT))
	{
		_refRot = 100.f * _deltaTime;
	}
	//OutputDebugStringA(("rotVal: " + std::to_string(_refRot) + "\n").c_str());
}

void APlayer::GetForwardPhysics(float _deltaTime, float& _refDx, bool _isCollided)
{
	float acc = 0.f;
	float dx = 0.f;
	bool isPushed = true;

	if (UEngineInput::IsPress(VK_UP))
	{
		acc = ACCELERATION - FRICTION_FORCE;
	}
	else if (UEngineInput::IsPress(VK_DOWN))
	{
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
		dx = FPhysics::GetDeltaX(Velocity, acc, _deltaTime);
	}
	else
	{
		if (fabs(Velocity) > 30)
		{
			acc = (Velocity * .5f) - Velocity;
			Velocity = FPhysics::GetVf(Velocity, acc, _deltaTime);
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

bool APlayer::CheckCollision(const FVector& _loc, int& _refIdx, float& _refDist)
{
	bool isCollided = false;
	_refIdx = TestMapPtr->GetNavIndex();
	const FTransform& trfmObj = TestMapPtr->GetTransform();

	const std::vector<NavData>& navDatas = TestMapPtr->GetNavData();
	NavData nd = navDatas[_refIdx];
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

				TestMapPtr->SetNavIndex(linkedIdx);
				_refIdx = linkedIdx;
				break;
			}
		}
	}

	return isCollided;
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

void APlayer::OnCollisionEnter(UCollision* _this, UCollision* _other)
{
	const std::string& name = _other->GetCollisionProfileName();

	if (name == "ITEM")
	{
		//_other->GetActor()->Destroy();

		OutputDebugStringA("Ouch!!!!!!!!!!!!!!!!!!!\n");
	}
	else if (name == "ITEMBOX")
	{
		_other->GetActor()->Destroy();

		OutputDebugStringA("Get ITEM!!!!!!!!!!!!!!!!!!!\n");

		ItemRoulette.Reset();
		IsPickingItem = true;
	}
}

void APlayer::PickItem(float _deltaTime)
{
	static int prevItemIdx = -1;
	int itemIdx = ItemRoulette.PickItem(_deltaTime);
	if (itemIdx == -1)
	{
		//ItemIndex = static_cast<int>(EItemType::FAKE_ITEMBOX);	// for test
		ItemIndex = prevItemIdx;
		IsPickingItem = false;
		prevItemIdx = -1;
		return;
	}

	if (prevItemIdx != itemIdx)
	{
		prevItemIdx = itemIdx;
		GameData::GetInstance()->SetItem(0, static_cast<EItemType>(itemIdx));
		DebugItem->SetSprite("Items.png", itemIdx);	// Temp
	}
}

void APlayer::CheckUsingItem(float _deltaTime)
{
	static const int NONE = static_cast<int>(EItemType::SIZE);
	if (ItemIndex >= NONE) return;

	if (UEngineInput::IsDown(VK_SPACE))
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

		ItemIndex = NONE;
		DebugItem->SetSprite("Items.png", NONE);
	}
}

void APlayer::UseItem_Shell(const EItemType& _itemType)
{
	if (_itemType < EItemType::RED_SHELL)
	{
		// TODO
	}
	else if (_itemType < EItemType::BOWSER_SHELL)
	{
		// TODO
	}

	std::shared_ptr<AItem> item = GetWorld()->SpawnActor<AItem>();
	item->Init(_itemType);
	//item->SetActorRotation(GetActorRotation());
	item->SetActorLocation(GetActorLocation() + FVector{ -item->Size * .5f, item->Size, 0.f });
	item->SetDirection(GetActorForwardVector());
}

void APlayer::UseItem_Mushroom(const EItemType& _itemType)
{
	Velocity += 100.f;
}

void APlayer::UseItem_Banana(const EItemType& _itemType)
{

}

void APlayer::UseItem_Star(const EItemType& _itemType)
{
}

void APlayer::UseItem_Thunder(const EItemType& _itemType)
{
}

void APlayer::UseItem_Ghost(const EItemType& _itemType)
{
}

void APlayer::UseItem_FakeItemBox(const EItemType& _itemType)
{
}
