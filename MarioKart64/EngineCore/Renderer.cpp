#include "PreCompile.h"
#include "Renderer.h"
#include <EngineBase/EngineString.h>
#include <EngineCore/EngineCamera.h>
#include <EngineCore/EngineTexture.h>
#include <EngineCore/Mesh.h>
#include "EngineVertex.h"
#include "EngineBlend.h"

URenderer::URenderer()
{
}

URenderer::~URenderer()
{

}

void URenderer::SetSprite(UEngineSprite* _Sprite)
{
	Sprite = _Sprite;

	if (nullptr == Sprite)
	{
		MSGASSERT("존재하지 않는 스프라이트를 사용하려고 했습니다.");
	}
}

void URenderer::SetSprite(std::string_view _Value)
{
	std::string UpperName = UEngineString::ToUpper(_Value);

	Sprite = UEngineSprite::Find<UEngineSprite>(UpperName).get();

	if (nullptr == Sprite)
	{
		MSGASSERT("존재하지 않는 스프라이트를 사용하려고 했습니다.");
	}
}

void URenderer::SetOrder(int _Order)
{
	int PrevOrder = GetOrder();
	UObject::SetOrder(_Order);

	ULevel* Level = GetActor()->GetWorld();

	std::shared_ptr<URenderer> RendererPtr = GetThis<URenderer>();
	Level->ChangeRenderGroup(0, PrevOrder, RendererPtr);
}

ENGINEAPI void URenderer::BeginPlay()
{
	USceneComponent::BeginPlay();
	SetOrder(0);
	
	VertexShaderInit();
	RasterizerInit();
	PixelShaderInit();
	ShaderResInit();
}

void URenderer::ShaderResInit()
{
	{
		D3D11_BUFFER_DESC BufferInfo = { 0 };
		BufferInfo.ByteWidth = sizeof(FTransform);
		BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		BufferInfo.Usage = D3D11_USAGE_DYNAMIC;

		if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, nullptr, &TransformConstBuffer))
		{
			MSGASSERT("상수버퍼 생성에 실패했습니다..");
			return;
		}
	}

	{
		D3D11_BUFFER_DESC BufferInfo = { 0 };
		BufferInfo.ByteWidth = sizeof(FSpriteData);
		BufferInfo.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		BufferInfo.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		BufferInfo.Usage = D3D11_USAGE_DYNAMIC;

		if (S_OK != UEngineCore::GetDevice().GetDevice()->CreateBuffer(&BufferInfo, nullptr, &SpriteConstBuffer))
		{
			MSGASSERT("상수버퍼 생성에 실패했습니다..");
			return;
		}
	}

	D3D11_SAMPLER_DESC SampInfo = { D3D11_FILTER::D3D11_FILTER_MIN_MAG_MIP_LINEAR };
	SampInfo.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	SampInfo.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	SampInfo.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	SampInfo.ComparisonFunc = D3D11_COMPARISON_NEVER;
	SampInfo.MinLOD = 0;
	SampInfo.MaxLOD = D3D11_FLOAT32_MAX;

	UEngineCore::GetDevice().GetDevice()->CreateSamplerState(&SampInfo, &SamplerState);
}

void URenderer::ShaderResSetting()
{
	{
		FTransform& RendererTrans = GetTransformRef();
		D3D11_MAPPED_SUBRESOURCE Data = {};

		UEngineCore::GetDevice().GetContext()->Map(TransformConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);

		if (nullptr == Data.pData)
		{
			MSGASSERT("그래픽카드가 수정을 거부했습니다.");
		}
		memcpy_s(Data.pData, sizeof(FTransform), &RendererTrans, sizeof(FTransform));
		UEngineCore::GetDevice().GetContext()->Unmap(TransformConstBuffer.Get(), 0);

		ID3D11Buffer* ArrPtr[16] = { TransformConstBuffer.Get() };
		UEngineCore::GetDevice().GetContext()->VSSetConstantBuffers(0, 1, ArrPtr);
	}

	{
		D3D11_MAPPED_SUBRESOURCE Data = {};
		
		UEngineCore::GetDevice().GetContext()->Map(SpriteConstBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &Data);

		if (nullptr == Data.pData)
		{
			MSGASSERT("그래픽카드가 수정을 거부했습니다.");
		}
		memcpy_s(Data.pData, sizeof(FSpriteData), &SpriteData, sizeof(FSpriteData));
		UEngineCore::GetDevice().GetContext()->Unmap(SpriteConstBuffer.Get(), 0);

		ID3D11Buffer* ArrPtr[16] = { SpriteConstBuffer.Get() };
		UEngineCore::GetDevice().GetContext()->VSSetConstantBuffers(1, 1, ArrPtr);
	}
	
	ID3D11ShaderResourceView* ArrSRV[16] = { Sprite->GetSRV() };
	UEngineCore::GetDevice().GetContext()->PSSetShaderResources(0, 1, ArrSRV);

	ID3D11SamplerState* ArrSMP[16] = { SamplerState.Get() };
	UEngineCore::GetDevice().GetContext()->PSSetSamplers(0, 1, ArrSMP);
}

void URenderer::Render(UEngineCamera* _Camera, float _DeltaTime)
{
	FTransform& CameraTrans = _Camera->GetTransformRef();

	FTransform& RendererTrans = GetTransformRef();
	
	RendererTrans.View = CameraTrans.View;
	RendererTrans.Projection = CameraTrans.Projection;

	RendererTrans.WVP = RendererTrans.World * RendererTrans.View * RendererTrans.Projection;

	
	if (nullptr == Mesh)
	{
		MSGASSERT("매쉬가 세팅되지 않아서 랜더링을 할수 없습니다.");
		return;
	}
	
	ShaderResSetting();
	InputAssembler1Setting();
	VertexShaderSetting();
	InputAssembler2Setting();
	RasterizerSetting();
	PixelShaderSetting();
	OutPutMergeSetting();

	UEngineCore::GetDevice().GetContext()->DrawIndexed(6, 0, 0);
}

void URenderer::InputAssembler1Setting()
{
	Mesh->GetVertexBuffer()->Setting();
	UEngineCore::GetDevice().GetContext()->IASetInputLayout(InputLayOut.Get());
}

void URenderer::InputAssembler1LayOut()
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> InputLayOutData;
	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "POSITION";
		Desc.InputSlot = 0;
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		Desc.AlignedByteOffset = 0;
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

		Desc.SemanticIndex = 0;
		Desc.InstanceDataStepRate = 0;
		InputLayOutData.push_back(Desc);
	}

	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "TEXCOORD";
		Desc.InputSlot = 0;
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		Desc.AlignedByteOffset = 16;
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

		Desc.SemanticIndex = 0;
		Desc.InstanceDataStepRate = 0;
		InputLayOutData.push_back(Desc);
	}

	{
		D3D11_INPUT_ELEMENT_DESC Desc;
		Desc.SemanticName = "COLOR";
		Desc.InputSlot = 0;
		Desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		Desc.AlignedByteOffset = 32;
		Desc.InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;

		Desc.SemanticIndex = 0;
		Desc.InstanceDataStepRate = 0;
		InputLayOutData.push_back(Desc);
	}

	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateInputLayout(
		&InputLayOutData[0],
		static_cast<unsigned int>(InputLayOutData.size()),
		VSShaderCodeBlob->GetBufferPointer(),
		VSShaderCodeBlob->GetBufferSize(),
		&InputLayOut);

	if (S_OK != Result)
	{
		MSGASSERT("인풋 레이아웃 생성에 실패했습니다");
	}
}

void URenderer::VertexShaderInit()
{
	UEngineDirectory CurDir;
	CurDir.MoveParentToDirectory("EngineShader");
	UEngineFile File = CurDir.GetFile("EngineSpriteShader.fx");

	std::string Path = File.GetPathToString();

	std::wstring WPath = UEngineString::AnsiToUnicode(Path);

	std::string version = "vs_5_0";

	int Flag0 = 0;
	int Flag1 = 0;

#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif

	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	D3DCompileFromFile(
		WPath.c_str(),
		nullptr,
		nullptr,
		"VertexToWorld",
		version.c_str(),
		Flag0,
		Flag1,
		&VSShaderCodeBlob,
		&VSErrorCodeBlob
	);

	if (nullptr == VSShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(VSErrorCodeBlob->GetBufferPointer());
		MSGASSERT("쉐이더 코드 중간빌드에서 실패했습니다\n" + ErrString);
		return;
	}

	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreateVertexShader(
		VSShaderCodeBlob->GetBufferPointer(),
		VSShaderCodeBlob->GetBufferSize(),
		nullptr,
		&VertexShader
	);

	if (S_OK != Result)
	{
		MSGASSERT("버텍스 쉐이더 생성에 실패했습니다.");
	}

	InputAssembler1LayOut();
}

void URenderer::VertexShaderSetting()
{
	UEngineCore::GetDevice().GetContext()->VSSetShader(VertexShader.Get(), nullptr, 0);
}

void URenderer::RasterizerInit()
{
	D3D11_RASTERIZER_DESC Desc = {};
	Desc.CullMode = D3D11_CULL_MODE::D3D11_CULL_NONE;
	Desc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
	UEngineCore::GetDevice().GetDevice()->CreateRasterizerState(&Desc, &RasterizerState);

	ViewPortInfo.Width = 1280.0f;
	ViewPortInfo.Height = 720.0f;
	ViewPortInfo.TopLeftX = 0.0f;
	ViewPortInfo.TopLeftY = 0.0f;
	ViewPortInfo.MinDepth = 0.0f;
	ViewPortInfo.MaxDepth = 1.0f;
}

void URenderer::RasterizerSetting()
{
	UEngineCore::GetDevice().GetContext()->RSSetViewports(1, &ViewPortInfo);
	UEngineCore::GetDevice().GetContext()->RSSetState(RasterizerState.Get());
}

void URenderer::InputAssembler2Setting()
{

	Mesh->GetIndexBuffer()->Setting();
	UEngineCore::GetDevice().GetContext()->IASetPrimitiveTopology(Topology);
}

void URenderer::PixelShaderInit()
{
	UEngineDirectory CurDir;
	CurDir.MoveParentToDirectory("EngineShader");
	UEngineFile File = CurDir.GetFile("EngineSpriteShader.fx");

	std::string Path = File.GetPathToString();
	std::wstring WPath = UEngineString::AnsiToUnicode(Path);

	std::string version = "ps_5_0";

	int Flag0 = 0;
	int Flag1 = 0;

#ifdef _DEBUG
	Flag0 = D3D10_SHADER_DEBUG;
#endif

	Flag0 |= D3DCOMPILE_PACK_MATRIX_ROW_MAJOR;

	D3DCompileFromFile(
		WPath.c_str(),
		nullptr,
		nullptr,
		"PixelToWorld",
		version.c_str(),
		Flag0,
		Flag1,
		&PSShaderCodeBlob,
		&PSErrorCodeBlob
	);

	if (nullptr == PSShaderCodeBlob)
	{
		std::string ErrString = reinterpret_cast<char*>(PSErrorCodeBlob->GetBufferPointer());
		MSGASSERT("쉐이더 코드 중간빌드에서 실패했습니다\n" + ErrString);
		return;
	}

	HRESULT Result = UEngineCore::GetDevice().GetDevice()->CreatePixelShader(
		PSShaderCodeBlob->GetBufferPointer(),
		PSShaderCodeBlob->GetBufferSize(),
		nullptr,
		&PixelShader
	);

	if (S_OK != Result)
	{
		MSGASSERT("픽셀 쉐이더 생성에 실패했습니다.");
	}
}

void URenderer::PixelShaderSetting()
{
	UEngineCore::GetDevice().GetContext()->PSSetShader(PixelShader.Get(), nullptr, 0);
}

void URenderer::OutPutMergeSetting()
{
	if (nullptr != Blend)
	{
		Blend->Setting();
	}

	ID3D11RenderTargetView* RTV = UEngineCore::GetDevice().GetRTV();

	ID3D11RenderTargetView* ArrRtv[16] = { 0 };
	ArrRtv[0] = RTV; // SV_Target0

	UEngineCore::GetDevice().GetContext()->OMSetRenderTargets(1, &ArrRtv[0], nullptr);
}

void URenderer::SetSpriteData(size_t _Index)
{
	SpriteData = Sprite->GetSpriteData(_Index);
}

void URenderer::SetMesh(std::string_view _Name)
{
	std::shared_ptr<UMesh> FindMesh = UMesh::Find<UMesh>(_Name);

	Mesh = FindMesh.get();

	if (nullptr == Mesh)
	{
		MSGASSERT("존재하지 않는 매쉬를 세팅할수 없습니다.\n");
	}
}

void URenderer::SetBlend(std::string_view _Name)
{
	std::shared_ptr<UEngineBlend> FindBlend = UEngineBlend::Find<UEngineBlend>(_Name);

	Blend = FindBlend.get();

	if (nullptr == Blend)
	{
		MSGASSERT("존재하지 않는 Blend를 세팅할수 없습니다.\n");
	}
}