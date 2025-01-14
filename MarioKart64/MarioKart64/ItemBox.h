#pragma once
#include <EngineCore/Actor.h>

class AItemBox : public AActor
{
public:
	AItemBox();
	~AItemBox();

	AItemBox(const AItemBox& _other) = delete;
	AItemBox(AItemBox&& _other) noexcept = delete;
	AItemBox& operator=(const AItemBox& _other) = delete;
	AItemBox& operator=(AItemBox&& _other) noexcept = delete;

	const float SIZE = 50.f;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	const float ROTATION_DEG = 30.f;

	std::shared_ptr<class CubeRenderer> BoxRenderer = nullptr;

	std::shared_ptr<class UCollision> Collision = nullptr;
};
