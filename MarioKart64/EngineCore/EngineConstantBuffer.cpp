#include "PreCompile.h"
#include "EngineConstantBuffer.h"

std::map<int, std::map<std::string, std::shared_ptr<UEngineConstantBuffer>>> UEngineConstantBuffer::BufferMap;

void UEngineConstantBuffer::Release()
{
	BufferMap.clear();
}

UEngineConstantBuffer::UEngineConstantBuffer()
{
}

UEngineConstantBuffer::~UEngineConstantBuffer()
{
}

std::shared_ptr<UEngineConstantBuffer> UEngineConstantBuffer::CreateOrFind(UINT _Byte, const std::string_view& _Name)
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

	std::shared_ptr<UEngineConstantBuffer> NewRes = std::make_shared<UEngineConstantBuffer>();
	NewRes->SetName(UpperName);
	NewRes->ResCreate(_Byte);
	BufferMap[_Byte][UpperName] = NewRes;

	return NewRes;
}

void UEngineConstantBuffer::ResCreate(UINT _Byte)
{
	{
		BufferInfo.ByteWidth = _Byte;
		BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		BufferInfo.Usage = D3D11_USAGE_DYNAMIC;

		if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, nullptr, &Buffer))
		{
			MSGASSERT("상수버퍼 생성에 실패했습니다..");
			return;
		}
	}
}

void UEngineConstantBuffer::ChangeData(void* _Data, UINT _Size)
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

void UEngineConstantBuffer::Setting(EShaderType _Type, UINT _BindIndex)
{
	ID3D11Buffer* ArrPtr[1] = { Buffer.Get() };

	switch (_Type)
	{
	case EShaderType::VS:
		UEngineCore::GetDevice().GetContext()->VSSetConstantBuffers(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::PS:
		UEngineCore::GetDevice().GetContext()->PSSetConstantBuffers(_BindIndex, 1, ArrPtr);
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
