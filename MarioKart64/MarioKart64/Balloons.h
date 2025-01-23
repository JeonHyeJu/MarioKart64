#pragma once
#include <EngineCore/Actor.h>

class ABalloons : public AActor
{
public:
	ABalloons();
	~ABalloons();

	ABalloons(const ABalloons& _other) = delete;
	ABalloons(ABalloons&& _other) noexcept = delete;
	ABalloons& operator=(const ABalloons& _other) = delete;
	ABalloons& operator=(ABalloons&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	const int SIZE = 100;
	std::vector<class USpriteRenderer*> Renderers;
};
