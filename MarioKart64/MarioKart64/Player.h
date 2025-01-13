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

	void SetMap(class ATestMap* _ptr);

protected:
	void Tick(float _deltaTime) override;

private:
	void Move(float _deltaTime);
	bool CheckCollision(const FVector& _loc, int& _refIdx, float& _refDist);
	void CheckCollisionOfAllMap();
	float GetSlope();
	void GetForwardPhysics(float _deltaTime, float& _refDx, bool _isCollided=true);
	void GetHandleRotation(float _deltaTime, float& _refRot);
	void CheckLab();

	std::shared_ptr<class USpriteRenderer> Renderer;
	std::shared_ptr<USpriteRenderer> RendererDebug;

	float Speed = 5.f;
	float SpeedWeight = 0.f;

	const float FRICTIONAL_FORCE = .05f;
	const float MAX_SPEED = 20.f;
	const float MAX_TURN = 5.f;

	const float WEIGHT = 50.f;
	const float ACCELERATION = WEIGHT * 7.f;	// 350px/s
	const float FRICTION_FORCE = WEIGHT * 2.f;	// 100px/s
	const float MAX_VELOCITY = 1000.f * 2;
	const float GRAVITY_FORCE = -300.f;

	float Velocity = 0.f;
	float VelocityV = 0.f;

	int PrevIdx = -1;
	int PrevGroupIdx = -1;
	ATestMap* TestMapPtr = nullptr;

	bool IsTouchLastTriangle = false;
	int Lab = 0;
};

