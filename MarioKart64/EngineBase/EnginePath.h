#pragma once
#include <filesystem>

class UEnginePath
{
public:
	ENGINEAPI UEnginePath();
	ENGINEAPI UEnginePath(std::string_view _Path);
	ENGINEAPI UEnginePath(std::filesystem::path _Path);
	ENGINEAPI virtual ~UEnginePath();

	// delete Function
	//UEnginePath(const UEnginePath& _Other) = delete;
	//UEnginePath(UEnginePath&& _Other) noexcept = delete;
	//UEnginePath& operator=(const UEnginePath& _Other) = delete;
	//UEnginePath& operator=(UEnginePath&& _Other) noexcept = delete;

	bool IsExists();
	void MoveParent();

	ENGINEAPI std::string GetPathToString();

	std::string GetFileName();

	std::string GetDirectoryName();

	std::string GetExtension();


	ENGINEAPI bool MoveParentToDirectory(std::string_view _Path);

	ENGINEAPI bool Move(std::string_view _Path);

	bool IsDirectory();

	bool IsFile();

	void Append(std::string_view _AppendName);


protected:
	std::filesystem::path Path;

private:
};

