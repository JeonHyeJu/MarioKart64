#include "PreCompile.h"
#include "ObjRenderer.h"
#include "TextureLoader.h"
#include <EngineCore/EngineGraphicDevice.h>
#include <EngineCore/EngineCamera.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

void Throwanerror(LPCSTR errormessage)
{
	throw std::runtime_error(errormessage);
}

// Temp
HRESULT	CompileShaderFromFile(LPCWSTR pFileName, const D3D_SHADER_MACRO* pDefines, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3DBlob** ppBytecodeBlob)
{
	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;
	//UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;

#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(pFileName, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryPoint, pShaderModel, compileFlags, 0, ppBytecodeBlob, &pErrorBlob);
	if (FAILED(result))
	{
		if (pErrorBlob != nullptr)
		{
			OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
		}
	}

	if (pErrorBlob != nullptr)
	{
		pErrorBlob->Release();
	}

	return result;
}

ObjRenderer::ObjRenderer()
{
}

ObjRenderer::~ObjRenderer()
{
	for (AiMesh& mesh : Meshes)
	{
		mesh.Close();
	}

	for (TEXTURE& tex : Textures)
	{
		tex.Release();
	}
}

void ObjRenderer::Init(std::string_view _path)
{
	ObjPath = _path.data() + std::string(".obj");
	MtlPath = _path.data() + std::string(".mtl");
}

void ObjRenderer::Init(std::string_view _objPath, std::string_view _mtlPath)
{
	ObjPath = _objPath;
	MtlPath = _mtlPath;
}

void ObjRenderer::BeginPlay()
{
	USceneComponent::BeginPlay();	// Don't use URenderer::BeginPlay()
	SetOrder(0);

	InitShader();
	ShaderResInit();
	RasterizerInit();

	LoadModel(ObjPath, MtlPath);
}

// TODO: NemuMat27.. part
void ObjRenderer::Render(UEngineCamera* _Camera, float _DeltaTime)
{
	FTransform& CameraTrans = _Camera->GetTransformRef();
	FTransform& RendererTrans = GetTransformRef();

	RendererTrans.View = CameraTrans.View;
	RendererTrans.Projection = CameraTrans.Projection;
	RendererTrans.WVP = RendererTrans.World * CameraTrans.View * CameraTrans.Projection;
	//OutputDebugStringA((std::to_string(CameraTrans.View.Arr2D[3][2]) + "\n").c_str());

	UEngineCore::GetDevice().GetContext()->IASetPrimitiveTopology(Topology);

	ShaderResSetting();
	VertexShaderSetting();
	PixelShaderSetting();
	UEngineCore::GetDevice().GetContext()->IASetInputLayout(InputLayOut.Get());

	RasterizerSetting();
	OutPutMergeSetting();

	for (size_t i = 0; i < Meshes.size(); ++i) {
		Meshes[i].Draw(UEngineCore::GetDevice().GetContext());
	}
}

void ObjRenderer::InitShader()
{
	UEngineDirectory dir;
	dir.MoveParentToDirectory("MarioKart64");
	UEngineFile fVs = dir.GetFile("VertexShader.fx");
	UEngineFile fPs = dir.GetFile("PixelShader.fx");
	std::wstring vsPath = UEngineString::AnsiToUnicode(fVs.GetPathToString());
	std::wstring psPath = UEngineString::AnsiToUnicode(fPs.GetPathToString());

	if (FAILED(CompileShaderFromFile(vsPath.c_str(), 0, "main", "vs_5_0", &VSShaderCodeBlob)))
	{
		MSGASSERT("Failed to compile vertex shader from file");
	}
	if (FAILED(CompileShaderFromFile(psPath.c_str(), 0, "main", "ps_5_0", &PSShaderCodeBlob)))
	{
		MSGASSERT("Failed to compile pixel shader from file");
	}

	UEngineCore::GetDevice().GetDevice()->CreateVertexShader(VSShaderCodeBlob->GetBufferPointer(), VSShaderCodeBlob->GetBufferSize(), nullptr, &VertexShader);
	UEngineCore::GetDevice().GetDevice()->CreatePixelShader(PSShaderCodeBlob->GetBufferPointer(), PSShaderCodeBlob->GetBufferSize(), nullptr, &PixelShader);

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	UEngineCore::GetDevice().GetDevice()->CreateInputLayout(ied, 2, VSShaderCodeBlob->GetBufferPointer(), VSShaderCodeBlob->GetBufferSize(), &InputLayOut);
}

void ObjRenderer::RasterizerInit()
{
	D3D11_RASTERIZER_DESC Desc = {};
	Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
	Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	UEngineCore::GetDevice().GetDevice()->CreateRasterizerState(&Desc, &RasterizerState);

	ViewPortInfo.Width = 1280.0f;
	ViewPortInfo.Height = 720.0f;
	ViewPortInfo.TopLeftX = 0.0f;
	ViewPortInfo.TopLeftY = 0.0f;
	ViewPortInfo.MinDepth = 0.0f;
	ViewPortInfo.MaxDepth = 1.0f;
}

void ObjRenderer::ShaderResInit()
{
	D3D11_BUFFER_DESC BufferInfo = { 0 };
	BufferInfo.ByteWidth = sizeof(FTransform);
	BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	BufferInfo.CPUAccessFlags = 0;
	BufferInfo.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr = UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, nullptr, &TransformConstBuffer);

	if (FAILED(hr))
	{
		MSGASSERT("Constant buffer couldn't be created");
	}

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = UEngineCore::GetDevice().GetDevice()->CreateSamplerState(&sampDesc, &SamplerState);
	if (FAILED(hr))
	{
		MSGASSERT("Texture sampler state couldn't be created");
	}
}

void ObjRenderer::ShaderResSetting()
{
	FTransform& RendererTrans = GetTransformRef();
	D3D11_MAPPED_SUBRESOURCE Data = {};

	UEngineCore::GetDevice().GetContext()->UpdateSubresource(TransformConstBuffer.Get(), 0, nullptr, &RendererTrans, 0, 0);

	ID3D11Buffer* arrPtr[16] = { TransformConstBuffer.Get() };
	UEngineCore::GetDevice().GetContext()->VSSetConstantBuffers(0, 1, arrPtr);

	ID3D11SamplerState* arrSMP[16] = { SamplerState.Get() };
	UEngineCore::GetDevice().GetContext()->PSSetSamplers(0, 1, arrSMP);
}

bool ObjRenderer::LoadModel(std::string_view _objPath, std::string_view _mtlPath)
{
	std::string fileName = _objPath.data();
	Assimp::Importer importer;

	unsigned int flag;
	flag = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded;
	//flag = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_GenNormals;
	//flag = aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace | aiProcess_GenNormals | aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder;

	const aiScene* pScene = importer.ReadFile(fileName, flag);

	if (pScene == nullptr)
	{
		return false;
	}

	Directory = fileName.substr(0, fileName.find_last_of("/\\"));

	ProcessNode(pScene->mRootNode, pScene);
	return true;
}

void ObjRenderer::ProcessNode(aiNode* node, const aiScene* scene)
{
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		Meshes.push_back(ProcessMesh(mesh, scene));
	}

	for (UINT i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

AiMesh ObjRenderer::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<VERTEX> vertices;
	std::vector<UINT> indices;
	std::vector<TEXTURE> textures;

	for (UINT i = 0; i < mesh->mNumVertices; i++)
	{
		VERTEX vertex;

		vertex.X = mesh->mVertices[i].x;
		vertex.Y = mesh->mVertices[i].y;
		vertex.Z = mesh->mVertices[i].z;

		if (mesh->mTextureCoords[0]) {
			vertex.texcoord.x = (float)mesh->mTextureCoords[0][i].x;
			vertex.texcoord.y = (float)mesh->mTextureCoords[0][i].y;
		}

		vertices.push_back(vertex);
	}

	for (UINT i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for (UINT j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

		std::vector<TEXTURE> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", scene);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
	}

	return AiMesh(UEngineCore::GetDevice().GetDevice(), vertices, indices, textures);
}

std::vector<TEXTURE> ObjRenderer::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName, const aiScene* scene)
{
	std::vector<TEXTURE> textures;
	for (UINT i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		// Check if texture was loaded before and if so, continue to next iteration: skip loading a new texture
		bool skip = false;
		for (UINT j = 0; j < Textures.size(); j++)
		{
			if (std::strcmp(Textures[j].path.c_str(), str.C_Str()) == 0)
			{
				textures.push_back(Textures[j]);
				skip = true; // A texture with the same filepath has already been loaded, continue to next one. (optimization)
				break;
			}
		}
		if (!skip)	// If texture hasn't been loaded already, load it
		{
			HRESULT hr;
			TEXTURE texture;

			const aiTexture* embeddedTexture = scene->GetEmbeddedTexture(str.C_Str());
			if (embeddedTexture != nullptr)
			{
				texture.texture = LoadEmbeddedTexture(embeddedTexture);
			}
			else
			{
				std::string filename = Directory + "\\" + std::string(str.C_Str());

				std::wstring filenamews = std::wstring(filename.begin(), filename.end());
				hr = CreateWICTextureFromFile(
					UEngineCore::GetDevice().GetDevice(), UEngineCore::GetDevice().GetContext(),
					filenamews.c_str(), nullptr, &texture.texture
				);

				if (FAILED(hr))
				{
					MSGASSERT("Texture couldn't be loaded");
				}
			}

			texture.type = typeName;
			texture.path = str.C_Str();
			textures.push_back(texture);
			Textures.push_back(texture);  // Store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
		}
	}

	return textures;
}

ID3D11ShaderResourceView* ObjRenderer::LoadEmbeddedTexture(const aiTexture* embeddedTexture)
{
	HRESULT hr;
	ID3D11ShaderResourceView* texture = nullptr;

	if (embeddedTexture->mHeight != 0)
	{
		// Load an uncompressed ARGB8888 embedded texture
		D3D11_TEXTURE2D_DESC desc;
		desc.Width = embeddedTexture->mWidth;
		desc.Height = embeddedTexture->mHeight;
		desc.MipLevels = 1;
		desc.ArraySize = 1;
		desc.SampleDesc.Count = 1;
		desc.SampleDesc.Quality = 0;
		desc.Usage = D3D11_USAGE_DEFAULT;
		desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.CPUAccessFlags = 0;
		desc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA subresourceData;
		subresourceData.pSysMem = embeddedTexture->pcData;
		subresourceData.SysMemPitch = embeddedTexture->mWidth * 4;
		subresourceData.SysMemSlicePitch = embeddedTexture->mWidth * embeddedTexture->mHeight * 4;

		ID3D11Texture2D* texture2D = nullptr;
		hr = UEngineCore::GetDevice().GetDevice()->CreateTexture2D(&desc, &subresourceData, &texture2D);
		if (FAILED(hr))
		{
			MSGASSERT("CreateTexture2D failed!");
		}

		hr = UEngineCore::GetDevice().GetDevice()->CreateShaderResourceView(texture2D, nullptr, &texture);
		if (FAILED(hr))
		{
			MSGASSERT("CreateShaderResourceView failed");
		}

		return texture;
	}

	// mHeight is 0, so try to load a compressed texture of mWidth bytes
	const size_t size = embeddedTexture->mWidth;

	hr = CreateWICTextureFromMemory(
		UEngineCore::GetDevice().GetDevice(), UEngineCore::GetDevice().GetContext(),
		reinterpret_cast<const unsigned char*>(embeddedTexture->pcData), size, 
		nullptr, &texture
	);

	if (FAILED(hr))
	{
		MSGASSERT("Texture couldn't be created from memory!");

	}

	return texture;
}

