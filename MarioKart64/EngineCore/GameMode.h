#pragma once
#include "Actor.h"

class AGameMode : public AActor
{
public:
	ENGINEAPI AGameMode();
	ENGINEAPI ~AGameMode();

	AGameMode(const AGameMode& _Other) = delete;
	AGameMode(AGameMode&& _Other) noexcept = delete;
	AGameMode& operator=(const AGameMode& _Other) = delete;
	AGameMode& operator=(AGameMode&& _Other) noexcept = delete;

protected:

private:

};

