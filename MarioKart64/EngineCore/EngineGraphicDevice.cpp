#include "PreCompile.h"
#include "EngineGraphicDevice.h"

UEngineGraphicDevice::UEngineGraphicDevice()
{
}

UEngineGraphicDevice::~UEngineGraphicDevice()
{
    Release();
}

void UEngineGraphicDevice::Release()
{
    if (nullptr != Context)
    {
        Context->Release();
        Context = nullptr;
    }

    if (nullptr != Device)
    {
        Device->Release();
        Context = nullptr;
    }
}

IDXGIAdapter* UEngineGraphicDevice::GetHighPerFormanceAdapter()
{
    IDXGIFactory* Factory = nullptr;
    unsigned __int64 MaxVideoMemory = 0;
    IDXGIAdapter* ResultAdapter = nullptr;

    HRESULT HR = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(&Factory));

    if (nullptr == Factory)
    {
        MSGASSERT("�׷���ī�� ����� ���丮 ������ �����߽��ϴ�.");
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
        MSGASSERT("�׷���ī�尡 �޷����� ���� ��ǻ���Դϴ�.");
        return nullptr;
    }

    //int memG = MaxVideoMemory / (1024 * 1024 * 1024);

    return ResultAdapter;
}

void UEngineGraphicDevice::CreateDeviceAndContext()
{
    IDXGIAdapter* Adapter = GetHighPerFormanceAdapter();

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
    D3D11CreateDevice(
        Adapter, 
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
        MSGASSERT("�׷��� ����̽� ������ �����߽��ϴ�.");
        return;
    }

    if (nullptr == Context)
    {
        MSGASSERT("�׷��� ���ؽ�Ʈ ������ �����߽��ϴ�.");
        return;
    }

    Adapter->Release();
}

void UEngineGraphicDevice::CreateBackBuffer(const UEngineWindow& _Window)
{
	int a = 0;
}