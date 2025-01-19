#include "PreCompile.h"
#include "EngineWorkThreadPool.h"

class UWork
{
public:
	std::function<void()> Function;
};

UEngineWorkThreadPool::UEngineWorkThreadPool()
{
}

UEngineWorkThreadPool::~UEngineWorkThreadPool()
{
	IsWork = false;

	while (0 < RunningCount)
	{
		PostQueuedCompletionStatus(IOCPHandle, static_cast<DWORD>(EThreadStatus::Destroy), 0, nullptr);
	}

}

void UEngineWorkThreadPool::WorkQueue(std::function<void()> _Work)
{
	UWork* NewWork = new UWork();
	NewWork->Function = _Work;

	PostQueuedCompletionStatus(IOCPHandle, static_cast<DWORD>(EThreadStatus::Work), reinterpret_cast<ULONG_PTR>(NewWork), nullptr);
}

void UEngineWorkThreadPool::Initialize(std::string_view ThreadName /*= "WorkThread"*/, int Count /*= 0*/)
{
	ThreadCount = Count;

	if (0 >= Count)
	{
		SYSTEM_INFO Info;
		GetSystemInfo(&Info);
		ThreadCount = Info.dwNumberOfProcessors;
	}

	RunningCount = ThreadCount;

	IOCPHandle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, 0, 0, 0);

	if (nullptr == IOCPHandle)
	{
		MSGASSERT("IOCP 핸들 생성에 실패했습니다.");
	}

	Threads.resize(ThreadCount);
	for (size_t i = 0; i < ThreadCount; i++)
	{
		Threads[i] = std::make_shared<UEngineThread>();

		Threads[i]->Start(std::string(ThreadName) + std::to_string(i), std::bind(ThreadQueueFunction, IOCPHandle, this));
	}
}

void UEngineWorkThreadPool::ThreadQueueFunction(HANDLE _IOCPHandle, UEngineWorkThreadPool* _JobQueue)
{
	unsigned long Byte = 0;
	unsigned __int64 Ptr;

	LPOVERLAPPED OverPtr = nullptr;

	while (_JobQueue->IsWork)
	{
		GetQueuedCompletionStatus(_IOCPHandle, &Byte, &Ptr, &OverPtr, INFINITE);

		if (-1 == Byte)
		{
			break;
		}

		if (-2 == Byte)
		{
			UWork* Work = reinterpret_cast<UWork*>(Ptr);

			if (nullptr != Work)
			{
				Work->Function();
			}

			delete Work;
		}
	}

	_JobQueue->RunningCount -= 1;
}