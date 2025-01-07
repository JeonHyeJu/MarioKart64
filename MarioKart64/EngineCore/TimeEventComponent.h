#pragma once
#include "ActorComponent.h"

struct FTimeEvent
{
public:
	bool Loop = false;
	float CurTime = 0.0f;
	float MaxTime = 0.0f;
	std::function<void(float, float)> TimeUpdateEvent;
	std::function<void()> TimeEndEvent;
};

class UTimeEventComponent : public UActorComponent
{
public:
	ENGINEAPI UTimeEventComponent();
	ENGINEAPI ~UTimeEventComponent();

	UTimeEventComponent(const UTimeEventComponent& _Other) = delete;
	UTimeEventComponent(UTimeEventComponent&& _Other) noexcept = delete;
	UTimeEventComponent& operator=(const UTimeEventComponent& _Other) = delete;
	UTimeEventComponent& operator=(UTimeEventComponent&& _Other) noexcept = delete;

	ENGINEAPI void AddEvent(float _Time, std::function<void(float, float)> _TimeUpdateEvent, std::function<void()> _TimeEndEvent, bool _Loop = false
	);

	ENGINEAPI void AddEndEvent(float _Time, std::function<void()> _TimeEndEvent, bool _Loop = false);

	ENGINEAPI void AddUpdateEvent(float _Time, std::function<void(float, float)> _TimeUpdateEvent,		bool _Loop = false
	);

protected:
	ENGINEAPI void BeginPlay() override;
	ENGINEAPI void ComponentTick(float _DeltaTime) override;

private:
	std::list<FTimeEvent> Events;
};

