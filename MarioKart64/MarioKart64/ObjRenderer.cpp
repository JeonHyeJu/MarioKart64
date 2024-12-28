#include "PreCompile.h"
#include "ObjRenderer.h"
#include "TextureLoader.h"
#include <EngineCore/EngineGraphicDevice.h>
#include <EngineCore/EngineCamera.h>

#include <assimp\Importer.hpp>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

// Temp
struct ConstantBuffer {
	DirectX::XMMATRIX mWorld;
	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProjection;
};

// temp
DirectX::XMMATRIX m_World;
DirectX::XMMATRIX m_View;
DirectX::XMMATRIX m_Projection;

void Throwanerror(LPCSTR errormessage)
{
	throw std::runtime_error(errormessage);
}

// Temp
HRESULT	CompileShaderFromFile(LPCWSTR pFileName, const D3D_SHADER_MACRO* pDefines, LPCSTR pEntryPoint, LPCSTR pShaderModel, ID3DBlob** ppBytecodeBlob)
{
	UINT compileFlags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;

#ifdef _DEBUG
	compileFlags |= D3DCOMPILE_DEBUG;
#endif

	ID3DBlob* pErrorBlob = nullptr;

	HRESULT result = D3DCompileFromFile(pFileName, pDefines, D3D_COMPILE_STANDARD_FILE_INCLUDE, pEntryPoint, pShaderModel, compileFlags, 0, ppBytecodeBlob, &pErrorBlob);
	if (FAILED(result))
	{
		if (pErrorBlob != nullptr)
			OutputDebugStringA((LPCSTR)pErrorBlob->GetBufferPointer());
	}

	if (pErrorBlob != nullptr)
		pErrorBlob->Release();

	return result;
}

ObjRenderer::ObjRenderer()
{
}

ObjRenderer::~ObjRenderer()
{
}

void ObjRenderer::BeginPlay()
{
	USceneComponent::BeginPlay();	// Don't use URenderer::BeginPlay()
	SetOrder(0);

	InputAssembler1Init();
	VertexShaderInit();
	InputAssembler2Init();
	RasterizerInit();
	PixelShaderInit();
	ShaderResInit();
	RasterizerSetting();

	UEngineDirectory dir;
	dir.MoveParentToDirectory("MarioKart64\\Resources\\Models\\Courses\\Royal_Raceway");
	std::string path = dir.GetPathToString();

	LoadModel(path + "\\Royal_Raceway.obj", path + "\\Royal_Raceway.mtl");

	// Temp
	m_Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1280 / (float)720, 0.01f, 1000.0f);
}

void ObjRenderer::Render(UEngineCamera* _Camera, float _DeltaTime)
{
	FTransform& CameraTrans = _Camera->GetTransformRef();

	FTransform& RendererTrans = GetTransformRef();

	static float t = 0.0f;
	static ULONGLONG timeStart = 0;
	ULONGLONG timeCur = GetTickCount64();
	if (timeStart == 0)
		timeStart = timeCur;
	t = (timeCur - timeStart) / 1000.0f;

	m_World = DirectX::XMMatrixRotationY(-t);

	DirectX::XMVECTOR Eye = DirectX::XMVectorSet(0.0f, 5.0f, -3300.0f, 0.0f);
	DirectX::XMVECTOR At = DirectX::XMVectorSet(0.0f, 100.0f, 0.0f, 0.0f);
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 100.0f, 0.0f);
	m_View = DirectX::XMMatrixLookAtLH(Eye, At, Up);

	m_World = XMMatrixTranspose(m_World);
	m_View = XMMatrixTranspose(m_View);
	m_Projection = XMMatrixTranspose(m_Projection);

	RendererTrans.View.DirectMatrix = m_View;
	//RendererTrans.View = CameraTrans.View;
	RendererTrans.Projection = CameraTrans.Projection;
	RendererTrans.World.DirectMatrix = m_World;
	RendererTrans.Projection.DirectMatrix = m_Projection;
	RendererTrans.WVP = RendererTrans.World * RendererTrans.View * RendererTrans.Projection;

	ShaderResSetting();

	//InputAssembler1Setting();
	UEngineCore::GetDevice().GetContext()->IASetInputLayout(InputLayOut.Get());

	VertexShaderSetting();

	//InputAssembler2Setting();
	UEngineCore::GetDevice().GetContext()->IASetPrimitiveTopology(Topology);

	PixelShaderSetting();
	OutPutMergeSetting();

	for (size_t i = 0; i < Meshes.size(); ++i) {
		Meshes[i].Draw(UEngineCore::GetDevice().GetContext());
	}

	//UEngineCore::GetDevice().GetContext()->DrawIndexed(6, 0, 0);
}

bool ObjRenderer::LoadModel(std::string_view _objPath, std::string_view _mtlPath)
{
	std::string fileName = _objPath.data();
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (pScene == nullptr) return false;

	Directory = fileName.substr(0, fileName.find_last_of("/\\"));

	// process node
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
	// Data to fill
	std::vector<VERTEX> vertices;
	std::vector<UINT> indices;
	std::vector<TEXTURE> textures;

	// Walk through each of the mesh's vertices
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
			if (embeddedTexture != nullptr) {
				texture.texture = LoadEmbeddedTexture(embeddedTexture);
			}
			else {
				std::string filename = Directory + "\\" + std::string(str.C_Str());

				// std::string log = filename;
				// MessageBoxA(hwnd_, log.c_str(), "TEMP", MB_OK);

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

