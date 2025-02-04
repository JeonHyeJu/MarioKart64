#include "PreCompile.h"
#include "EngineFile.h"
#include "EngineDebug.h"
#include "EngineSerializer.h"

UEngineFile::UEngineFile()
{


}

UEngineFile::UEngineFile(const UEnginePath& _Path)
	: UEnginePath(_Path)
{

}

UEngineFile::UEngineFile(const std::string& _Path)
	: UEnginePath(std::string_view(_Path.c_str()))
{

}

UEngineFile::UEngineFile(std::string_view _Path)
	: UEnginePath(_Path)
{

}

UEngineFile::UEngineFile(std::filesystem::path _Path)
	: UEnginePath(_Path)
{

}

UEngineFile::~UEngineFile()
{
	Close();
}

bool UEngineFile::FileOpen(const char* _Mode)
{
	fopen_s(&File, GetPathToString().c_str(), _Mode);

	if (nullptr == File)
	{
		return false;
		//MSGASSERT(GetPathToString() + + "파일 오픈에 실패했습니다");
	}

	return true;
}


void UEngineFile::Write(UEngineSerializer& _Ser)
{
	Write(_Ser.GetDataPtr(), _Ser.GetWriteOffset());
}

int UEngineFile::GetFileSize()
{
	if (false == IsFile())
	{
		MSGASSERT(Path.string() + "파일이 아닌 존재의 크기를 알수는 없습니다.");
		return -1;
	}

	return static_cast<int>(std::filesystem::file_size(Path));
}

void UEngineFile::Read(class UEngineSerializer& _Ser)
{
	int FileSize = GetFileSize();

	_Ser.DataResize(FileSize);

	Read(_Ser.GetDataPtr(), FileSize);
}

void UEngineFile::Write(const void* _Ptr, size_t _Size)
{
	if (0 == _Size)
	{
		MSGASSERT("크기가 0인 데이터를 쓸수는 없습니다.");
	}

	if (nullptr == _Ptr)
	{
		MSGASSERT("존재하지 않는 메모리를 사용하려고 했습니다.");
	}

	if (nullptr == File)
	{
		MSGASSERT("열지 않은 파일에 내용을 쓰려고 했습니다");
		return;
	}

	fwrite(_Ptr, _Size, 1, File);
}

void UEngineFile::Read(void* _Ptr, size_t _Size)
{
	if (0 == _Size)
	{
		MSGASSERT("크기가 0인 데이터를 읽을수는 없습니다.");
	}

	if (nullptr == _Ptr)
	{
		MSGASSERT("존재하지 않는 메모리를 읽으려고 했습니다.");
	}

	if (nullptr == File)
	{
		MSGASSERT("열지 않은 파일에 내용을 쓰려고 했습니다");
		return;
	}

	fread(_Ptr, _Size, 1, File);
}

void UEngineFile::Close()
{
	if (nullptr != File)
	{
		fclose(File);
		File = nullptr;
	}
}


std::string UEngineFile::GetAllFileText()
{
	UEngineSerializer Ser;
	Read(Ser);

	return reinterpret_cast<const char*>(Ser.GetDataPtr());
}