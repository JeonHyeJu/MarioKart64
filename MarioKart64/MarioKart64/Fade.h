#pragma once
#include <EngineCore/Actor.h>

class AFade : public AActor
{
public:
	AFade();
	~AFade();

	AFade(const AFade& _other) = delete;
	AFade(AFade&& _other) noexcept = delete;
	AFade& operator=(const AFade& _other) = delete;
	AFade& operator=(AFade&& _other) noexcept = delete;

	void Show();
	void Hide();
	void Update(float _deltaTime);

protected:
	void BeginPlay() override;
	void Tick(float _deltaTime) override;

private:
	std::shared_ptr<class ColorRenderer> RRect;

	float CurVal = 0.f;
	float ChangeVal = 1.f;
	bool IsRun = false;
};
