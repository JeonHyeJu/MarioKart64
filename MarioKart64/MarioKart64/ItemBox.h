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

	static const float SIZE;
	static const float ROTATION_DEG;

	void SetRandomState(bool _isRotX, bool _isRotY, bool _isRotZ);

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	std::shared_ptr<class CubeRenderer> BoxRenderer = nullptr;
	std::shared_ptr<class UCollision> Collision = nullptr;
	FVector Rotation = FVector::ZERO;
};
