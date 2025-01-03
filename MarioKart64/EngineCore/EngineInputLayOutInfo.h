#pragma once

class UEngineInputLayOutInfo
{
public:
	UEngineInputLayOutInfo();
	~UEngineInputLayOutInfo();

	UEngineInputLayOutInfo(const UEngineInputLayOutInfo& _Other) = delete;
	UEngineInputLayOutInfo(UEngineInputLayOutInfo&& _Other) noexcept = delete;
	UEngineInputLayOutInfo& operator=(const UEngineInputLayOutInfo& _Other) = delete;
	UEngineInputLayOutInfo& operator=(UEngineInputLayOutInfo&& _Other) noexcept = delete;


	void AddInputLayout(
		LPCSTR _SemanticName,
		DXGI_FORMAT _Format,
		D3D11_INPUT_CLASSIFICATION _InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
		UINT _InstanceDataStepRate = 0,
		UINT _AlignedByteOffset = -1,
		UINT _InputSlot = 0,
		UINT _SemanticIndex = 0
	);

	UINT FormatSize(DXGI_FORMAT _Value);

	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayOutData;

protected:

private:
	int Offset = 0;
};

