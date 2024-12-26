#include "PreCompile.h"
#include "EngineTimer.h"

UEngineTimer::UEngineTimer()
{
	QueryPerformanceFrequency(&Count);

	TimeCounter = static_cast<double>(Count.QuadPart);

	QueryPerformanceCounter(&PrevTime);
	QueryPerformanceCounter(&CurTime);

	int a = 0;
}

UEngineTimer::~UEngineTimer()
{
}

void UEngineTimer::TimeStart()
{
	QueryPerformanceCounter(&PrevTime);
}

float UEngineTimer::End()
{
	TimeCheck();
	return GetDeltaTime();
}

double UEngineTimer::DEnd()
{
	TimeCheck();
	return GetDoubleDeltaTime();
}

void UEngineTimer::TimeCheck()
{
	// QueryPerformanceCounter(&PrevTime); 이전에 재었던 시간이 남아있을 것이다.

	QueryPerformanceCounter(&CurTime);

	// 사이간격이 나옵니다.
	double Tick = static_cast<double>(CurTime.QuadPart - PrevTime.QuadPart);
	DeltaTime = Tick / TimeCounter * TimeScale;
	fDeltaTime = static_cast<float>(DeltaTime);
	PrevTime.QuadPart = CurTime.QuadPart;
}
