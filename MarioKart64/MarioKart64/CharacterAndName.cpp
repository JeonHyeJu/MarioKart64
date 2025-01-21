#include "PreCompile.h"
#include "CharacterAndName.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>

ACharacterAndName::ACharacterAndName()
{
	std::shared_ptr<UDefaultSceneComponent> _default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = _default;

	RImage = CreateDefaultSubObject<USpriteRenderer>();
	RName = CreateDefaultSubObject<USpriteRenderer>();

	RImage->SetupAttachment(RootComponent);
	RName->SetupAttachment(RootComponent);
}

ACharacterAndName::~ACharacterAndName()
{

}

void ACharacterAndName::BeginPlay()
{
	AActor::BeginPlay();
}

void ACharacterAndName::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
}

void ACharacterAndName::Init(std::string_view _spriteName, int _idx, float _scaleRatio)
{
	const float ANIM_SEC = .1f;
	const int STRIDE = 20;

	std::vector<int> _selIdxs = { 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };
	std::vector<int> _idlIdxs = { 0, 1, 2, 3, 4, 5, 4, 3, 2, 1, 0 };
	std::vector<float> _idlSecs(_idlIdxs.size(), ANIM_SEC);
	_idlSecs[_idlSecs.size() - 1] = 1.f;

	std::vector<int> idleIdxs = _idlIdxs;
	std::vector<int> selectIdxs = _selIdxs;
	int sadIdx = 16 + (STRIDE * _idx);

	for (int& val : idleIdxs)
	{
		val += STRIDE * _idx;
	}
	for (int& val : selectIdxs)
	{
		val += STRIDE * _idx;
	}

	RImage->CreateAnimation("Idle", _spriteName, idleIdxs[0], idleIdxs[0], 1.f);
	RImage->CreateAnimation("Blink", _spriteName, idleIdxs, _idlSecs, false);
	RImage->CreateAnimation("Select", _spriteName, selectIdxs, ANIM_SEC, false);
	RImage->CreateAnimation("Sad", _spriteName, sadIdx, sadIdx, ANIM_SEC);
	RImage->ChangeAnimation("Idle");
	RImage->SetAutoScaleRatio(_scaleRatio);

	FVector scale = RImage->GetRealScaleOfSprite();
	float width = scale.X;
	float height = scale.Y;

	const int INIT_NAME_IDX = 17;
	RName = CreateDefaultSubObject<USpriteRenderer>();
	RName->SetSprite(_spriteName, INIT_NAME_IDX + (_idx * STRIDE));
	RName->SetAutoScaleRatio(_scaleRatio);
	RName->SetupAttachment(RootComponent);
	RName->SetRelativeLocation(FVector{ 0.f, -height * .5f, -1.f });
}

FVector ACharacterAndName::GetScale()
{
	if (RImage == nullptr)
	{
		return FVector::ZERO;
	}

	return RImage->GetRealScaleOfSprite();
}

FVector ACharacterAndName::GetScaleName()
{
	if (RName == nullptr)
	{
		return FVector::ZERO;
	}

	return RName->GetRealScaleOfSprite();
}

void ACharacterAndName::SetBlink()
{
	if (RImage != nullptr)
	{
		RImage->ChangeAnimation("Blink", true);
	}
}

void ACharacterAndName::SetSelect()
{
	if (RImage != nullptr)
	{
		RImage->ChangeAnimation("Select");
	}
}

void ACharacterAndName::SetIdle()
{
	if (RImage != nullptr)
	{
		RImage->ChangeAnimation("Idle");
	}
}