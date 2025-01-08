#include "PreCompile.h"
#include "LineRenderer.h"
#include "CGlobal.h"

LineRenderer::LineRenderer()
{
	URenderUnit& unit = CreateRenderUnit();
	unit.SetMesh("Line");
	unit.SetMaterial(CGlobal::OBJ_LINE_SHADER_NAME);

	RenderColor.Albedo = float4(1.0f, 0.f, 0.f, 1.f);
	GetRenderUnit().ConstantBufferLinkData("FColor", RenderColor);
}

LineRenderer::~LineRenderer()
{

}

void LineRenderer::BeginPlay()
{
	URenderer::BeginPlay();
}

void LineRenderer::Render(UEngineCamera* _camera, float _deltaTime)
{
	URenderer::Render(_camera, _deltaTime);
}