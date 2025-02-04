#include "PreCompile.h"
#include "InstanceRectRenderer.h"
#include <EngineCore/EngineCamera.h>

// TODO

InstanceRectRenderer::InstanceRectRenderer()
{
	{
		URenderUnit& unit = CreateRenderUnit();
		SetMesh("Rect");
		SetMaterial("TileMap");
	}

	{
		URenderUnit& unit = CreateRenderUnit();
		unit.SetMesh("Rect");
		unit.SetMaterial("TileMapInst");
	}
}

InstanceRectRenderer::~InstanceRectRenderer()
{

}

void InstanceRectRenderer::BeginPlay()
{
	URenderer::BeginPlay();
}

void InstanceRectRenderer::Render(UEngineCamera* _camera, float _deltaTime)
{
	switch (RenderType)
	{
	case Normal:
		URenderer::Render(_camera, _deltaTime);
		break;
	case Instancing:
		RenderInstancing(_camera, _deltaTime);
		break;
	default:
		break;
	}
}

void InstanceRectRenderer::RenderInstancing(class UEngineCamera* _camera, float _deltaTime)
{
	/*FTransform& CameraTrans = _camera->GetTransformRef();
	FTransform& RendererTrans = GetTransformRef();

	RendererTrans.View = CameraTrans.View;
	RendererTrans.Projection = CameraTrans.Projection;
	RendererTrans.WVP = RendererTrans.World * RendererTrans.View * RendererTrans.Projection;

	URenderUnit& unit = GetRenderUnit(1);

	FTransform Trans;
	FMatrix Scale;
	FMatrix Pos;

	Scale.Scale(ImageSize);

	InstTransform.resize(Tiles.size());
	InstSpriteData.resize(Tiles.size());
	InstColorData.resize(Tiles.size());

	int RenderCount = 0;

	for (std::pair<const __int64, FTileData>& TilePair : Tiles)
	{
		FTileData& Tile = TilePair.second;
		FTileIndex Index;

		Unit.SetTexture("TileMapTex", Sprite->GetTexture(Tile.SpriteIndex));
		Tile.SpriteData = Sprite->GetSpriteData(Tile.SpriteIndex);
		Tile.SpriteData.Pivot = { 0.0f, 0.0f };

		Index.Key = TilePair.first;

		FVector ConvertPos = TileIndexToWorldPos(Index);

		Pos.Position({ ConvertPos.X, ConvertPos.Y, 0.0f });

		Trans.WVP = Scale * Pos * RendererTrans.View * RendererTrans.Projection;

		InstTransform[RenderCount] = Trans;
		InstColorData[RenderCount] = Tile.ColorData;
		InstSpriteData[RenderCount] = Tile.SpriteData;
		++RenderCount;
	}

	Unit.StructuredBufferLinkData("TransformBuffer", InstTransform);
	Unit.StructuredBufferLinkData("SpriteDataBuffer", InstSpriteData);
	Unit.StructuredBufferLinkData("ColorDataBuffer", InstColorData);

	Unit.RenderInst(_Camera, static_cast<UINT>(Tiles.size()), _DeltaTime);*/
}

void InstanceRectRenderer::InstancingOn()
{
	RenderType = ERenderType::Instancing;
}