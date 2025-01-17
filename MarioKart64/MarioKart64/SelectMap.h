#pragma once
#include <EngineCore/Actor.h>

class ASelectButton;
class USpriteRenderer;
class ASelectMap : public AActor
{
public:
	ASelectMap();
	~ASelectMap();

	ASelectMap(const ASelectMap& _other) = delete;
	ASelectMap(ASelectMap&& _other) noexcept = delete;
	ASelectMap& operator=(const ASelectMap& _other) = delete;
	ASelectMap& operator=(ASelectMap&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	static const int SIZE = 4;
	std::shared_ptr<USpriteRenderer> RBg = nullptr;
	std::shared_ptr<USpriteRenderer> RTitle = nullptr;
	ASelectButton* MainRects[4] = { nullptr, };
};
