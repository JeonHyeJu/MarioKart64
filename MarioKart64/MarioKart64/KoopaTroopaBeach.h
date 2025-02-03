#pragma once
#include "BaseMap.h"

class AKoopaTroopaBeach : public ABaseMap
{
public:
	AKoopaTroopaBeach();
	~AKoopaTroopaBeach();

	AKoopaTroopaBeach(const AKoopaTroopaBeach& _other) = delete;
	AKoopaTroopaBeach(AKoopaTroopaBeach&& _other) noexcept = delete;
	AKoopaTroopaBeach& operator=(const AKoopaTroopaBeach& _other) = delete;
	AKoopaTroopaBeach& operator=(AKoopaTroopaBeach&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	void InitItem() override;

private:
	UINT Seed = 3;
};
