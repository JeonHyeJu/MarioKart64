#include "PreCompile.h"
#include "TimeEventComponent.h"

UTimeEventComponent::UTimeEventComponent()
{
}

UTimeEventComponent::~UTimeEventComponent()
{
}

void UTimeEventComponent::AddEndEvent(float _Time, std::function<void()> _TimeEndEvent, bool _Loop)
{
	AddEvent(_Time, nullptr, _TimeEndEvent, _Loop);
}

void UTimeEventComponent::AddUpdateEvent(float _Time, std::function<void(float, float)> _TimeUpdateEvent, bool _Loop
)
{
	AddEvent(_Time, _TimeUpdateEvent, nullptr, _Loop);
}

void UTimeEventComponent::AddEvent(float _Time, std::function<void(float, float)> _TimeUpdateEvent, std::function<void()> _TimeEndEvent, bool _Loop /*= false*/)
{
	FTimeEvent& NewEvent = Events.emplace_back();
	NewEvent.MaxTime = _Time;
	NewEvent.CurTime = 0.0f;
	NewEvent.Loop = _Loop;
	NewEvent.TimeEndEvent = _TimeEndEvent;
	NewEvent.TimeUpdateEvent = _TimeUpdateEvent;

};

void UTimeEventComponent::BeginPlay()
{

}

void UTimeEventComponent::ComponentTick(float _DeltaTime)
{
	UActorComponent::ComponentTick(_DeltaTime);

	for (FTimeEvent& TimeEvent : Events)
	{
		TimeEvent.CurTime += _DeltaTime;

		if (nullptr != TimeEvent.TimeUpdateEvent)
		{
			TimeEvent.TimeUpdateEvent(_DeltaTime, TimeEvent.CurTime);
		}

		if (TimeEvent.CurTime > TimeEvent.MaxTime)
		{
			if (nullptr == TimeEvent.TimeEndEvent)
			{
				continue;
			}
			TimeEvent.TimeEndEvent();
		}
	}

	std::list<FTimeEvent>::iterator StartIter = Events.begin();
	std::list<FTimeEvent>::iterator EndIter = Events.end();

	for ( ; StartIter != EndIter; )
	{
		FTimeEvent& TimeEvent = *StartIter;

		if (TimeEvent.CurTime < TimeEvent.MaxTime)
		{
			++StartIter;
			continue;
		}

		if (false == TimeEvent.Loop)
		{
			StartIter = Events.erase(StartIter);
		}
		else 
		{
			++StartIter;
			TimeEvent.CurTime = 0.0f;
		}
	}

}
