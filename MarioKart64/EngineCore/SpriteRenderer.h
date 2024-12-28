#pragma once
#include "Renderer.h"
#include <EngineBase/EngineDelegate.h>

class USpriteRenderer : public URenderer
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
		bool IsAutoScale = true;
		float AutoScaleRatio = 1.0f;

		void Reset()
		{
			CurIndex = 0;
			CurTime = 0;
			ResultIndex = 0;
			IsEnd = false;
		}
	};

	// constrcuter destructer
	ENGINEAPI USpriteRenderer();
	ENGINEAPI ~USpriteRenderer();

	USpriteRenderer(const USpriteRenderer& _Other) = delete;
	USpriteRenderer(USpriteRenderer&& _Other) noexcept = delete;
	USpriteRenderer& operator=(const USpriteRenderer& _Other) = delete;
	USpriteRenderer& operator=(USpriteRenderer&& _Other) noexcept = delete;

	int GetCurIndex()
	{
		return CurIndex;
	}

	int GetFrameIndex()
	{
		return CurAnimation->CurIndex;
	}

	ENGINEAPI void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, int _Start, int _End, float Time = 0.1f, bool _Loop = true);

	ENGINEAPI void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, std::vector<float> _Frame, bool _Loop = true);

	ENGINEAPI void CreateAnimation(std::string_view _AnimationName, std::string_view _SpriteName, std::vector<int> _Indexs, float _Frame, bool _Loop = true);

	ENGINEAPI void ChangeAnimation(std::string_view _AnimationName, bool _Force = false);

	ENGINEAPI void SetAnimationEvent(std::string_view _AnimationName, int _Frame, std::function<void()> _Function);

	ENGINEAPI FrameAnimation* FindAnimation(std::string_view _AnimationName);

	ENGINEAPI std::string GetCurSpriteName()
	{
		return Sprite->GetName();
	}

	ENGINEAPI void SetSprite(std::string_view _Name, size_t _CurIndex = 0);

	bool IsCurAnimationEnd()
	{
		return CurAnimation->IsEnd;
	}

	void SetAnimationSpeed(float _Speed)
	{
		CurAnimationSpeed = _Speed;
	}

	void ResetAnimationSpeed()
	{
		CurAnimationSpeed = 1.0f;
	}

protected:
	ENGINEAPI void Render(class UEngineCamera* _Camera, float _DeltaTime) override;
	void BeginPlay() override;
	void ComponentTick(float _DeltaTime) override;

private:
	int CurIndex = 0;
	float CurAnimationSpeed = 1.0f;

	std::map<std::string, FrameAnimation> FrameAnimations;
	FrameAnimation* CurAnimation = nullptr; 
};

