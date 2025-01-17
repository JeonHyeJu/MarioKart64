#pragma once
#include <EngineCore/Actor.h>

class ASelectButton;
class USpriteRenderer;
class AMapSpec : public AActor
{
public:
	AMapSpec();
	~AMapSpec();

	AMapSpec(const AMapSpec& _other) = delete;
	AMapSpec(AMapSpec&& _other) noexcept = delete;
	AMapSpec& operator=(const AMapSpec& _other) = delete;
	AMapSpec& operator=(AMapSpec&& _other) noexcept = delete;

	void Init(int _idx);

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	static const int SIZE = 4;
	const char* SPRITE_NAME = "TrackIcons.png";

	std::shared_ptr<USpriteRenderer> RMapImg = nullptr;
	ASelectButton* TrackList[4] = { nullptr, };
};
