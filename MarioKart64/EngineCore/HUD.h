#pragma once
#include <EngineCore/Actor.h>

class AHUD : public AActor
{
public:
	ENGINEAPI AHUD();
	ENGINEAPI ~AHUD();

	AHUD(const AHUD& _Other) = delete;
	AHUD(AHUD&& _Other) noexcept = delete;
	AHUD& operator=(const AHUD& _Other) = delete;
	AHUD& operator=(AHUD&& _Other) noexcept = delete;

	void CreateWidget();

protected:

private:

};

