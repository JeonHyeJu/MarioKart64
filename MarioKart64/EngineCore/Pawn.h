#pragma once
#include <EngineCore/Actor.h>

class APawn : public AActor
{
public:
	ENGINEAPI APawn();
	ENGINEAPI ~APawn();

	APawn(const APawn& _Other) = delete;
	APawn(APawn&& _Other) noexcept = delete;
	APawn& operator=(const APawn& _Other) = delete;
	APawn& operator=(APawn&& _Other) noexcept = delete;

protected:

private:

};

