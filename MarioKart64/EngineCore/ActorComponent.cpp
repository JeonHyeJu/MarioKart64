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
		MSGASSERT("�θ� �������� �ʴ� ������Ʈ�� �����մϴ�");
		return false;
	}

	bool ret = UObject::IsActive() && actor->IsActive();

	AActor* parent = actor->GetParent();
	if (parent != nullptr)
	{
		ret = ret && parent->IsActive();
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