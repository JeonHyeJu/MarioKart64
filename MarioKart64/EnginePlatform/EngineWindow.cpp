#include "PreCompile.h"
#include "EngineWindow.h"
#include <EngineBase/EngineDebug.h>

HINSTANCE UEngineWindow::hInstance = nullptr;
std::map<std::string, WNDCLASSEXA> UEngineWindow::WindowClasss;
std::map<HWND, UEngineWindow*> UEngineWindow::AllWindows;
std::function<bool(HWND, UINT, WPARAM, LPARAM)> UEngineWindow::CustomProc = nullptr;
int WindowCount = 0;
int WheelDir = 0;

int UEngineWindow::GetWheelDir()
{
    return WheelDir;
}

void UEngineWindow::SetCustomProc(std::function<bool(HWND, UINT, WPARAM, LPARAM)> _CustomProc)
{
    CustomProc = _CustomProc;
}

LRESULT CALLBACK UEngineWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (nullptr != CustomProc)
    {
        if (true == CustomProc(hWnd, message, wParam, lParam))
        {
            return true;
        }
    }

    switch (message)
    {
    case WM_CREATE:
        ++WindowCount;
        break;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_MOUSEWHEEL:
    {
        WheelDir = GET_WHEEL_DELTA_WPARAM(wParam);
    }
    break;
    case WM_SETFOCUS:
    {
        if (true == AllWindows.contains(hWnd))
        {
            AllWindows[hWnd]->IsFocusValue = true;
        }
    }
    break;
    case WM_KILLFOCUS:
    {
        if (true == AllWindows.contains(hWnd))
        {
            AllWindows[hWnd]->IsFocusValue = false;
        }
    }
    break;
    case WM_DESTROY:
        --WindowCount;
        if (0 >= WindowCount)
        {
            UEngineWindow::LoopActive = false;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


void UEngineWindow::EngineWindowInit(HINSTANCE _Instance)
{
    hInstance = _Instance;

    WNDCLASSEXA wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = nullptr;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = "Default";
    wcex.hIconSm = nullptr;
    CreateWindowClass(wcex);
}

int UEngineWindow::WindowMessageLoop(std::function<void()> _StartFunction, std::function<void()> _FrameFunction, std::function<void()> _EndFunction)
{
    MSG msg = MSG();

    if (nullptr != _StartFunction)
    {
        _StartFunction();
    }

    if (nullptr == _FrameFunction)
    {
        MSGASSERT("업데이트 펑션이 바인드 되어 있지 않습니다.");
        return 0;
    }

    while (true == LoopActive)
    {
        if(0 != PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        if (false == LoopActive)
        {
            break;
        }

        _FrameFunction();

        WheelDir = 0;
    }

    if (nullptr != _EndFunction)
    {
        _EndFunction();
    }

    return (int)msg.wParam;
}

void UEngineWindow::CreateWindowClass(const WNDCLASSEXA& _Class)
{
    std::map<std::string, WNDCLASSEXA>::iterator EndIter = WindowClasss.end();
    std::map<std::string, WNDCLASSEXA>::iterator FindIter = WindowClasss.find(std::string(_Class.lpszClassName));

    if (EndIter != FindIter)
    {
        MSGASSERT(std::string(_Class.lpszClassName) + " 같은 이름의 윈도우 클래스를 2번 등록했습니다");
        return;
    }

    RegisterClassExA(&_Class);

    WindowClasss.insert(std::pair{ _Class.lpszClassName, _Class });
}

UEngineWindow::UEngineWindow() 
{
    
}

UEngineWindow::~UEngineWindow()
{
    if (nullptr != WindowHandle)
    {
        DestroyWindow(WindowHandle);
        WindowHandle = nullptr;
    }
}

void UEngineWindow::Create(std::string_view _TitleName, std::string_view _ClassName)
{
    if (nullptr != WindowHandle)
    {
        return;
    }

    if (false == WindowClasss.contains(_ClassName.data()))
    {
        MSGASSERT(std::string(_ClassName) + " 등록하지 않은 클래스로 윈도우창을 만들려고 했습니다");
        return;
    }

    WindowHandle = CreateWindowA(_ClassName.data(), _TitleName.data(), WS_OVERLAPPEDWINDOW,
        0, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

    if (nullptr == WindowHandle)
    {
        MSGASSERT(std::string(_TitleName) + " 윈도우 생성에 실패했습니다.");
        return;
    }

    HDC WindowMainDC = GetDC(WindowHandle);

    AllWindows.insert({ WindowHandle, this });
}

void UEngineWindow::Open(std::string_view _TitleName /*= "Window"*/)
{
    if (0 == WindowHandle)
    {
        Create(_TitleName);
    }

    if (0 == WindowHandle)
    {
        return;
    }

	ShowWindow(WindowHandle, SW_SHOW);
    UpdateWindow(WindowHandle);
}

void UEngineWindow::SetWindowPosAndScale(FVector _Pos, FVector _Scale)
{
    WindowSize = _Scale;
    RECT Rc = { 0, 0, _Scale.iX(), _Scale.iY() };

    AdjustWindowRect(&Rc, WS_OVERLAPPEDWINDOW, FALSE);
    ::SetWindowPos(WindowHandle, nullptr, _Pos.iX(), _Pos.iY(), Rc.right - Rc.left, Rc.bottom - Rc.top, SWP_NOZORDER);
}

FVector UEngineWindow::GetMousePos()
{
    POINT MousePoint;

    GetCursorPos(&MousePoint);
    ScreenToClient(WindowHandle, &MousePoint);

    return FVector(MousePoint.x, MousePoint.y);
}


bool UEngineWindow::IsMouseScreenOut() const
{
    POINT MousePoint;

    GetCursorPos(&MousePoint);
    ScreenToClient(WindowHandle, &MousePoint);

    if (0.0f > MousePoint.x)
    {
        return true;
    }

    if (0.0f > MousePoint.y)
    {
        return true;
    }

    if (WindowSize.X < MousePoint.x)
    {
        return true;
    }

    if (WindowSize.Y < MousePoint.y)
    {
        return true;
    }

    return false;
}