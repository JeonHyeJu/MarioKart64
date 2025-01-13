#pragma once
#include "EngineConstantBuffer.h"
#include <EngineBase/Object.h>
#include "EngineTexture.h"
#include "EngineSampler.h"

class UEngineShaderRes 
{
public:
	std::string Name;
	EShaderType ShaderType = EShaderType::MAX;
	UINT BindIndex = 0;
};

class UEngineConstantBufferRes : public UEngineShaderRes
{
public:
	void* Data = nullptr;
	UINT BufferSize;
	std::shared_ptr<UEngineConstantBuffer> Res;

	void Setting()
	{
		if (nullptr != Data)
		{
			Name;
			Res->ChangeData(Data, BufferSize);
		}

		Res->Setting(ShaderType, BindIndex);
	}
};

class UEngineTextureRes : public UEngineShaderRes
{
public:
	UEngineTexture* Res;

	void Setting()
	{
		Res->Setting(ShaderType, BindIndex);
	}
};

class UEngineSamplerRes : public UEngineShaderRes
{
public:
	std::shared_ptr<UEngineSampler> Res;

	void Setting()
	{
		Res->Setting(ShaderType, BindIndex);
	}

};

class UEngineShaderResources
{
public:
	UEngineShaderResources();
	~UEngineShaderResources();

	void CreateSamplerRes(std::string_view _Name, UEngineSamplerRes _Res);

	void CreateTextureRes(std::string_view _Name, UEngineTextureRes _Res);

	void CreateConstantBufferRes(std::string_view _Name, UEngineConstantBufferRes Res);

	template<typename DataType>
	void ConstantBufferLinkData(std::string_view _Name, DataType& Data)
	{
		ConstantBufferLinkData(_Name, reinterpret_cast<void*>(&Data));
	}

	void ConstantBufferLinkData(std::string_view _Name, void* Data);

	void SamplerSetting(std::string_view _Name, std::string_view _ResName);
	void TextureSetting(std::string_view _Name, std::string_view _ResName);
	void TextureSetting(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture);

	void TextureSetting(std::string_view _Name, UEngineTexture* _Texture);

	bool IsSampler(std::string_view _Name);
	bool IsTexture(std::string_view _Name);
	bool IsConstantBuffer(std::string_view _Name);
	void Setting();

protected:

private:
	std::map<std::string, UEngineConstantBufferRes> ConstantBufferRes;
	std::map<std::string, UEngineTextureRes> TextureRes;
	std::map<std::string, UEngineSamplerRes> SamplerRes;
};

