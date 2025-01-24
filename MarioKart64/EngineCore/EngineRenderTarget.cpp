#include "PreCompile.h"
#include "EngineRenderTarget.h"

UEngineRenderTarget::UEngineRenderTarget()
{
    TargetUnit.SetMesh("FullRect");
    TargetUnit.SetMaterial("TargetMerge");
}

UEngineRenderTarget::~UEngineRenderTarget()
{
}


void UEngineRenderTarget::CreateTarget(float4 _Scale, float4 _ClearColor /*= float4::NONE*/, DXGI_FORMAT _Format /*= DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT*/)
{
    ClearColor = _ClearColor;

    std::shared_ptr<class UEngineTexture> NewTarget = std::make_shared<UEngineTexture>();

    D3D11_TEXTURE2D_DESC Desc = { 0 };
    Desc.ArraySize = 1;
    Desc.Width = _Scale.iX();
    Desc.Height = _Scale.iY();
    Desc.Format = _Format;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    Desc.MipLevels = 1;
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.CPUAccessFlags = 0;
    Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET | D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;

    NewTarget->ResCreate(Desc);
    NewTarget->Size = _Scale;

    ArrRTVs.push_back(NewTarget->GetRTV());
    ArrTexture.push_back(NewTarget);
}


void UEngineRenderTarget::CreateTarget(Microsoft::WRL::ComPtr<ID3D11Texture2D> _Texture2D)
{
	std::shared_ptr<class UEngineTexture> NewTarget = std::make_shared<UEngineTexture>();
	NewTarget->ResCreate(_Texture2D);


    if (nullptr == NewTarget->GetRTV())
    {
        MSGASSERT("랜더타겟 뷰가 존재하지 않는 랜더타겟 입니다.");
    }
    ;

    ArrRTVs.push_back(NewTarget->GetRTV());

    ArrTexture.push_back(NewTarget);
}

void UEngineRenderTarget::CreateDepth(int _Index)
{
    if (ArrTexture.size() <= _Index)
    {
        MSGASSERT("존재하지 않는 텍스처 인덱스로 깊이버퍼를 만들려고 했습니다.");
    }

    FVector Size = ArrTexture[_Index]->GetTextureSize();

    D3D11_TEXTURE2D_DESC Desc = { 0 };
    Desc.ArraySize = 1;
    Desc.Width = Size.iX();
    Desc.Height = Size.iY();
    Desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

    Desc.SampleDesc.Count = 1;
    Desc.SampleDesc.Quality = 0;

    Desc.MipLevels = 1;
    Desc.Usage = D3D11_USAGE_DEFAULT;
    Desc.CPUAccessFlags = 0;
    Desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;

    DepthTexture = std::make_shared<UEngineTexture>();
    DepthTexture->ResCreate(Desc);

}

void UEngineRenderTarget::Clear()
{
    for (size_t i = 0; i < ArrRTVs.size(); i++)
    {
        UEngineCore::GetDevice().GetContext()->ClearRenderTargetView(ArrRTVs[i], ClearColor.Arr1D);
    }

    ID3D11DepthStencilView* DSV = nullptr;

    if (nullptr != DepthTexture)
    {
        DSV = DepthTexture->GetDSV();
    }

    if (nullptr != DSV)
    {
        UEngineCore::GetDevice().GetContext()->ClearDepthStencilView(DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
    }
}

void UEngineRenderTarget::Setting()
{
    ID3D11DepthStencilView* DSV = nullptr;

    if (nullptr != DepthTexture)
    {
        DSV = DepthTexture->GetDSV();
    }

    UEngineCore::GetDevice().GetContext()->OMSetRenderTargets(1, &ArrRTVs[0], DSV);
}

void UEngineRenderTarget::CopyTo(std::shared_ptr<UEngineRenderTarget> _Target)
{
    _Target->Clear();
    MergeTo(_Target);
}

void UEngineRenderTarget::CopyTo(UEngineRenderTarget* _Target)
{
    _Target->Clear();
    MergeTo(_Target);
}

void UEngineRenderTarget::MergeTo(UEngineRenderTarget* _Target)
{
    _Target->Setting();
    TargetUnit.SetTexture("MergeTex", ArrTexture[0]);
    TargetUnit.Render(nullptr, 0.0f);
    TargetUnit.Reset();
}

void UEngineRenderTarget::MergeTo(std::shared_ptr<UEngineRenderTarget> _Target)
{
    _Target->Setting();
    TargetUnit.SetTexture("MergeTex", ArrTexture[0]);
    TargetUnit.Render(nullptr, 0.0f);
    TargetUnit.Reset();
}

void UEngineRenderTarget::Effect(UEngineCamera* _Camera, float _DeltaTime)
{
    for (std::shared_ptr<UPostEffect>& Effect : PosEffects)
    {
        if (false == Effect->IsActive)
        {
            continue;
        }

        Effect->Effect(_Camera, _DeltaTime);
    }
}

