#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class UMesh : public UEngineResources
{
public:
	UMesh();
	~UMesh();

	UMesh(const UMesh& _Other) = delete;
	UMesh(UMesh&& _Other) noexcept = delete;
	UMesh& operator=(const UMesh& _Other) = delete;
	UMesh& operator=(UMesh&& _Other) noexcept = delete;

	static std::shared_ptr<UMesh> Create(std::string_view _Name)
	{
		return Create(_Name, _Name, _Name);
	}

	static std::shared_ptr<UMesh> Create(std::string_view _Name, std::string_view _VertexBuffer, std::string_view _IndexBuffer);

	std::shared_ptr<UVertexBuffer> GetVertexBuffer()
	{
		return VertexBuffer;
	}

	std::shared_ptr<UIndexBuffer> GetIndexBuffer()
	{
		return IndexBuffer;
	}

protected:

private:
	std::shared_ptr<UVertexBuffer> VertexBuffer;
	std::shared_ptr<UIndexBuffer> IndexBuffer;

};

