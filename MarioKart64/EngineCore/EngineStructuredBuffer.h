#pragma once
#include "EngineResources.h"
#include "EngineDeviceBuffer.h"
#include "EngineEnums.h"

class UEngineStructuredBuffer : public UEngineResources, public UEngineDeviceBuffer
{
	friend class UEngineShaderResources;

public:
	UEngineStructuredBuffer();
	~UEngineStructuredBuffer();

	UEngineStructuredBuffer(const UEngineStructuredBuffer& _Other) = delete;
	UEngineStructuredBuffer(UEngineStructuredBuffer&& _Other) noexcept = delete;
	UEngineStructuredBuffer& operator=(const UEngineStructuredBuffer& _Other) = delete;
	UEngineStructuredBuffer& operator=(UEngineStructuredBuffer&& _Other) noexcept = delete;

	static std::shared_ptr<UEngineStructuredBuffer> CreateOrFind(UINT _Byte, const std::string_view& _Name);

	void ChangeData(void* _Data, UINT _Size);
	void Setting(EShaderType _Type, UINT _BindIndex);

	static void Release();

protected:

private:
	static std::map<int, std::map<std::string, std::shared_ptr<UEngineStructuredBuffer>>> BufferMap;
	
	void ResCreate(UINT _DataSize, UINT _DataCount);

	UINT DataSize = 0;
	UINT DataCount = 0;

	ID3D11ShaderResourceView* SRV = nullptr;
};

