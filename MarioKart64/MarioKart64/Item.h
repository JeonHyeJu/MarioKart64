#pragma once
#include <EngineCore/Actor.h>
#include <string>
#include "CData.h"

class AItem : public AActor
{
public:
	AItem();
	~AItem();

	AItem(const AItem& _other) = delete;
	AItem(AItem&& _other) noexcept = delete;
	AItem& operator=(const AItem& _other) = delete;
	AItem& operator=(AItem&& _other) noexcept = delete;

	void SetInitVelocity(float _velocity);
	void SetDirection(const FVector& _dir);
	void Init(const EItemType& _itemType);

	const float Size = 27.f;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;
	
private:
	void RunningShell(float _deltaTime);
	void RunningFakeItem(float _deltaTime);

	std::shared_ptr<class USpriteRenderer> _CreateSpriteRenderer();
	std::shared_ptr<class CubeRenderer> _CreateCubeRenderer();
	//std::shared_ptr<class ObjRenderer> _CreateObjRenderer();

	std::shared_ptr<class URenderer> Renderer = nullptr;
	std::shared_ptr<class UCollision> Collision = nullptr;

	float Velocity = 1000.f;
	float Acceleration = -5.f;
	FVector Direction;
	EItemType ItemType = EItemType::SIZE;
};
