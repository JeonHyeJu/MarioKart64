#pragma once
#include "Widget.h"
#include <EngineCore/EngineSprite.h>
#include <EngineBase/EngineDelegate.h>

class UImageWidget : public UWidget
{
public:
	class FrameAnimation
	{
	public:
		UEngineSprite* Sprite = nullptr;
		std::vector<int> FrameIndex;
		std::vector<float> FrameTime;
		std::map<int, EngineDelegate> Events;

		int CurIndex = 0;
		int ResultIndex = 0;
		float CurTime = 0.0f;
		bool Loop = true;
		bool IsEnd = false;

		void Reset()
		{
			CurIndex = 0;
			CurTime = 0;
			ResultIndex = 0;
			IsEnd = false;
		}
	};

	ENGINEAPI void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, int _Start, int _End, float Time = 0.1f, bool _Loop = true);

	ENGINEAPI void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, std::vector<float> _Frame, bool _Loop = true);

	ENGINEAPI void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, float _Frame, bool _Loop = true);

	ENGINEAPI void ChangeAnimation(std::string_view _AnimationName, bool _Force = false);

	ENGINEAPI void SetAnimationEvent(std::string_view _AnimationName, int _Frame, std::function<void()> _Function);

	ENGINEAPI FrameAnimation* FindAnimation(std::string_view _AnimationName);

private:
	int CurIndex = 0;
	float CurAnimationSpeed = 1.0f;
	FrameAnimation* CurAnimation = nullptr;
	std::map<std::string, FrameAnimation> FrameAnimations;

public:
	ENGINEAPI UImageWidget();
	ENGINEAPI ~UImageWidget();

	UImageWidget(const UImageWidget& _Other) = delete;
	UImageWidget(UImageWidget&& _Other) noexcept = delete;
	UImageWidget& operator=(const UImageWidget& _Other) = delete;
	UImageWidget& operator=(UImageWidget&& _Other) noexcept = delete;

	void Tick(float _DeltaTime) override;
	void Render(UEngineCamera* Camera, float _DeltaTime) override;

	URenderUnit& GetRenderUnit()
	{
		return RenderUnit;
	}

	ENGINEAPI void SetSprite(std::string_view _Name, UINT _Index = 0);
	ENGINEAPI void SetTexture(std::string_view _Name, bool AutoScale = false, float _Ratio = 1.0f);
	ENGINEAPI FVector GetRealScaleOfSprite() const;

	void SetAutoScale(bool _Value)
	{
		IsAutoScale = _Value;
	}

	void SetAutoScaleRatio(float _Scale)
	{
		AutoScaleRatio = _Scale;
	}

	FResultColor ColorData;
	FUVValue UVValue;
	FSpriteData SpriteData;

protected:

private:
	bool IsAutoScale = true;
	float AutoScaleRatio = 1.0f;
	class UEngineSprite* Sprite = nullptr;

	URenderUnit RenderUnit;

};

