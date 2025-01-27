#include "PreCompile.h"
#include "DefaultWidget.h"

WDefaultWidget::WDefaultWidget()
{
	RenderUnit.TransformObject = this;
}

WDefaultWidget::~WDefaultWidget()
{
}

void WDefaultWidget::Tick(float _deltaTime)
{
	UWidget::Tick(_deltaTime);

	for (std::pair<const int, std::list<std::shared_ptr<WDefaultWidget>>>& WidgetPair : Childs)
	{
		std::list<std::shared_ptr<WDefaultWidget>>& WidgetList = WidgetPair.second;

		for (std::shared_ptr<WDefaultWidget> Widget : WidgetList)
		{
			if (false == Widget->IsActive())
			{
				continue;
			}

			Widget->Tick(_deltaTime);
		}
	}
}

void WDefaultWidget::Render(UEngineCamera* _camera, float _deltaTime)
{
	UWidget::Render(_camera, _deltaTime);

	for (std::pair<const int, std::list<std::shared_ptr<WDefaultWidget>>> WidgetPair : Childs)
	{
		std::list<std::shared_ptr<WDefaultWidget>>& WidgetList = WidgetPair.second;

		for (std::shared_ptr<WDefaultWidget> Widget : WidgetList)
		{
			if (false == Widget->IsActive())
			{
				continue;
			}

			Widget->Render(_camera, _deltaTime);
		}
	}
}

void WDefaultWidget::SetParentWidget(WDefaultWidget* _parent)
{
	if (_parent == nullptr) return;
	if (_parent == Parent) return;

	Parent = _parent;
	RenderUnit.TransformObject->SetupAttachment(_parent->RenderUnit.TransformObject);
}