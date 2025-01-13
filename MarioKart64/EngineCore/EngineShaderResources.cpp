#include "PreCompile.h"
#include "EngineShaderResources.h"
#include "EngineBase/EngineDebug.h"

UEngineShaderResources::UEngineShaderResources()
{
}

UEngineShaderResources::~UEngineShaderResources()
{
}

void UEngineShaderResources::CreateSamplerRes(std::string_view _Name, UEngineSamplerRes _Res)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (true == SamplerRes.contains(UpperString))
	{
		MSGASSERT("같은 이름 상수버퍼가 한 쉐이더에 2개가 존재합니다");
		return;
	}

	SamplerRes[UpperString] = _Res;
}

void UEngineShaderResources::CreateTextureRes(std::string_view _Name, UEngineTextureRes _Res)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (true == TextureRes.contains(UpperString))
	{
		MSGASSERT("같은 이름 상수버퍼가 한 쉐이더에 2개가 존재합니다");
		return;
	}

	TextureRes[UpperString] = _Res;
}

void UEngineShaderResources::CreateConstantBufferRes(std::string_view _Name, UEngineConstantBufferRes _Res)
{
	std::string UpperString = UEngineString::ToUpper(_Name);

	if (true == ConstantBufferRes.contains(UpperString))
	{
		MSGASSERT("같은 이름 상수버퍼가 한 쉐이더에 2개가 존재합니다");
		return;
	}

	ConstantBufferRes[UpperString] = _Res;
}

void UEngineShaderResources::Setting()
{
	for (std::pair<const std::string, UEngineConstantBufferRes>& Res : ConstantBufferRes)
	{
		Res.second.Setting();
	}

	for (std::pair<const std::string, UEngineTextureRes>& Res : TextureRes)
	{
		Res.second.Setting();
	}

	for (std::pair<const std::string, UEngineSamplerRes>& Res : SamplerRes)
	{
		Res.second.Setting();
	}
}

bool UEngineShaderResources::IsSampler(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	return SamplerRes.contains(UpperName);
}

bool UEngineShaderResources::IsTexture(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	return TextureRes.contains(UpperName);
}
bool UEngineShaderResources::IsConstantBuffer(std::string_view _Name)
{
	std::string UpperName = UEngineString::ToUpper(_Name);
	return ConstantBufferRes.contains(UpperName);
}

void UEngineShaderResources::ConstantBufferLinkData(std::string_view _Name, void* _Data)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == ConstantBufferRes.contains(UpperName))
	{
		UEngineDebug::OutPutString("ConstantBufferLinkData has no " + UpperName);
		return;
	}

	ConstantBufferRes[UpperName].Data = _Data;
}

void UEngineShaderResources::SamplerSetting(std::string_view _Name, std::string_view _ResName)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == SamplerRes.contains(UpperName))
	{
		UEngineDebug::OutPutString("SamplerSetting has no " + UpperName);
		return;
	}

	SamplerRes[UpperName].Res = UEngineSampler::Find<UEngineSampler>(_ResName);

}

void UEngineShaderResources::TextureSetting(std::string_view _Name, std::string_view _ResName)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == TextureRes.contains(UpperName))
	{
		UEngineDebug::OutPutString("TextureSetting has no " + UpperName);
		return;
	}

	TextureRes[UpperName].Res = UEngineTexture::Find<UEngineTexture>(_ResName).get();
}

void UEngineShaderResources::TextureSetting(std::string_view _Name, std::shared_ptr<UEngineTexture> _Texture)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == TextureRes.contains(UpperName))
	{
		UEngineDebug::OutPutString("TextureRes.contains " + UpperName);
		return;
	}

	TextureRes[UpperName].Res = _Texture.get();
}

void UEngineShaderResources::TextureSetting(std::string_view _Name, UEngineTexture* _Texture)
{
	std::string UpperName = UEngineString::ToUpper(_Name);

	if (false == TextureRes.contains(UpperName))
	{
		UEngineDebug::OutPutString("TextureRes.contains " + UpperName);
		return;
	}

	TextureRes[UpperName].Res = _Texture;
}