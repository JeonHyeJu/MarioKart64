#include "PreCompile.h"
#include "ActorComponent.h"
#include "Actor.h"

UActorComponent::UActorComponent()
{
}

UActorComponent::~UActorComponent()
{
}

bool UActorComponent::IsActive() 
{
	AActor* actor = GetActor();
	if (nullptr == actor)
	{
		MSGASSERT("부모가 존재하지 않는 컴포넌트가 존재합니다");
		return false;
	}

	bool ret = UObject::IsActive() && actor->IsActive();

	AActor* parent = actor->GetParent();
	while (parent != nullptr)
	{
		ret = ret && parent->IsActive();
		parent = parent->GetParent();
	}

	return ret;
	//return UObject::IsActive() && GetActor()->IsActive();
}

bool UActorComponent::IsDestroy() 
{
	return UObject::IsDestroy() || GetActor()->IsDestroy();
}

class AActor* UActorComponent::GetActor()
{
	return Actor;
}

ULevel* UActorComponent::GetWorld()
{
	return Actor->GetWorld();
}