#pragma once

class UEngineDeviceBuffer
{
public:
	UEngineDeviceBuffer();
	~UEngineDeviceBuffer();

	UEngineDeviceBuffer(const UEngineDeviceBuffer& _Other) = delete;
	UEngineDeviceBuffer(UEngineDeviceBuffer&& _Other) noexcept = delete;
	UEngineDeviceBuffer& operator=(const UEngineDeviceBuffer& _Other) = delete;
	UEngineDeviceBuffer& operator=(UEngineDeviceBuffer&& _Other) noexcept = delete;

	const D3D11_BUFFER_DESC& GetBufferInfo()
	{
		return BufferInfo;
	}

protected:
	Microsoft::WRL::ComPtr<ID3D11Buffer> Buffer = nullptr;
	D3D11_BUFFER_DESC BufferInfo = { 0 };

private:
};

