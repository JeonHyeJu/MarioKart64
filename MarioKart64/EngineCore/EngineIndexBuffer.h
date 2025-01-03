#pragma once
#include "EngineResources.h"
#include "EngineDeviceBuffer.h"

class UEngineIndexBuffer : public UEngineResources, public UEngineDeviceBuffer
{
public:
	UEngineIndexBuffer();
	~UEngineIndexBuffer();

	UEngineIndexBuffer(const UEngineIndexBuffer& _Other) = delete;
	UEngineIndexBuffer(UEngineIndexBuffer&& _Other) noexcept = delete;
	UEngineIndexBuffer& operator=(const UEngineIndexBuffer& _Other) = delete;
	UEngineIndexBuffer& operator=(UEngineIndexBuffer&& _Other) noexcept = delete;

	template<typename IndexType>
	static std::shared_ptr<UEngineIndexBuffer> Create(std::string_view _Name, const std::vector<IndexType>& _Data)
	{
		return Create(_Name, reinterpret_cast<const void*>(&_Data[0]), sizeof(IndexType), _Data.size());
	}

	static std::shared_ptr<UEngineIndexBuffer> Create(std::string_view _Name, const void* _InitData, size_t _VertexSize, size_t _VertexCount);

	void Setting();

	ENGINEAPI UINT GetIndexCount()
	{
		return IndexCount;
	}

protected:
	void ResCreate(const void* _InitData, size_t _VertexSize, size_t _VertexCount);

private:
	UINT IndexSize = 0;
	UINT IndexCount = 0;
	DXGI_FORMAT Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
};

