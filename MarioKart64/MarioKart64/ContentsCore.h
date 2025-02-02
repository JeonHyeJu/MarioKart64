#pragma once
#include <EngineCore/IContentsCore.h>

class UContentsCore : public IContentsCore
{
public:
	UContentsCore();
	~UContentsCore();

	UContentsCore(const UContentsCore& _Other) = delete;
	UContentsCore(UContentsCore&& _Other) noexcept = delete;
	UContentsCore& operator=(const UContentsCore& _Other) = delete;
	UContentsCore& operator=(UContentsCore&& _Other) noexcept = delete;

protected:
	void EngineStart(UEngineInitData& _Data);
	void EngineTick(float _DeltaTime);
	void EngineEnd();

private:
	void InitTextures(std::string_view _path);
	void InitSprites(std::string_view _path);
	void InitGraphics();
	void InitTest();
	void InitObjs();
	void InitSound(std::string_view _path);
};

