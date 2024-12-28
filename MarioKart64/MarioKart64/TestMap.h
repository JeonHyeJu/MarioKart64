#pragma once
#include <EngineCore/Actor.h>

class ATestMap : public AActor
{
public:
	ATestMap();
	~ATestMap();

	ATestMap(const ATestMap& _other) = delete;
	ATestMap(ATestMap&& _other) noexcept = delete;
	ATestMap& operator=(const ATestMap& _other) = delete;
	ATestMap& operator=(ATestMap&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	std::shared_ptr<class ObjRenderer> Renderer = nullptr;
	std::shared_ptr<class USpriteRenderer> LogoRenderer;
};
