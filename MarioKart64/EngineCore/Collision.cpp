#include "PreCompile.h"
#include "Collision.h"
#include <EngineBase/EngineString.h>
#include <EngineCore/EngineCamera.h>

UCollision::UCollision()
{
}

UCollision::~UCollision()
{
	for (UCollision* Other : CollisionCheckSet)
	{
		Other->CollisionCheckSet.erase(this);
	}
}

void UCollision::SetCollisionProfileName(std::string_view _ProfileName)
{
	if (_ProfileName == GetCollisionProfileName())
	{
		return;
	}

	std::string UpperName = UEngineString::ToUpper(_ProfileName);

	std::string PrevProfileName = GetCollisionProfileName();
	ProfileName = UpperName;
	ULevel* Level = GetActor()->GetWorld();

	std::shared_ptr<UCollision> ThisPtr = GetThis<UCollision>();
	Level->ChangeCollisionProfileName(ProfileName, PrevProfileName, ThisPtr);
}

bool UCollision::CollisionCheck(std::string_view _OtherName, std::vector<UCollision*>& _Vector)
{
	std::string UpperName = UEngineString::ToUpper(_OtherName);

	std::map<std::string, std::list<std::shared_ptr<class UCollision>>>& Collision = GetWorld()->Collisions;

	if (false == Collision.contains(UpperName))
	{
		MSGASSERT("존재하지 않는 그룹과 충돌할수 없습니다" + std::string(UpperName));
		return false;
	}

	std::list<std::shared_ptr<class UCollision>>& Group = Collision[UpperName];

	for (std::shared_ptr<class UCollision>& OtherCol : Group)
	{
		if (false == OtherCol->IsActive())
		{
			continue;
		}

		if (true == FTransform::Collision(CollisionType, Transform, OtherCol->CollisionType, OtherCol->Transform))
		{
			_Vector.push_back(OtherCol.get());
		}
	}

	return 0 != _Vector.size();
}

bool UCollision::CollisionCheck(std::string_view _OtherName, FVector _NextPos, std::vector<UCollision*>& _Vector)
{
	std::string UpperName = UEngineString::ToUpper(_OtherName);

	std::map<std::string, std::list<std::shared_ptr<class UCollision>>>& Collision = GetWorld()->Collisions;

	if (false == Collision.contains(UpperName))
	{
		MSGASSERT("존재하지 않는 그룹과 충돌할수 없습니다" + std::string(UpperName));
		return false;
	}

	FTransform NextTransform = Transform;

	NextTransform.Location += _NextPos;
	NextTransform.TransformUpdate();

	std::list<std::shared_ptr<class UCollision>>& Group = Collision[UpperName];

	for (std::shared_ptr<class UCollision>& OtherCol : Group)
	{
		if (false == OtherCol->IsActive())
		{
			continue;
		}

		if (true == FTransform::Collision(CollisionType, NextTransform, OtherCol->CollisionType, OtherCol->Transform))
		{
			_Vector.push_back(OtherCol.get());
		}
	}

	return 0 != _Vector.size();
}


void UCollision::BeginPlay()
{
	USceneComponent::BeginPlay();
}

void UCollision::SetRadius(float _Value)
{
	FVector Scale = GetWorldScale3D();
	Scale.X = _Value * 2.0f;
	SetScale3D(Scale);
}

void UCollision::SetCollisionEnter(std::function<void(UCollision*, UCollision*)> _Function)
{
	if ("NONE" == GetCollisionProfileName())
	{
		MSGASSERT("아직 충돌 그룹이 지정되지 않은 충돌체를 이벤트 등록하려고 했습니다.");
		return;
	}

	if (true == IsEvent())
	{
		Enter = _Function;
		return;
	}

	Enter = _Function;
	ULevel* Level = GetActor()->GetWorld();
	std::shared_ptr<UCollision> ThisPtr = GetThis<UCollision>();

	Level->CheckCollisions[GetCollisionProfileName()].push_back(ThisPtr);
}

void UCollision::SetCollisionStay(std::function<void(UCollision*, UCollision*)> _Function)
{
	if ("NONE" == GetCollisionProfileName())
	{
		MSGASSERT("아직 충돌 그룹이 지정되지 않은 충돌체를 이벤트 등록하려고 했습니다.");
		return;
	}

	if (true == IsEvent())
	{
		Stay = _Function;
		return;
	}

	Stay = _Function;
	ULevel* Level = GetActor()->GetWorld();
	std::shared_ptr<UCollision> ThisPtr = GetThis<UCollision>();

	Level->CheckCollisions[GetCollisionProfileName()].push_back(ThisPtr);
}

void UCollision::SetCollisionEnd(std::function<void(UCollision*, UCollision*)> _Function)
{
	if ("NONE" == GetCollisionProfileName())
	{
		MSGASSERT("아직 충돌 그룹이 지정되지 않은 충돌체를 이벤트 등록하려고 했습니다.");
		return;
	}

	if (true == IsEvent())
	{
		End = _Function;
		return;
	}

	End = _Function;
	ULevel* Level = GetActor()->GetWorld();
	std::shared_ptr<UCollision> ThisPtr = GetThis<UCollision>();
	Level->CheckCollisions[GetCollisionProfileName()].push_back(ThisPtr);
}

void UCollision::CollisionEventCheck(std::shared_ptr<UCollision> _Other)
{
	if (false == _Other->IsActive())
	{
		return;
	}

	if (true == FTransform::Collision(CollisionType, Transform, _Other->CollisionType, _Other->Transform))
	{
		if (false ==  CollisionCheckSet.contains(_Other.get()))
		{
			CollisionCheckSet.insert(_Other.get());
			_Other->CollisionCheckSet.insert(this);
			if (nullptr != Enter)
			{
				Enter(this, _Other.get());
			}
		}
		else 
		{
			if (nullptr != Stay)
			{
				Stay(this, _Other.get());
			}
		}
	}
	else
	{
		if (true == CollisionCheckSet.contains(_Other.get()))
		{
			if (nullptr != End)
			{
				End(this, _Other.get());
			}

			CollisionCheckSet.erase(_Other.get());
			_Other->CollisionCheckSet.erase(this);
		}
	}
}

void UCollision::DebugRender(UEngineCamera* _Camera, float _DeltaTime)
{
	URenderUnit Unit;

	FTransform& CameraTrans = _Camera->GetTransformRef();
	FTransform& RendererTrans = GetTransformRef();

	RendererTrans.View = CameraTrans.View;
	RendererTrans.Projection = CameraTrans.Projection;
	RendererTrans.WVP = RendererTrans.World * RendererTrans.View * RendererTrans.Projection;

	Unit.SetMesh("Rect");
	Unit.SetMaterial("CollisionDebugMaterial");

	Unit.ConstantBufferLinkData("FTransform", GetTransformRef());
	FVector Color = {0.0f, 1.0f, 0.0f};
	Unit.ConstantBufferLinkData("OutColor", Color);

	Unit.Render(_Camera, _DeltaTime);

}