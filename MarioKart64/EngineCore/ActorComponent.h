#pragma once
#include <EngineBase/Object.h>
#include <memory>

class UActorComponent : public UObject
{
	friend AActor;

public:
	UActorComponent();
	~UActorComponent();

	UActorComponent(const UActorComponent& _Other) = delete;
	UActorComponent(UActorComponent&& _Other) noexcept = delete;
	UActorComponent& operator=(const UActorComponent& _Other) = delete;
	UActorComponent& operator=(UActorComponent&& _Other) noexcept = delete;

	class AActor* GetActor();
	ENGINEAPI virtual void InitializeComponent() {}
	ENGINEAPI virtual void BeginPlay() {}

protected:

private:
	class AActor* Actor;
};

