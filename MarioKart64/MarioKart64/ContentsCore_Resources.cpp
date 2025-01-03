#include "PreCompile.h"
#include "ContentsCore.h"
#include <EngineCore/EngineGraphicDevice.h>
#include <EngineCore/EngineVertex.h>
#include <EngineCore/EngineIndexBuffer.h>
#include <EngineCore/Mesh.h>
#include <EngineCore/EngineBlend.h>
#include <EngineCore/EngineShader.h>
#include <EngineCore/EngineMaterial.h>
#include <EngineCore/EngineTexture.h>

void UContentsCore::MyGSetting()
{
	UEngineDirectory CurDir;
	CurDir.MoveParentToDirectory("MarioKart64");

	std::vector<UEngineFile> ShaderFiles = CurDir.GetAllFile(true, { ".fx", ".hlsl" });

	for (size_t i = 0; i < ShaderFiles.size(); i++)
	{
		UEngineShader::ReflectionCompile(ShaderFiles[i]);
	}

	std::shared_ptr<UEngineMaterial> Mat = UEngineMaterial::Create("MyMaterial");
	Mat->SetVertexShader("SpriteShader.fx");
	Mat->SetPixelShader("SpriteShader.fx");

}
