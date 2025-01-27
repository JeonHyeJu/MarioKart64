#pragma once
#include <EngineCore/Widget.h>

class WDefaultWidget : public UWidget
{
public:
	WDefaultWidget();
	~WDefaultWidget();

	WDefaultWidget(const WDefaultWidget& _other) = delete;
	WDefaultWidget(WDefaultWidget&& _other) noexcept = delete;
	WDefaultWidget& operator=(const WDefaultWidget& _other) = delete;
	WDefaultWidget& operator=(WDefaultWidget&& _other) noexcept = delete;

	void SetParentWidget(WDefaultWidget* _parent);

	template<typename WidgetType>
	std::shared_ptr<WidgetType> CreateWidget(int _ZOrder, class AHUD* _hud, std::string_view _Name = "NONE")
	{
		static_assert(std::is_base_of_v<WDefaultWidget, WidgetType>, "위젯을 상속받지 않은 클래스를 CreateWidget하려고 했습니다.");

		if (false == std::is_base_of_v<WDefaultWidget, WidgetType>)
		{
			MSGASSERT("위젯을 상속받지 않은 클래스를 CreateWidget하려고 했습니다.");
			return nullptr;
		}

		char* Memory = new char[sizeof(WidgetType)];

		WDefaultWidget* WidgetPtr = reinterpret_cast<WidgetType*>(Memory);
		WidgetPtr->HUD = _hud;

		WidgetType* NewPtr = reinterpret_cast<WidgetType*>(Memory);
		std::shared_ptr<WidgetType> NewWidgetPtr(NewPtr = new(Memory) WidgetType());

		NewWidgetPtr->SetName(_Name);
		NewWidgetPtr->SetOrder(_ZOrder);
		NewWidgetPtr->SetParentWidget(this);

		Childs[NewWidgetPtr->GetOrder()].push_back(NewWidgetPtr);
		return NewWidgetPtr;
	}

protected:
	void Tick(float _deltaTime) override;
	void Render(UEngineCamera* _camera, float _deltaTime) override;

	URenderUnit RenderUnit;
	WDefaultWidget* Parent = nullptr;

private:
	std::map<int, std::list<std::shared_ptr<class WDefaultWidget>>> Childs;
};
