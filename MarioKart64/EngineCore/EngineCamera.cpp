#include "PreCompile.h"
#include "EngineCamera.h"
#include "Renderer.h"

UEngineCamera::UEngineCamera()
{
}

UEngineCamera::~UEngineCamera()
{
}

void UEngineCamera::Tick(float _DetlaTime)
{
	Transform.View;
	Transform.Projection;
}

void UEngineCamera::Render(float _DetlaTime)
{
	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& RenderList = RenderGroup.second;

		for (std::shared_ptr<URenderer> Renderer : RenderList)
		{
			Renderer->Render(this, _DetlaTime);
		}
	}
}

void UEngineCamera::ChangeRenderGroup(int _PrevGroupOrder, std::shared_ptr<URenderer> _Renderer)
{
	Renderers[_PrevGroupOrder].remove(_Renderer);
	Renderers[_Renderer->GetOrder()].push_back(_Renderer);
}