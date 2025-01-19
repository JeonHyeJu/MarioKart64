#include "PreCompile.h"
#include "FontRenderer.h"
#include "EngineCamera.h"
#include "CameraActor.h"

UFontRenderer::UFontRenderer()
{
}

UFontRenderer::~UFontRenderer()
{
}

void UFontRenderer::SetFont(std::string_view _Value, float _Scale, UColor _Color, FW1_TEXT_FLAG _Flag)
{
	std::shared_ptr<UEngineFont> SFont = UEngineFont::Find<UEngineFont>(_Value);
	Font = SFont.get();
	Scale = _Scale;
	Color = _Color;
	Flag = _Flag;
}

void UFontRenderer::Render(UEngineCamera* _Camera, float _DeltaTime)
{
	URenderer::Render(_Camera, _DeltaTime);

	ACameraActor* CameraActor = _Camera->GetActor<ACameraActor>();

	FVector ScreenPos = CameraActor->WorldPosToScreenPos(GetWorldLocation());

	Font->FontDraw(Text.c_str(), Scale, ScreenPos, Color, Flag);
}
