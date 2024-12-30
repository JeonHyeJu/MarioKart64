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
	std::shared_ptr<class USpriteRenderer> Renderer;
};

