#include "PreCompile.h"
#include "EngineString.h"
#include <EngineBase/EngineDebug.h>

UEngineString::UEngineString()
{
}

UEngineString::~UEngineString()
{
}

std::string UEngineString::ToUpper(std::string_view _string)
{
	std::string Result = _string.data();
	Result.resize(_string.size());

	for (size_t i = 0; i < _string.size(); i++)
	{
		Result[i] = std::toupper(_string[i]);
	}
	
	return Result;
}

std::wstring UEngineString::AnsiToUnicode(std::string_view _Name)
{
	int Size = MultiByteToWideChar(CP_ACP, 0, _Name.data(), static_cast<int>(_Name.size()), nullptr, 0);

	if (0 == Size)
	{
		MSGASSERT("MultiByteToWideChar 문자열 변환에 실패했습니다" + std::string(_Name));
		return L"";
	}

	std::wstring Result;
	Result.resize(Size);

	Size = MultiByteToWideChar(CP_ACP, 0, _Name.data(), static_cast<int>(_Name.size()), &Result[0], Size);

	if (0 == Size)
	{
		MSGASSERT("MultiByteToWideChar 문자열 변환에 실패했습니다" + std::string(_Name));
		return L"";
	}

	return Result;
}


