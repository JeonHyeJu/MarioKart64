#include "PreCompile.h"
#include "Renderer.h"

URenderer::URenderer()
{
}

URenderer::~URenderer()
{
}

void URenderer::SetOrder(int _Order)
{
	int PrevOrder = GetOrder();
	UObject::SetOrder(_Order);

	ULevel* Level = GetActor()->GetWorld();

	std::shared_ptr<URenderer> RendererPtr = GetThis<URenderer>();

	Level->ChangeRenderGroup(PrevOrder, RendererPtr);
}

ENGINEAPI void URenderer::BeginPlay()
{
	SetOrder(0);
}

void URenderer::Render(float _DeltaTime)
{
}