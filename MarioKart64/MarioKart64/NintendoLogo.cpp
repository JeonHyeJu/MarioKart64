#include "PreCompile.h"
#include "NintendoLogo.h"
#include "ObjRenderer.h"
#include "CGlobal.h"

ANintendoLogo::ANintendoLogo()
{
	std::string path = CGlobal::GetModelPath("Miscellaneous\\Nintendo_Logo", "nintendo");

	Renderer = CreateDefaultSubObject<ObjRenderer>();
	RootComponent = Renderer;

	Renderer->Init(path);
	Renderer->SetScale3D({ .2f, .2f, .2f });
	Renderer->SetWorldLocation({ 0.0f, -150.0f, 0.0f });
}

ANintendoLogo::~ANintendoLogo()
{

}

void ANintendoLogo::BeginPlay()
{
	AActor::BeginPlay();
}

void ANintendoLogo::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
}

void ANintendoLogo::Show()
{
	if (Renderer != nullptr)
	{
		Renderer->SetActive(true);
	}
}

void ANintendoLogo::Hide()
{
	if (Renderer != nullptr)
	{
		Renderer->SetActive(false);
	}
}
