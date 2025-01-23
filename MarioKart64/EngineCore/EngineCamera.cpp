#include "PreCompile.h"
#include "EngineCamera.h"
#include "Renderer.h"
#include "EngineRenderTarget.h"

UEngineCamera::UEngineCamera()
{
}

void UEngineCamera::BeginPlay()
{
	USceneComponent::BeginPlay();

	FVector Scale = UEngineCore::GetScreenScale();

	ProjectionScale = Scale;

	ViewPortInfo.Width = Scale.X;
	ViewPortInfo.Height = Scale.Y;

	ViewPortInfo.TopLeftX = 0.0f;
	ViewPortInfo.TopLeftY = 0.0f;
	ViewPortInfo.MinDepth = 0.0f;
	ViewPortInfo.MaxDepth = 1.0f;

	CameraTarget = std::make_shared<UEngineRenderTarget>();
	CameraTarget->CreateTarget(UEngineCore::GetScreenScale());
	CameraTarget->CreateDepth();
}

UEngineCamera::~UEngineCamera()
{
	Renderers.clear();
}

void UEngineCamera::Tick(float _DetlaTime)
{
	USceneComponent::ComponentTick(_DetlaTime);
}

void UEngineCamera::Render(float _DetlaTime)
{
	UEngineCore::GetDevice().GetContext()->RSSetViewports(1, &ViewPortInfo);
	CameraTarget->Clear();
	CameraTarget->Setting();

	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& RenderList = RenderGroup.second;

		if (true == RendererZSort[RenderGroup.first])
		{
			RenderList.sort([](std::shared_ptr<URenderer>& _Left, std::shared_ptr<URenderer>& _Right)
			{
				return _Left->GetTransformRef().WorldLocation.Z > _Right->GetTransformRef().WorldLocation.Z;
			});
		}


		for (std::shared_ptr<URenderer> Renderer : RenderList)
		{
			if (false == Renderer->IsActive())
			{
				continue;
			}

			Renderer->Render(this, _DetlaTime);
		}
	}
}

void UEngineCamera::Release(float _DeltaTime)
{
	for (std::pair<const int, std::list<std::shared_ptr<URenderer>>>& RenderGroup : Renderers)
	{
		std::list<std::shared_ptr<URenderer>>& RenderList = RenderGroup.second;
		std::list<std::shared_ptr<URenderer>>::iterator StartIter = RenderList.begin();
		std::list<std::shared_ptr<URenderer>>::iterator EndIter = RenderList.end();

		for (; StartIter != EndIter; )
		{
			if (false == (*StartIter)->IsDestroy())
			{
				++StartIter;
				continue;
			}

			StartIter = RenderList.erase(StartIter);
		}
	}
}

void UEngineCamera::SetZSort(int _Order, bool _Value)
{
	RendererZSort[_Order] = _Value;
}

void UEngineCamera::ChangeRenderGroup(int _PrevGroupOrder, std::shared_ptr<URenderer> _Renderer)
{
	Renderers[_PrevGroupOrder].remove(_Renderer);
	Renderers[_Renderer->GetOrder()].push_back(_Renderer);
}

void UEngineCamera::CalculateViewAndProjection()
{
	FTransform& Trans = GetTransformRef();

	Trans.View.View(Trans.World.ArrVector[3], Trans.World.GetFoward(), Trans.World.GetUp());

	switch (ProjectionType)
	{
	case EProjectionType::Perspective:
		Trans.Projection.PerspectiveFovDeg(FOV, ProjectionScale.X, ProjectionScale.Y, Near, Far);
		break;
	case EProjectionType::Orthographic:
		Trans.Projection.OrthographicLH(ProjectionScale.X, ProjectionScale.Y, Near, Far);
		break;
	default:
		break;
	}
}