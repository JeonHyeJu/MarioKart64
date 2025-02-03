#pragma once
#include "BaseMap.h"

class ABowserCastle : public ABaseMap
{
public:
	ABowserCastle();
	~ABowserCastle();

	ABowserCastle(const ABowserCastle& _other) = delete;
	ABowserCastle(ABowserCastle&& _other) noexcept = delete;
	ABowserCastle& operator=(const ABowserCastle& _other) = delete;
	ABowserCastle& operator=(ABowserCastle&& _other) noexcept = delete;

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

	void InitItem() override;

private:
	UINT Seed = 2;
};
