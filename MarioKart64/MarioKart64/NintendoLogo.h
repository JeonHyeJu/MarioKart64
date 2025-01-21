#pragma once
#include <EngineCore/Actor.h>

class ANintendoLogo : public AActor
{
public:
	ANintendoLogo();
	~ANintendoLogo();

	ANintendoLogo(const ANintendoLogo& _other) = delete;
	ANintendoLogo(ANintendoLogo&& _other) noexcept = delete;
	ANintendoLogo& operator=(const ANintendoLogo& _other) = delete;
	ANintendoLogo& operator=(ANintendoLogo&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	std::shared_ptr<class ObjRenderer> Renderer = nullptr;
};
