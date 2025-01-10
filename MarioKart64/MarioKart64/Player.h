#pragma once
#include <EngineCore/Pawn.h>

class APlayer : public APawn
{
public:
	APlayer();
	~APlayer();

	APlayer(const APlayer& _Other) = delete;
	APlayer(APlayer&& _Other) noexcept = delete;
	APlayer& operator=(const APlayer& _Other) = delete;
	APlayer& operator=(APlayer&& _Other) noexcept = delete;

protected:
	void Tick(float _deltaTime) override;

private:
	void Move(float _deltaTime);
	void MoveBackup(float _deltaTime);

	std::shared_ptr<class USpriteRenderer> Renderer;

	float Speed = 5.f;
	float SpeedWeight = 0.f;

	const float FRICTIONAL_FORCE = .05f;
	const float MAX_SPEED = 20.f;
	const float MAX_TURN = 5.f;

	const float WEIGHT = 50.f;
	const float ACCELERATION = WEIGHT * 7.f;	// 350px/s
	const float FRICTION_FORCE = WEIGHT * 2.f;	// 100px/s
	const float MAX_VELOCITY = 1000.f;
	float Velocity = 0.f;
};

