#include "PreCompile.h"
#include "EngineResources.h"
#include "EngineTexture.h"

std::shared_ptr<UEngineResources> UEngineResources::Find(std::string_view _ResName, std::string_view _Name)
{
	if (false == ResMap[_ResName.data()].contains(_Name.data()))
	{
		return nullptr;
	}

	return ResMap[_ResName.data()][_Name.data()];
}

void UEngineResources::PushRes(std::shared_ptr<UEngineResources> _Res, const std::string_view _Info, std::string_view _Name, std::string_view _Path)
{
	if (true == ResMap[_Info.data()].contains(_Name.data()))
	{
		MSGASSERT("이미 로드한 리소스를 또 로드 하려고 했습니다" + std::string(_Info.data()) + "  " + _Name.data());
		return;
	}

	std::string UpperName = UEngineString::ToUpper(_Name);

	_Res->SetName(UpperName);
	_Res->Path = _Path;
	ResMap[_Info.data()].insert({UpperName, _Res });

	return;
}