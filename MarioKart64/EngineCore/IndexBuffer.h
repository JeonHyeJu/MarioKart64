#pragma once
#include "EngineResources.h"

class UIndexBuffer : public UEngineResources
{
public:
	UIndexBuffer();
	~UIndexBuffer();

	UIndexBuffer(const UIndexBuffer& _Other) = delete;
	UIndexBuffer(UIndexBuffer&& _Other) noexcept = delete;
	UIndexBuffer& operator=(const UIndexBuffer& _Other) = delete;
	UIndexBuffer& operator=(UIndexBuffer&& _Other) noexcept = delete;

	template<typename IndexType>
	static std::shared_ptr<UIndexBuffer> Create(std::string_view _Name, const std::vector<IndexType>& _Data)
	{
		return Create(_Name, reinterpret_cast<const void*>(&_Data[0]), sizeof(IndexType), _Data.size());
	}

	static std::shared_ptr<UIndexBuffer> Create(std::string_view _Name, const void* _InitData, size_t _VertexSize, size_t _VertexCount);

	void Setting();

protected:
	void ResCreate(const void* _InitData, size_t _VertexSize, size_t _VertexCount);

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer = nullptr;
	D3D11_BUFFER_DESC BufferInfo = { 0 };
	UINT IndexSize = 0;
	UINT IndexCount = 0;
	DXGI_FORMAT Format = DXGI_FORMAT::DXGI_FORMAT_R32_UINT;
};

