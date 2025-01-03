#pragma once
#include "EngineEnums.h"
#include <EngineBase/EngineFile.h>
#include "EngineShaderResources.h"

class UEngineShader
{
	friend class UEngineVertexShader;

public:
	UEngineShader();
	~UEngineShader();

	UEngineShader(const UEngineShader& _Other) = delete;
	UEngineShader(UEngineShader&& _Other) noexcept = delete;
	UEngineShader& operator=(const UEngineShader& _Other) = delete;
	UEngineShader& operator=(UEngineShader&& _Other) noexcept = delete;

	ENGINEAPI static void ReflectionCompile(UEngineFile& _File);

	UEngineShaderResources ShaderResources;

	Microsoft::WRL::ComPtr<ID3DBlob> GetShaderCodeBlob()
	{
		return ShaderCodeBlob;
	};

protected:
	EShaderType ShaderType = EShaderType::MAX;
	UINT VersionHigh = 5;
	UINT VersionLow = 0;
	Microsoft::WRL::ComPtr<ID3DBlob> ShaderCodeBlob = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> ErrorCodeBlob = nullptr;
	std::string EntryName;

	void ShaderResCheck();

private:
};

