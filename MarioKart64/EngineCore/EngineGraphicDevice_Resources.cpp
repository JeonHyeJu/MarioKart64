#include "PreCompile.h"
#include "EngineGraphicDevice.h"
#include "EngineVertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"
#include "EngineBlend.h"

void UEngineGraphicDevice::DefaultResourcesInit()
{
	MeshInit();
	BlendInit();
}

void UEngineGraphicDevice::MeshInit()
{
	int a = 0;

	{
		std::vector<EngineVertex> Vertexs;
		Vertexs.resize(4);
		Vertexs[0] = EngineVertex{ FVector(-0.5f, 0.5f, 0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = EngineVertex{ FVector(0.5f, 0.5f, 0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = EngineVertex{ FVector(-0.5f, -0.5f, 0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = EngineVertex{ FVector(0.5f, -0.5f, 0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

		UVertexBuffer::Create("Rect", Vertexs);
	}

	{
		std::vector<unsigned int> Indexs;

		Indexs.push_back(0);
		Indexs.push_back(1);
		Indexs.push_back(2);

		Indexs.push_back(1);
		Indexs.push_back(3);
		Indexs.push_back(2);


		UIndexBuffer::Create("Rect", Indexs);
	}

	{
		UMesh::Create("Rect");
	}
}

void UEngineGraphicDevice::BlendInit()
{
	D3D11_BLEND_DESC Desc = {0};

	Desc.AlphaToCoverageEnable = false;
	Desc.IndependentBlendEnable = true;
	Desc.RenderTarget[0].BlendEnable = true;
	Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	Desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;

	Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

	UEngineBlend::Create("AlphaBlend", Desc);
}