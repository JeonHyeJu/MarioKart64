#pragma once
#include "BaseMap.h"

class AMarioRaceway : public ABaseMap
{
public:
	AMarioRaceway();
	~AMarioRaceway();

	AMarioRaceway(const AMarioRaceway& _other) = delete;
	AMarioRaceway(AMarioRaceway&& _other) noexcept = delete;
	AMarioRaceway& operator=(const AMarioRaceway& _other) = delete;
	AMarioRaceway& operator=(AMarioRaceway&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	void InitItem() override;

private:
	UINT Seed = 5;
};
