#include "PreCompile.h"
#include "EngineGraphicDevice.h"
#include "EngineVertex.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Mesh.h"

void UEngineGraphicDevice::DefaultResourcesInit()
{
	MeshInit();
}

void UEngineGraphicDevice::MeshInit()
{
	int a = 0;

	{
		std::vector<EngineVertex> Vertexs;
		Vertexs.resize(4);
		Vertexs[0] = EngineVertex{ FVector(-0.5f, 0.5f, -0.0f), {0.0f , 0.0f }, {1.0f, 0.0f, 0.0f, 1.0f} };
		Vertexs[1] = EngineVertex{ FVector(0.5f, 0.5f, -0.0f), {1.0f , 0.0f } , {0.0f, 1.0f, 0.0f, 1.0f} };
		Vertexs[2] = EngineVertex{ FVector(-0.5f, -0.5f, -0.0f), {0.0f , 1.0f } , {0.0f, 0.0f, 1.0f, 1.0f} };
		Vertexs[3] = EngineVertex{ FVector(0.5f, -0.5f, -0.0f), {1.0f , 1.0f } , {1.0f, 1.0f, 1.0f, 1.0f} };

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