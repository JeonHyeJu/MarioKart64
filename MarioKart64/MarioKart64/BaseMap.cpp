#include "PreCompile.h"
#include "BaseMap.h"

BaseMap::BaseMap()
{

}

BaseMap::~BaseMap()
{

}

void BaseMap::BeginPlay()
{
	AActor::BeginPlay();
}

void BaseMap::Tick(float _deltaTime)
{
	AActor::Tick(_deltaTime);
}
