#include "PreCompile.h"
#include "Title.h"
#include "ObjRenderer.h"
#include "CGlobal.h"
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/DefaultSceneComponent.h>
#include <EnginePlatform/EngineInput.h>

ATitle::ATitle()
{
	std::shared_ptr<UDefaultSceneComponent> Default = CreateDefaultSubObject<UDefaultSceneComponent>();
	RootComponent = Default;

	Default->SetWorldLocation({ -500.0f, 500.f, 0.0f });

	const float SCALE_RATIO = 3.f;

	RBackground = CreateDefaultSubObject<USpriteRenderer>();
	RBackground->SetSprite("Title_Screen.png", 0);
	RBackground->SetAutoScaleRatio(SCALE_RATIO);
	RBackground->SetupAttachment(RootComponent);

	std::string path = CGlobal::GetModelPath("Miscellaneous\\Title_Screen_Flag", "title_screen_flag");
	RFlag = CreateDefaultSubObject<ObjRenderer>();
	RFlag->Init(path);
	RFlag->SetScale3D({ 500.f, 500.f, 500.f });
	RFlag->SetRelativeLocation({ 500.f, -350.f, 0.f });
	RFlag->SetRotation({ -5.f, 165.f, 0.f });
	RFlag->SetupAttachment(RootComponent);
	
	RTitleText = CreateDefaultSubObject<USpriteRenderer>();
	RTitleText->SetSprite("Title_Screen.png", 4);
	RTitleText->SetAutoScaleRatio(SCALE_RATIO);
	RTitleText->SetRelativeLocation({ 125.f, -300.f, 1.f });
	RTitleText->SetupAttachment(RootComponent);

	RPushButton = CreateDefaultSubObject<USpriteRenderer>();
	RPushButton->SetSprite("Title_Screen.png", 2);
	RPushButton->SetAutoScaleRatio(SCALE_RATIO);
	RPushButton->SetRelativeLocation({ 250.f, -600.f, 1.f });
	RPushButton->SetupAttachment(RootComponent);

	RCopyright = CreateDefaultSubObject<USpriteRenderer>();
	RCopyright->SetSprite("Title_Screen.png", 3);
	RCopyright->SetAutoScaleRatio(SCALE_RATIO);
	RCopyright->SetRelativeLocation({ 300.f, -800.f, 1.f });
	RCopyright->SetupAttachment(RootComponent);
}

ATitle::~ATitle()
{

}

void ATitle::BeginPlay()
{
	AActor::BeginPlay();
}

void ATitle::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);

	static float elapsedSecs = 0.f;

	if (IsHidden == false)
	{
		elapsedSecs += _deltaTime;
		if (elapsedSecs >= 1.f)
		{
			elapsedSecs = 0.f;
			RPushButton->SetActiveSwitch();
		}
	}
}

void ATitle::Show()
{
	IsHidden = false;

	if (RBackground != nullptr)
	{
		RBackground->SetActive(true);
	}
	if (RFlag != nullptr)
	{
		RFlag->SetActive(true);
	}
	if (RTitleText != nullptr)
	{
		RTitleText->SetActive(true);
	}
	if (RPushButton != nullptr)
	{
		RPushButton->SetActive(true);
	}
	if (RCopyright != nullptr)
	{
		RCopyright->SetActive(true);
	}
}

void ATitle::Hide()
{
	IsHidden = true;

	if (RBackground != nullptr)
	{
		RBackground->SetActive(false);
	}
	if (RFlag != nullptr)
	{
		RFlag->SetActive(false);
	}
	if (RTitleText != nullptr)
	{
		RTitleText->SetActive(false);
	}
	if (RPushButton != nullptr)
	{
		RPushButton->SetActive(false);
	}
	if (RCopyright != nullptr)
	{
		RCopyright->SetActive(false);
	}
}
