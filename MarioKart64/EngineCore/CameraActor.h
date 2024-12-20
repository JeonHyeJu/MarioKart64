#pragma once
#include "Actor.h"

class ACameraActor : public AActor
{
public:
	ACameraActor();
	~ACameraActor();

	ACameraActor(const ACameraActor& _Other) = delete;
	ACameraActor(ACameraActor&& _Other) noexcept = delete;
	ACameraActor& operator=(const ACameraActor& _Other) = delete;
	ACameraActor& operator=(ACameraActor&& _Other) noexcept = delete;

	std::shared_ptr<class UEngineCamera> CameraComponent = nullptr;

	void BeginPlay() override;
	void Tick(float _DeltaTime);


protected:

private:
};

