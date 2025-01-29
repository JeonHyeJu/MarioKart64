#include "PreCompile.h"
#include "CGlobal.h"
#include "LetterboxWidget.h"
#include <EngineCore/RenderUnit.h>

WLetterboxWidget::WLetterboxWidget()
{
	RenderUnit.TransformObject = this;
	RenderUnit.SetMesh("Rect");
	RenderUnit.SetMaterial(CGlobal::COLOR_WIDGET_MATERIAL);

	RenderUnit.ConstantBufferLinkData("LetterData", LetterData);
}

WLetterboxWidget::~WLetterboxWidget()
{

}

void WLetterboxWidget::Tick(float _deltaTime)
{
	UWidget::Tick(_deltaTime);
}

void WLetterboxWidget::Render(UEngineCamera* _camera, float _deltaTime)
{
	UWidget::Render(_camera, _deltaTime);
	CameraTransUpdate(_camera);
	RenderUnit.Render(_camera, _deltaTime);
}