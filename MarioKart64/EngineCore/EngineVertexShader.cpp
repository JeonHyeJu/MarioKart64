#include "PreCompile.h"
#include "EngineVertexShader.h"

UEngineVertexShader::UEngineVertexShader()
{
	ShaderType = EShaderType::VS;
}

UEngineVertexShader::~UEngineVertexShader()
{
}

std::shared_ptr<UEngineVertexShader> UEngineVertexShader::Load(std::string_view _Name, std::string_view _Path, const std::string_view& _EntryPoint, UINT _VersionHigh /*= 5*/, UINT _VersionLow /*= 0*/)
{
	std::string UpperName = ToUpperName(_Name);

	if (true == Contains(UpperName))
	{
		MSGASSERT("이미 로드한 텍스처를 도 로드하려고 했습니다." + UpperName);
		return nullptr;
	}

	std::shared_ptr<UEngineVertexShader> NewRes = std::make_shared<UEngineVertexShader>();
	PushRes<UEngineVertexShader>(NewRes, _Name, _Path);
	NewRes->VersionHigh = _VersionHigh;
	NewRes->VersionLow = _VersionLow;
	NewRes->EntryName = _EntryPoint;
	NewRes->ResLoad();

	return NewRes;
}

void UEngineVertexShader::ResLoad()
{
	std::wstring WPath = UEngineString::AnsiToUnicode(GetPath().GetPathToString());

	std::string version = "vs_" + std::to_string(VersionHigh) + "_" + std::to_string(VersionLow);

	int Flag0 = 0;
	int Flag1 = 0;

#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif

	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	D3DCompileFromFile(
		WPath.c_str(),
		nullptr,
		nullptr,
		EntryName.c_str(),
		version.c_str(),
		Flag0,
		Flag1,
		&ShaderCodeBlob,
		&ErrorCodeBlob
	);

	if (nullptr == ShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(ErrorCodeBlob->GetBufferPointer());
		MSGASSERT("쉐이더 코드 중간빌드에서 실패했습니다\n" + ErrString);
		return;
	}

	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateVertexShader(
		ShaderCodeBlob->GetBufferPointer(),
		ShaderCodeBlob->GetBufferSize(),
		nullptr,
		&ShaderRes
	);

	if (S_OK != Result)
	{
		MSGASSERT("버텍스 쉐이더 생성에 실패했습니다.");
	}

	UEngineShader::ShaderResCheck();
}

void UEngineVertexShader::Setting()
{
	UEngineCore::GetDevice().GetContext()->VSSetShader(ShaderRes.Get(), nullptr, 0);
}