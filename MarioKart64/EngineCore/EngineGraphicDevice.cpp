#include "PreCompile.h"
#include "EngineGraphicDevice.h"
#include "EngineTexture.h"
#include "EngineDepthStencilState.h"

UEngineGraphicDevice::UEngineGraphicDevice()
{
}

UEngineGraphicDevice::~UEngineGraphicDevice()
{
    Release();
}

void UEngineGraphicDevice::Release()
{
    MainAdapter = nullptr;
    DXBackBufferTexture = nullptr;
    RTV = nullptr;
    SwapChain = nullptr;
    Context = nullptr;
    Device = nullptr;
}

IDXGIAdapter* UEngineGraphicDevice::GetHighPerFormanceAdapter()
{
    IDXGIFactory* Factory = nullptr;
    IDXGIAdapter* ResultAdapter = nullptr;
    unsigned __int64 MaxVideoMemory = 0;

    HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));

    if (nullptr == Factory)
    {
        MSGASSERT("그래픽카드 조사용 팩토리 생성에 실패했습니다.");
        return nullptr;
    }

    for (int Index = 0;; ++Index)
    {
        IDXGIAdapter* CurAdapter = nullptr;

        Factory->EnumAdapters(Index, &CurAdapter);

        if (nullptr == CurAdapter)
        {
            break;
        }

        DXGI_ADAPTER_DESC Desc;
        CurAdapter->GetDesc(&Desc);

        if (MaxVideoMemory <= Desc.DedicatedVideoMemory)
        {
            MaxVideoMemory = Desc.DedicatedVideoMemory;
            if (nullptr != ResultAdapter)
            {
                ResultAdapter->Release();
            }

            ResultAdapter = CurAdapter;
            continue;
        }

        CurAdapter->Release();
    }

    if (nullptr != Factory)
    {
        Factory->Release();
    }

    if (nullptr == ResultAdapter)
    {
        MSGASSERT("그래픽카드가 달려있지 않은 컴퓨터입니다.");
        return nullptr;
    }

    //int memG = MaxVideoMemory / (1024 * 1024 * 1024);

    return ResultAdapter;
}

void UEngineGraphicDevice::CreateDeviceAndContext()
{
    MainAdapter = GetHighPerFormanceAdapter();

    int iFlag = 0;

#ifdef _DEBUG
//  D3D11_CREATE_DEVICE_SINGLETHREADED = 0x1,
//	D3D11_CREATE_DEVICE_DEBUG = 0x2,
//	D3D11_CREATE_DEVICE_SWITCH_TO_REF = 0x4,
//	D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS = 0x8,
//	D3D11_CREATE_DEVICE_BGRA_SUPPORT = 0x20,
//	D3D11_CREATE_DEVICE_DEBUGGABLE = 0x40,
//	D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY = 0x80,
//	D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT = 0x100,
//	D3D11_CREATE_DEVICE_VIDEO_SUPPORT = 0x800
    iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

    D3D_FEATURE_LEVEL ResultLevel;

    HRESULT Result = D3D11CreateDevice(
        MainAdapter.Get(),
        D3D_DRIVER_TYPE::D3D_DRIVER_TYPE_UNKNOWN,
        nullptr,
        iFlag,
        nullptr,
        0,
        D3D11_SDK_VERSION,
        &Device,
        &ResultLevel, 
        &Context);

    if (nullptr == Device)
    {
        MSGASSERT("그래픽 디바이스 생성에 실패했습니다.");
        return;
    }

    if (nullptr == Context)
    {
        MSGASSERT("그래픽 컨텍스트 생성에 실패했습니다.");
        return;
    }

    if (Result != S_OK)
    {
        MSGASSERT("뭔가 잘못됨.");
        return;
    }

    if (ResultLevel != D3D_FEATURE_LEVEL_11_0 && ResultLevel != D3D_FEATURE_LEVEL_11_1)
    {
        MSGASSERT("다이렉트 11버전을 지원하지 않는 그래픽카드 입니다.");
        return;
    }

    Result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);

    if (Result != S_OK)
    {
        MSGASSERT("쓰레드 안정성 적용에 문제가 생겼습니다.");
        return;
    }

    DefaultResourcesInit();
}

void UEngineGraphicDevice::CreateBackBuffer(const UEngineWindow& _Window)
{	
    FVector Size = _Window.GetWindowSize();

	// Depth stencil buffer
    D3D11_TEXTURE2D_DESC Desc = {0};
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

    DepthTex = std::make_shared<UEngineTexture>();

    DepthTex->ResCreate(Desc);

	// Swap chain
    DXGI_SWAP_CHAIN_DESC ScInfo = {0};

    ScInfo.BufferCount = 2;
    ScInfo.BufferDesc.Width = Size.iX();
    ScInfo.BufferDesc.Height = Size.iY();
    ScInfo.OutputWindow = _Window.GetWindowHandle();
    ScInfo.Windowed = true;
    ScInfo.BufferDesc.RefreshRate.Denominator = 1;
    ScInfo.BufferDesc.RefreshRate.Numerator = 60;
    ScInfo.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    ScInfo.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    ScInfo.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    ScInfo.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT;
    ScInfo.SampleDesc.Quality = 0;
    ScInfo.SampleDesc.Count = 1;
    ScInfo.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    ScInfo.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    IDXGIFactory* pF = nullptr;
    MainAdapter->GetParent(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&pF));

    pF->CreateSwapChain(Device.Get(), &ScInfo, &SwapChain);

    pF->Release();
    MainAdapter->Release();

    if (nullptr == SwapChain)
    {
        MSGASSERT("스왑체인 제작에 실패했습니다.");
    }

    if (S_OK != SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &DXBackBufferTexture))
    {
        MSGASSERT("백버퍼 텍스처를 얻어오는데 실패했습니다.");
    };

    if (S_OK != Device->CreateRenderTargetView(DXBackBufferTexture.Get(), nullptr, &RTV))
    {
        MSGASSERT("텍스처 수정권한 획득에 실패했습니다");
    }
}

void UEngineGraphicDevice::RenderStart()
{
    FVector ClearColor{ 0.0f, 0.0f, 0.0f, 1.0f };
    Context->ClearRenderTargetView(RTV.Get(), ClearColor.Arr1D);
    Context->ClearDepthStencilView(DepthTex->GetDSV(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    ID3D11RenderTargetView* RTV = UEngineCore::GetDevice().GetRTV();
    ID3D11RenderTargetView* ArrRtv[16] = { 0 };
    ArrRtv[0] = RTV;
    Context->OMSetRenderTargets(1, &ArrRtv[0], DepthTex->GetDSV());
}


void UEngineGraphicDevice::RenderEnd()
{
    HRESULT Result = SwapChain->Present(0, 0);

    if (Result == DXGI_ERROR_DEVICE_REMOVED || Result == DXGI_ERROR_DEVICE_RESET)
    {
        MSGASSERT("해상도 변경이나 디바이스 관련 설정이 런타임 도중 수정되었습니다");
        return;
    }
}