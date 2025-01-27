#pragma once
#include "TransformObject.h"
#include "EngineDataStruct.h"
#include "RenderUnit.h"

class UWidget : public UObject, public UTransformObject
{
	friend AHUD;

public:
	ENGINEAPI UWidget();
	ENGINEAPI ~UWidget();

	UWidget(const UWidget& _Other) = delete;
	UWidget(UWidget&& _Other) noexcept = delete;
	UWidget& operator=(const UWidget& _Other) = delete;
	UWidget& operator=(UWidget&& _Other) noexcept = delete;

	ENGINEAPI virtual void Tick(float _DeltaTime);
	ENGINEAPI virtual void Render(class UEngineCamera* Camera, float _DeltaTime);

	class ULevel* GetWorld();

	void SetHoverEvent(std::function<void()> _Value)
	{
		Hover = _Value;
	}

	void SetDownEvent(std::function<void()> _Value)
	{
		Down = _Value;
	}

	void SetUpEvent(std::function<void()> _Value)
	{
		Up = _Value;
	}

protected:
	class AHUD* HUD;

private:
	std::function<void()> Hover;
	std::function<void()> Down;
	std::function<void()> Up;
};

