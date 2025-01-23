#include "PreCompile.h"
#include "LightActor.h"
#include "Light.h"
#include "EngineCamera.h"

ALightActor::ALightActor()
{
	LightComponent = CreateDefaultSubObject<ULight>();
	RootComponent = LightComponent;
}

ALightActor::~ALightActor()
{
}




