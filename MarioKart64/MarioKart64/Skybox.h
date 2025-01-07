#pragma once
#include <EngineCore/Actor.h>

class USpriteRenderer;
class ColorRenderer;
class ASkybox : public AActor
{
public:
	ASkybox();
	~ASkybox();

	ASkybox(const ASkybox& _other) = delete;
	ASkybox(ASkybox&& _other) noexcept = delete;
	ASkybox& operator=(const ASkybox& _other) = delete;
	ASkybox& operator=(ASkybox&& _other) noexcept = delete;

	void MakeClouds();

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	std::shared_ptr<ColorRenderer> Front = nullptr;
	std::shared_ptr<ColorRenderer> Left = nullptr;
	std::shared_ptr<ColorRenderer> Right = nullptr;
	std::shared_ptr<ColorRenderer> Back = nullptr;
};
