#include "PreCompile.h"
#include "EngineStructuredBuffer.h"

std::map<int, std::map<std::string, std::shared_ptr<UEngineStructuredBuffer>>> UEngineStructuredBuffer::BufferMap;

void UEngineStructuredBuffer::Release()
{
	BufferMap.clear();
}

UEngineStructuredBuffer::UEngineStructuredBuffer()
{
}

UEngineStructuredBuffer::~UEngineStructuredBuffer()
{
}

std::shared_ptr<UEngineStructuredBuffer> UEngineStructuredBuffer::CreateOrFind(UINT _Byte, const std::string_view& _Name)
{
	if (0 == _Byte)
	{
		MSGASSERT("0바이트 상수버퍼가 만들어지려고 했습니다.");
	}

	std::string UpperName = UEngineString::ToUpper(_Name);

	if (true == BufferMap.contains(_Byte))
	{
		if (true == BufferMap[_Byte].contains(UpperName))
		{
			return BufferMap[_Byte][UpperName];
		}
	}

	std::shared_ptr<UEngineStructuredBuffer> NewRes = std::make_shared<UEngineStructuredBuffer>();
	NewRes->SetName(UpperName);
	NewRes->ResCreate(_Byte, 1);
	BufferMap[_Byte][UpperName] = NewRes;

	return NewRes;
}

void UEngineStructuredBuffer::ResCreate(UINT _DataSize, UINT _DataCount)
{
	Buffer = nullptr;

	{
		DataCount = _DataCount;
		DataSize = _DataSize;
		BufferInfo.ByteWidth = _DataCount * _DataSize;
		// 텍스처로 보기 대문에
		BufferInfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		BufferInfo.Usage = D3D11_USAGE_DYNAMIC;
		BufferInfo.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED; // 스트럭처드 버처
		BufferInfo.StructureByteStride = _DataSize; // 1개의 데이터 크기가 얼마인지 알려줘야 한다.

		if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, nullptr, &Buffer))
		{
			MSGASSERT("스트럭처드 버퍼 생성에 실패했습니다..");
			return;
		}
		D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
		tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
		tSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
		tSRVDesc.BufferEx.FirstElement = 0;
		tSRVDesc.BufferEx.Flags = 0;
		tSRVDesc.BufferEx.NumElements = DataCount; 

		if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateShaderResourceView(Buffer.Get(), &tSRVDesc, &SRV))
		{
			MSGASSERT("스트럭처드 버퍼 생성에 실패했습니다..");
			return;
		}
	}
}

void UEngineStructuredBuffer::ChangeData(void* _Data, UINT _Size)
{
	if (_Size != BufferInfo.ByteWidth)
	{
		MSGASSERT("바이트 크기가 다르게 세팅되었습니다" + GetName());
		return;
	}

	D3D11_MAPPED_SUBRESOURCE Data = {};
	UEngineCore::GetDevice().GetContext()->Map(Buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);

	if (nullptr == Data.pData)
	{
		MSGASSERT("그래픽카드가 수정을 거부했습니다.");
	}
	memcpy_s(Data.pData, BufferInfo.ByteWidth, _Data, BufferInfo.ByteWidth);
	UEngineCore::GetDevice().GetContext()->Unmap(Buffer.Get(), 0);
}

void UEngineStructuredBuffer::Setting(EShaderType _Type, UINT _BindIndex)
{
	ID3D11Buffer* ArrPtr[1] = { Buffer.Get() };

	switch (_Type)
	{
	case EShaderType::VS:
		UEngineCore::GetDevice().GetContext()->VSSetShaderResources(_BindIndex, 1, &SRV);
		break;
	case EShaderType::PS:
		UEngineCore::GetDevice().GetContext()->PSSetShaderResources(_BindIndex, 1, &SRV);
		break;
	case EShaderType::HS:
	case EShaderType::DS:
	case EShaderType::GS:
	case EShaderType::CS:
	default:
		MSGASSERT("아직 존재하지 않는 쉐이더에 세팅하려고 했습니다.");
		break;
	}
}
