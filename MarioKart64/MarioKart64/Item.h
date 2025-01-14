#pragma once
#include <EngineCore/Actor.h>

class AItem : public AActor
{
public:
	AItem();
	~AItem();

	AItem(const AItem& _other) = delete;
	AItem(AItem&& _other) noexcept = delete;
	AItem& operator=(const AItem& _other) = delete;
	AItem& operator=(AItem&& _other) noexcept = delete;

	void SetDirection(const FVector& _dir);
	float Size = 0.f;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;
	
private:
	std::shared_ptr<class USpriteRenderer> Renderer = nullptr;
	std::shared_ptr<class UCollision> Collision = nullptr;

	float Velocity = 500.f;
	float Acceleration = -5.f;
	FVector Direction;
};
