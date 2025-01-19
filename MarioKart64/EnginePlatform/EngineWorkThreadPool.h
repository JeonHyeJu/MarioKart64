#pragma once
#include <functional>
#include "EngineThread.h"
#include <atomic>

enum class EThreadStatus
{
	Work = -2,
	Destroy = -1,
};

class UEngineWorkThreadPool
{
public:
	ENGINEAPI UEngineWorkThreadPool();
	ENGINEAPI ~UEngineWorkThreadPool();

	UEngineWorkThreadPool(const UEngineWorkThreadPool& _Other) = delete;
	UEngineWorkThreadPool(UEngineWorkThreadPool&& _Other) noexcept = delete;
	UEngineWorkThreadPool& operator=(const UEngineWorkThreadPool& _Other) = delete;
	UEngineWorkThreadPool& operator=(UEngineWorkThreadPool&& _Other) noexcept = delete;

	ENGINEAPI void Initialize(std::string_view ThreadName = "WorkThread", int Count = 0);

	ENGINEAPI void WorkQueue(std::function<void()> _Work);

protected:

private:
	std::atomic<int> RunningCount;
	std::atomic<bool> IsWork = true;

	HANDLE IOCPHandle = nullptr;

	int ThreadCount = 0;
	std::vector<std::shared_ptr<UEngineThread>> Threads;

	static void ThreadQueueFunction(HANDLE _IOCPHandle, UEngineWorkThreadPool* _JobQueue);
};

