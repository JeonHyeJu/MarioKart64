#include "PreCompile.h"
#include "ColorRenderer.h"
#include "CGlobal.h"

ColorRenderer::ColorRenderer()
{
	URenderUnit& unit = CreateRenderUnit();
	unit.SetMesh("Rect");
	unit.SetMaterial(CGlobal::OBJ_SKY_SHADER_NAME);
	//unit.SetTexture("diffTexture", "Cloud_1.png");

	RenderColor.Albedo = { 0.f, 0.f, 0.f, 1.f };
	GetRenderUnit().ConstantBufferLinkData("FColor", RenderColor);
}

ColorRenderer::~ColorRenderer()
{
}

void ColorRenderer::BeginPlay()
{
	URenderer::BeginPlay();
}

void ColorRenderer::Render(UEngineCamera* _camera, float _deltaTime)
{
	URenderer::Render(_camera, _deltaTime);
}