#pragma once
#include <EngineCore/Actor.h>
#include <EngineCore/SpriteRenderer.h>
#include <EnginePlatform/EngineWinImage.h>

class TestActor : public AActor
{
public:
	TestActor();
	~TestActor();

	TestActor(const TestActor& _Other) = delete;
	TestActor(TestActor&& _Other) noexcept = delete;
	TestActor& operator=(const TestActor& _Other) = delete;
	TestActor& operator=(TestActor&& _Other) noexcept = delete;

	/*std::shared_ptr<class USpriteRenderer> GetRenderer()
	{
		return Player;
	}*/

protected:
	void BeginPlay() override;
	void Tick(float _DeltaTime) override;

private:
	void InitTestSingleTriangle();
	void RunTestSingleTriangle(float _DeltaTime);
	void InitTestMultipleTriangle();
	void RunTestMultipleTriangle(float _DeltaTime);

	//std::shared_ptr<class USpriteRenderer> Player;
	std::shared_ptr<class ColorRenderer> Player;

	std::shared_ptr<class TriangleRenderer> Triangle;
	std::shared_ptr<class LineRenderer> Line;
};

