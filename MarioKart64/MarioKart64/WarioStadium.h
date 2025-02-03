#pragma once
#include "BaseMap.h"

class AWarioStadium : public ABaseMap
{
public:
	AWarioStadium();
	~AWarioStadium();

	AWarioStadium(const AWarioStadium& _other) = delete;
	AWarioStadium(AWarioStadium&& _other) noexcept = delete;
	AWarioStadium& operator=(const AWarioStadium& _other) = delete;
	AWarioStadium& operator=(AWarioStadium&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	void InitItem();

private:
	UINT Seed = 9;
};
