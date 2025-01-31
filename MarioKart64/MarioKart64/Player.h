#pragma once
#include "Driver.h"
#include "CData.h"
#include <fstream>	// temp
#include <sstream>	// temp

class APlayer : public ADriver
{
public:
	APlayer();
	~APlayer();

	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;
	void TickItem(float _deltaTime) override;

	void GetHandleRotation(float _deltaTime, float& _refRot) override;

private:
	std::shared_ptr<class USpriteRenderer> Renderer;
	std::shared_ptr<class UCollision> CollisionItem = nullptr;
};

