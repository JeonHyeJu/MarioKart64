#include "PreCompile.h"
#include "Skybox.h"
#include "CGlobal.h"
#include <EngineCore/DefaultSceneComponent.h>
#include <EngineCore/SpriteRenderer.h>
#include <EngineCore/Renderer.h>
#include "ColorRenderer.h"

ASkybox::ASkybox()
{
	RootComponent = CreateDefaultSubObject<UDefaultSceneComponent>();

	const float SIZE = 100000.f;
	std::shared_ptr<URenderer> Renderer = CreateDefaultSubObject<URenderer>();
	Renderer->SetupAttachment(RootComponent);
	Renderer->CreateRenderUnit();
	Renderer->SetScale3D({ SIZE, SIZE, SIZE });

	URenderUnit& unit = Renderer->GetRenderUnit();
	unit.SetMesh("Sphere");
	unit.SetMaterial(CGlobal::OBJ_SPRITE_SHADER_NAME);
	unit.SetTexture("diffTexture", "bg_0.png");
}

ASkybox::~ASkybox()
{
}

void ASkybox::BeginPlay()
{
	AActor::BeginPlay();
}

void ASkybox::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
}