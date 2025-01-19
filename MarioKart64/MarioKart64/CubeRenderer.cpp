#include "PreCompile.h"
#include "CubeRenderer.h"
#include "CGlobal.h"

CubeRenderer::CubeRenderer()
{
	URenderUnit& unit = CreateRenderUnit();
	unit.SetMesh("Cube");
	unit.SetMaterial(CGlobal::OBJ_SKY_SHADER_NAME);
	//unit.SetTexture("diffTexture", "Cloud_1.png");

	SetColor({ -10.f, -10.f, -10.f, 1.f });
	GetRenderUnit().ConstantBufferLinkData("FColor", RenderColor);
}

CubeRenderer::~CubeRenderer()
{

}

void CubeRenderer::BeginPlay()
{
	URenderer::BeginPlay();
}

void CubeRenderer::Render(class UEngineCamera* _camera, float _deltaTime)
{
	URenderer::Render(_camera, _deltaTime);
}
