#include "PreCompile.h"
#include "EngineSprite.h"
#include "EngineBase/EngineDebug.h"

UEngineSprite::UEngineSprite()
{
}

UEngineSprite::~UEngineSprite()
{
}

std::shared_ptr<UEngineSprite> UEngineSprite::CreateSpriteToMeta(std::string_view _Name, std::string_view _DataFileExt)
{
	std::shared_ptr<UEngineTexture> Tex = UEngineTexture::Find<UEngineTexture>(_Name);

	if (nullptr == Tex)
	{
		MSGASSERT("존재하지 않는 텍스처로 스프라이트를 만들수는 없습니다.");
		return nullptr;
	}

	std::shared_ptr<UEngineSprite> NewRes = std::make_shared<UEngineSprite>();
	PushRes<UEngineSprite>(NewRes, _Name, "");
	NewRes->Texture = Tex.get();

	UEnginePath Path = Tex->GetPath();
	std::string FileName = Path.GetFileName();
	FileName += _DataFileExt;
	Path.MoveParent();
	Path.Append(FileName);

	UEngineFile File = Path;

	File.FileOpen("rt");

	std::string Text = File.GetAllFileText();

	std::vector<std::string> SpriteDataTexts;

	size_t StartPosition = 0;
	while (true)
	{
		size_t RectIndex = Text.find("rect:", StartPosition);
		size_t AligIndex = Text.find("outline:", RectIndex);

		if (RectIndex == std::string::npos || AligIndex == std::string::npos)
		{
			break;
		}

		SpriteDataTexts.push_back(Text.substr(RectIndex, AligIndex - RectIndex));
		StartPosition = AligIndex;
	}

	FVector TexSize = Tex->GetTextureSize();

	std::vector<FSpriteData> TestData;

	for (size_t i = 0; i < SpriteDataTexts.size(); i++)
	{
		std::string Text = SpriteDataTexts[i];

		size_t Start = 0;

		FSpriteData SpriteData;


		{
			std::string Number = UEngineString::InterString(Text, "x:", "\n", Start);
			SpriteData.CuttingPos.X = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "y:", "\n", Start);
			SpriteData.CuttingPos.Y = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "width:", "\n", Start);
			SpriteData.CuttingSize.X = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "height:", "\n", Start);
			SpriteData.CuttingSize.Y = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "x:", ",", Start);
			SpriteData.Pivot.X = static_cast<float>(atof(Number.c_str()));
		}

		{
			std::string Number = UEngineString::InterString(Text, "y:", "}", Start);
			SpriteData.Pivot.Y = static_cast<float>(atof(Number.c_str()));
		}



		SpriteData.CuttingPos.Y = TexSize.Y - SpriteData.CuttingPos.Y - SpriteData.CuttingSize.Y;

		SpriteData.CuttingPos.X /= TexSize.X;
		SpriteData.CuttingPos.Y /= TexSize.Y;
		SpriteData.CuttingSize.X /= TexSize.X;
		SpriteData.CuttingSize.Y /= TexSize.Y;


		TestData.push_back(SpriteData);
	}


	NewRes->SpriteDatas = TestData;

	return NewRes;

}


ID3D11ShaderResourceView* UEngineSprite::GetSRV()
{
	return Texture->GetSRV();
}

FVector UEngineSprite::GetSpriteScaleToReal(size_t _Index)
{
	if (SpriteDatas.size() <= _Index)
	{
		MSGASSERT("스프라이트의 인덱스를 초과하여 사용하려고 했습니다.");
	}

	FVector Result;

	Result.X = SpriteDatas[_Index].CuttingSize.X * Texture->GetTextureSize().X;
	Result.Y = SpriteDatas[_Index].CuttingSize.Y * Texture->GetTextureSize().Y;

	return Result;
}