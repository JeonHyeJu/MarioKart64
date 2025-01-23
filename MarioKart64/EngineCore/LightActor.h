#pragma once
#include "Actor.h"
#include "EngineEnums.h"
#include "Light.h"

class ALightActor : public AActor
{
public:
	ENGINEAPI ALightActor();
	ENGINEAPI ~ALightActor();

	ALightActor(const ALightActor& _Other) = delete;
	ALightActor(ALightActor&& _Other) noexcept = delete;
	ALightActor& operator=(const ALightActor& _Other) = delete;
	ALightActor& operator=(ALightActor&& _Other) noexcept = delete;

protected:

private:
	std::shared_ptr<class ULight> LightComponent = nullptr;
};

