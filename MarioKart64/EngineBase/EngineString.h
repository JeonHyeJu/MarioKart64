#pragma once
#include <string>

class UEngineString
{
public:
	~UEngineString();

	UEngineString(const UEngineString& _Other) = delete;
	UEngineString(UEngineString&& _Other) noexcept = delete;
	UEngineString& operator=(const UEngineString& _Other) = delete;
	UEngineString& operator=(UEngineString&& _Other) noexcept = delete;

	static std::string ToUpper(std::string_view);

	ENGINEAPI static std::wstring AnsiToUnicode(std::string_view _Name);

private:
	UEngineString();
};

