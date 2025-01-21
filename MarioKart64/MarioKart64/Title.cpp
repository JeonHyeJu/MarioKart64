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

	const float SCALE_RATIO = 4.f;

	float w = CGlobal::FWINDOW_W;
	float h = CGlobal::FWINDOW_H;

	RBackground = CreateDefaultSubObject<USpriteRenderer>();
	RBackground->SetSprite("Title_Screen.png", 0);
	RBackground->SetAutoScale(false);
	RBackground->SetScale3D({ w, h, 1.f });
	//RBackground->SetRelativeLocation({ -w * .5f, h * .5f, 0.f });
	RBackground->SetupAttachment(RootComponent);

	std::string path = CGlobal::GetModelPath("Miscellaneous\\Title_Screen_Flag", "title_screen_flag");
	RFlag = CreateDefaultSubObject<ObjRenderer>();
	RFlag->SetOrder(0);
	RFlag->Init(path);
	RFlag->SetScale3D({ 400.f, 350.f, 300.f });
	RFlag->SetRelativeLocation({ 0.f, 150.f, -150.f });
	RFlag->SetRotation({ -15.f, 180.f, 0.f });
	RFlag->SetupAttachment(RootComponent);

	RTitleText = CreateDefaultSubObject<USpriteRenderer>();
	RTitleText->SetSprite("Title_Screen.png", 4);
	RTitleText->SetAutoScaleRatio(SCALE_RATIO);
	RTitleText->SetRelativeLocation({ 0.f, 100.f, -300.f });
	RTitleText->SetupAttachment(RootComponent);
	RTitleText->SetOrder(0);

	float halfH = h * .5f;

	RPushButton = CreateDefaultSubObject<USpriteRenderer>();
	RPushButton->SetSprite("Title_Screen.png", 2);
	RPushButton->SetAutoScaleRatio(SCALE_RATIO);
	RPushButton->SetRelativeLocation({ 0.f, -halfH * .65f, -310.f });
	RPushButton->SetupAttachment(RootComponent);

	RCopyright = CreateDefaultSubObject<USpriteRenderer>();
	RCopyright->SetSprite("Title_Screen.png", 3);
	RCopyright->SetAutoScaleRatio(SCALE_RATIO - 1);
	RCopyright->SetRelativeLocation({ 0.f, -halfH * .85f, -320.f });
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

	elapsedSecs += _deltaTime;
	if (elapsedSecs >= 1.f)
	{
		elapsedSecs = 0.f;
		RPushButton->SetActiveSwitch();
	}
}