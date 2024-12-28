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
		SRV.GetAddressOf()
	))
	{
		MSGASSERT(UpperExt + "쉐이더 리소스 뷰 생성에 실패했습니다..");
		return;
	}

	Size.X = static_cast<float>(Metadata.width);
	Size.Y = static_cast<float>(Metadata.height);
}