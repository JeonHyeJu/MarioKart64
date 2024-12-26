#pragma once
#include "SceneComponent.h"

class UDefaultSceneComponent : public USceneComponent
{
public:
	ENGINEAPI UDefaultSceneComponent();
	ENGINEAPI ~UDefaultSceneComponent();
	
	UDefaultSceneComponent(const UDefaultSceneComponent& _Other) = delete;
	UDefaultSceneComponent(UDefaultSceneComponent&& _Other) noexcept = delete;
	UDefaultSceneComponent& operator=(const UDefaultSceneComponent& _Other) = delete;
	UDefaultSceneComponent& operator=(UDefaultSceneComponent&& _Other) noexcept = delete;

protected:

private:

};

