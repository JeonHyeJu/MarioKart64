#pragma once
#include "TransformObject.h"

class UWidget : public UTransformObject
{
public:
	UWidget();
	~UWidget();

	UWidget(const UWidget& _Other) = delete;
	UWidget(UWidget&& _Other) noexcept = delete;
	UWidget& operator=(const UWidget& _Other) = delete;
	UWidget& operator=(UWidget&& _Other) noexcept = delete;
};

