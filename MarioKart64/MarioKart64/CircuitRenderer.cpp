#include "PreCompile.h"
#include "CircuitRenderer.h"
#include "CGlobal.h"
#include "GameData.h"
#include <EngineCore/EngineGraphicDevice.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineCore.h>

CircuitRenderer::CircuitRenderer()
{
}

CircuitRenderer::~CircuitRenderer()
{
}

void CircuitRenderer::BeginPlay()
{
	URenderer::BeginPlay();
	//USceneComponent::BeginPlay();
}

void CircuitRenderer::Render(UEngineCamera* _camera, float _deltaTime)
{
	URenderer::Render(_camera, _deltaTime);
}


void CircuitRenderer::Init(ECircuit _map)
{
	//Sort();

	if (!SRenderInfo::MapInfos.contains(_map))
	{
		MSGASSERT("맵이 초기화되지 않았습니다.");
	}

	const std::vector<SRenderInfo>& _vec = SRenderInfo::MapInfos.find(_map)->second.RenderInfos;
	int size = static_cast<int>(_vec.size());
	for (int i = 0; i < size; ++i)
	{
		const SRenderInfo& unitInfo = _vec[i];
		URenderUnit& unit = CreateRenderUnit();

		SetMesh(unitInfo.Name, i);
		SetMaterial(unitInfo.MatName, i);
		unit.SetTexture("diffTexture", unitInfo.TexName);
		unit.ConstantBufferLinkData("FDebug", Debug);
	}
}

void CircuitRenderer::Sort(std::vector<SRenderInfo>& _refVec)
{
	std::sort(_refVec.begin(), _refVec.end(), [](const SRenderInfo& _left, const SRenderInfo& _right)
	{
		return _left.Z < _right.Z;
	});
}

