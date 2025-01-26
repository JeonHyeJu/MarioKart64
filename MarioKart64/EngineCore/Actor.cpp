#include "PreCompile.h"
#include "Actor.h"
#include "SceneComponent.h"

UGameInstance* AActor::GetGameInstance()
{
	return GEngine->GetGameInstance();
}

AActor::AActor()
{
}

AActor::~AActor()
{
	RootComponent = nullptr;
}

void AActor::BeginPlay()
{
	if (nullptr != RootComponent)
	{
		RootComponent->BeginPlay();
	}

	for (std::shared_ptr<class UActorComponent>& ActorComponent : ActorComponentList)
	{
		ActorComponent->BeginPlay();
	}
	
}

void AActor::Tick(float _DeltaTime)
{
	for (std::shared_ptr<AActor>& Actor : ChildList)
	{
		Actor->Tick(_DeltaTime);
	}

	if (nullptr == Parent)
	{
		if (nullptr != RootComponent)
		{
			RootComponent->ComponentTick(_DeltaTime);
		}
	}

	for (std::shared_ptr<class UActorComponent> ActorComponent : ActorComponentList)
	{
		if (false == ActorComponent->IsActive())
		{
			continue;
		}

		ActorComponent->ComponentTick(_DeltaTime);
	}
}

void AActor::AttachToActor(AActor* _Parent)
{
	if (nullptr == RootComponent)
	{
		MSGASSERT("씬 컴포넌트가 루트가 아닌 액터가 부모를 가질수 없습니다.");
		return;
	}

	if (nullptr == _Parent->RootComponent)
	{
		MSGASSERT("씬 컴포넌트가 루트가 아닌 액터가 부모를 가질수 없습니다.");
		return;
	}

	Parent = _Parent;

	_Parent->ChildList.push_back(GetThis<AActor>());
	RootComponent->SetupAttachment(_Parent->RootComponent);
}

void AActor::DetachFromActor()
{
	if (nullptr == Parent)
	{
		// Already detached.
		return;
	}

	if (nullptr == RootComponent)
	{
		MSGASSERT("RootComponent is empty.");
		return;
	}

	if (nullptr == Parent->RootComponent)
	{
		MSGASSERT("Parent's RootComponent is nullptr.");
		return;
	}

	Parent->ChildList.remove(GetThis<AActor>());
	RootComponent->RemoveAttachment();
	Parent = nullptr;
}

FVector AActor::GetActorUpVector()
{
	if (nullptr == RootComponent)
	{
		return FVector(0.0f, 0.0f, 0.0f, 1.0f);
	}

	return RootComponent->GetTransformRef().World.GetUp();
}

FVector AActor::GetActorRightVector()
{
	if (nullptr == RootComponent)
	{
		return FVector(0.0f, 0.0f, 0.0f, 1.0f);
	}
	return RootComponent->GetTransformRef().World.GetRight();
}

FVector AActor::GetActorForwardVector()
{
	if (nullptr == RootComponent)
	{
		return FVector(0.0f, 0.0f, 0.0f, 1.0f);
	}
	return RootComponent->GetTransformRef().World.GetFoward();
}