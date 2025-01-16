#include "PreCompile.h"
#include "NintendoLogo.h"
#include "ObjRenderer.h"
#include "CGlobal.h"
#include "CircuitRenderer.h"	// for test

#include <EngineCore/DefaultSceneComponent.h>

ANintendoLogo::ANintendoLogo()
{
	std::string path = CGlobal::GetModelPath("Miscellaneous\\Nintendo_Logo", "nintendo");

	Renderer = CreateDefaultSubObject<ObjRenderer>();
	RootComponent = Renderer;

	Renderer->SetOrder(0);
	Renderer->SetScale3D({ .25f, .25f, .25f });
	Renderer->Init(path);
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
