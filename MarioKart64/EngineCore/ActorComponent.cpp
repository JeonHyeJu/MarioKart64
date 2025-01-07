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
	if (nullptr == GetActor())
	{
		MSGASSERT("부모가 존재하지 않는 컴포넌트가 존재합니다");
		return false;
	}

	return UObject::IsActive() && GetActor()->IsActive();
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