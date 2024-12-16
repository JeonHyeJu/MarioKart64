#pragma once

#include <EngineBase/Object.h>

#include "EngineCore.h"

class ULevel : public UObject
{
public:
	ENGINEAPI ULevel();
	ENGINEAPI ~ULevel();

	ULevel(const ULevel& _Other) = delete;
	ULevel(ULevel&& _Other) noexcept = delete;
	ULevel& operator=(const ULevel& _Other) = delete;
	ULevel& operator=(ULevel&& _Other) noexcept = delete;

protected:

private:

};

