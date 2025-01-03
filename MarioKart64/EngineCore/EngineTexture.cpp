#include "PreCompile.h"
#include "EngineTexture.h"

#pragma comment(lib, "DirectXTex.lib")

UEngineTexture::UEngineTexture()
{
}

UEngineTexture::~UEngineTexture()
{
}

std::shared_ptr<UEngineTexture> UEngineTexture::Load(std::string_view _Name, std::string_view _Path)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 텍스처를 도 로드하려고 했습니다." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineTexture> NewRes =  std::make_shared<UEngineTexture>();
	PushRes<UEngineTexture>(NewRes, _Name, _Path);
	NewRes->ResLoad();

	return NewRes;
}

void UEngineTexture::ResLoad()
{
	UEngineFile File = Path;

	std::string Str = File.GetPathToString();
	std::string Ext = File.GetExtension();
	std::wstring wLoadPath = UEngineString::AnsiToUnicode(Str.c_str());
	std::string UpperExt = UEngineString::ToUpper(Ext.c_str());


	if (UpperExt == ".DDS")
	{
		if (S_OK != DirectX::LoadFromDDSFile(wLoadPath.c_str(), DirectX::DDS_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("DDS 파일 로드에 실패했습니다.");
			return;
		}
	}
	else if (UpperExt == ".TGA")
	{
		if (S_OK != DirectX::LoadFromTGAFile(wLoadPath.c_str(), DirectX::TGA_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT("TGA 파일 로드에 실패했습니다.");
			return;
		}
	}
	else
	{
		if (S_OK != DirectX::LoadFromWICFile(wLoadPath.c_str(), DirectX::WIC_FLAGS_NONE, &Metadata, ImageData))
		{
			MSGASSERT(UpperExt + "파일 로드에 실패했습니다.");
			return;
		}
	}

	if (S_OK != DirectX::CreateShaderResourceView(
		UEngineCore::GetDevice().GetDevice(),
		ImageData.GetImages(),
		ImageData.GetImageCount(),
		ImageData.GetMetadata(),
		&SRV
	))
	{
		MSGASSERT(UpperExt + "쉐이더 리소스 뷰 생성에 실패했습니다..");
		return;
	}

	Size.X = static_cast<float>(Metadata.width);
	Size.Y = static_cast<float>(Metadata.height);
}

void UEngineTexture::Setting(EShaderType _Type, UINT _BindIndex)
{
	ID3D11ShaderResourceView* ArrPtr[1] = { SRV.Get() };

	switch (_Type)
	{
	case EShaderType::VS:
		UEngineCore::GetDevice().GetContext()->VSSetShaderResources(_BindIndex, 1, ArrPtr);
		break;
	case EShaderType::PS:
		UEngineCore::GetDevice().GetContext()->PSSetShaderResources(_BindIndex, 1, ArrPtr);
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

void UEngineTexture::ResCreate(const D3D11_TEXTURE2D_DESC& _Value)
{
	Desc = _Value;

	UEngineCore::GetDevice().GetDevice()->CreateTexture2D(&Desc, nullptr, &Texture2D);

	if (nullptr == Texture2D)
	{
		MSGASSERT("텍스처 생성에 실패했습니다.");
		return;
	}

	if (Desc.BindFlags & D3D11_BIND_DEPTH_STENCIL)
	{
		if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateDepthStencilView(Texture2D.Get(), nullptr, &DSV))
		{
			MSGASSERT("깊이버퍼 생성에 실패했습니다..");
			return;
		}
	}
}