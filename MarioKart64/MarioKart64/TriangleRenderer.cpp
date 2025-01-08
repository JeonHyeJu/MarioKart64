#include "PreCompile.h"
#include "CGlobal.h"
#include "TriangleRenderer.h"

TriangleRenderer::TriangleRenderer()
{
	URenderUnit& unit = CreateRenderUnit();
	//unit.SetMesh("Triangle");
	unit.SetMesh("MultiTriangle");
	unit.SetMaterial(CGlobal::OBJ_SKY_SHADER_NAME);

	RenderColor.Albedo = float4(0.f, 0.f, 1.f, 1.f);
	GetRenderUnit().ConstantBufferLinkData("FColor", RenderColor);
}

TriangleRenderer::~TriangleRenderer()
{

}

void TriangleRenderer::BeginPlay()
{
	URenderer::BeginPlay();
}

void TriangleRenderer::Render(UEngineCamera* _camera, float _deltaTime)
{
	URenderer::Render(_camera, _deltaTime);
}
