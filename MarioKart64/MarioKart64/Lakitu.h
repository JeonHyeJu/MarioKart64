#pragma once
#include <EngineCore/Actor.h>

class ALakitu : public AActor
{
public:
	ALakitu();
	~ALakitu();

	ALakitu(const ALakitu& _Other) = delete;
	ALakitu(ALakitu&& _Other) noexcept = delete;
	ALakitu& operator=(const ALakitu& _Other) = delete;
	ALakitu& operator=(ALakitu&& _Other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	std::shared_ptr<class USpriteRenderer> Renderer = nullptr;
};

