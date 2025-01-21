#pragma once
#include <iostream>
#include <io.h>

#include "EnginePath.h"

const int MAXPATH = 256;

class UEngineFile : public UEnginePath
{
public:
	ENGINEAPI UEngineFile();

	ENGINEAPI UEngineFile(const std::string& _Path);
	ENGINEAPI UEngineFile(std::string_view _Path);
	ENGINEAPI UEngineFile(std::filesystem::path _Path);
	ENGINEAPI UEngineFile(const UEnginePath& _Path);
	ENGINEAPI ~UEngineFile();

	ENGINEAPI void Write(class UEngineSerializer& _Ser);
	ENGINEAPI void Read(class UEngineSerializer& _Ser);

	ENGINEAPI void Write(const void* _Ptr, size_t _Size);
	ENGINEAPI void Read(void* _Ptr, size_t _Size);

	ENGINEAPI void FileOpen(const char* _Mode);
	void Close();

	int GetFileSize();

	ENGINEAPI std::string GetAllFileText();

private:
	FILE* File = nullptr;
};

