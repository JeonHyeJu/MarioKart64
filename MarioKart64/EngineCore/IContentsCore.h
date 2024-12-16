#pragma once
#include <EngineBase/EngineMath.h>

struct UEngineInitData
{
public:
	FVector WindowPos;
	FVector WindowSize;
};

class IContentsCore
{
public:
	virtual ~IContentsCore() = 0 {};

	virtual void EngineStart(UEngineInitData& _Data) = 0 {};
	virtual void EngineTick(float _DeltaTime) {};
	virtual void EngineEnd() {};
};

#define CreateContentsCoreDefine(TYPE) \
STDAPI_(__declspec(dllexport) INT_PTR) __stdcall CreateContentsCore(std::shared_ptr<IContentsCore>& _Test) \
{ \
	_Test = std::make_shared<TYPE>(); \
	if (nullptr == _Test) \
	{ \
		MSGASSERT("컨텐츠 모듈 생성에 실패했습니다."); \
	} \
	return 0; \
}