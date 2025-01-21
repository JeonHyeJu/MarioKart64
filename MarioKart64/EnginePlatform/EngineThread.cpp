#include "PreCompile.h"
#include "EngineThread.h"
#include <EngineBase/EngineDebug.h>
#include <EngineBase/EngineString.h>

UEngineThread::UEngineThread()
{
}

UEngineThread::~UEngineThread()
{
	Join();
}

void UEngineThread::ThreadBaseFunction(UEngineThread* _Thread)
{
	std::wstring WName = UEngineString::AnsiToUnicode(_Thread->Name);
	SetThreadDescription(GetCurrentThread(), WName.c_str());

	_Thread->ThreadFunction();
}

bool UEngineThread::Start(std::string _Name, std::function<void()> _Function)
{
	if (true == ThreadInst.joinable())
	{
		return false;
	}

	if (nullptr == _Function)
	{
		MSGASSERT("함수를 세팅하지 않은 쓰레드는 start 할수 없습니다.");
		return false;
	}

	Name = _Name;
	ThreadFunction = _Function;
	ThreadInst = std::thread(std::bind(ThreadBaseFunction, this));
	return true;
}

void UEngineThread::Join()
{
	if (true == ThreadInst.joinable())
	{
		ThreadInst.join();
	}
}
