#pragma once
#include "EngineTexture.h"
#include <EngineBase/EngineMath.h>
#include <EngineBase/EngineDirectory.h>
#include "EngineResources.h"
#include "EngineDataStruct.h"

class UEngineSprite : public UEngineResources
{
public:
	UEngineSprite();
	~UEngineSprite();

	UEngineSprite(const UEngineSprite& _Other) = delete;
	UEngineSprite(UEngineSprite&& _Other) noexcept = delete;
	UEngineSprite& operator=(const UEngineSprite& _Other) = delete;
	UEngineSprite& operator=(UEngineSprite&& _Other) noexcept = delete;


	ENGINEAPI static std::shared_ptr<UEngineSprite> CreateSpriteToFolder(std::string_view _Path)
	{
		UEngineDirectory Dir = _Path;

		return CreateSpriteToFolder(Dir.GetDirectoryName(), _Path);
	}

	ENGINEAPI static std::shared_ptr<UEngineSprite> CreateSpriteToFolder(std::string_view _Name, std::string_view _Path);

	ENGINEAPI static std::shared_ptr<UEngineSprite> CreateSpriteToMeta(std::string_view _Name, std::string_view _DataFileExt);

	ENGINEAPI UEngineTexture* GetTexture(size_t _Index = 0);

	ENGINEAPI ID3D11ShaderResourceView* GetSRV(size_t _Index = 0);

	ENGINEAPI FSpriteData GetSpriteData(size_t _Index)
	{
		if (SpriteDatas.size() <= _Index)
		{
			MSGASSERT("스프라이트의 인덱스를 초과하여 사용하려고 했습니다.");
		}

		return SpriteDatas[_Index];
	}

	ENGINEAPI FVector GetSpriteScaleToReal(size_t _Index);

	ENGINEAPI size_t GetSpriteCount()
	{
		return SpriteTexture.size();
	}

protected:

private:
	std::vector<UEngineTexture*> SpriteTexture;
	std::vector<FSpriteData> SpriteDatas;

};

