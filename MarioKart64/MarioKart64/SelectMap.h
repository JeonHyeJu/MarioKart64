#pragma once
#include <EngineCore/Actor.h>

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
	void Hover(uint8_t _idx);
	void Selecting(float _deltaTime);

	static const int SIZE = 4;
	std::shared_ptr<USpriteRenderer> RBg = nullptr;
	std::shared_ptr<USpriteRenderer> RTitle = nullptr;
	class ASelectButton* MainRects[4] = { nullptr, };
	std::shared_ptr<class AMapSpec> MapSpec = nullptr;

	uint8_t PreSelectedIdx = 0;
	uint8_t CurSelectedIdx = 0;
};
