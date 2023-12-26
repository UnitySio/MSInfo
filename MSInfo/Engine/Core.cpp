﻿#include "Core.h"
#include "Graphics/Graphics.h"

Core::Core() : class_name_(L"MSINFO")
{
}

ATOM Core::MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;
    ZeroMemory(&wcex, sizeof(WNDCLASSEX));

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = StaticWndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = static_cast<HBRUSH>(GetStockObject(BLACK_BRUSH));
    wcex.lpszClassName = class_name_;

    return RegisterClassEx(&wcex);
}

BOOL Core::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
    const int screen_width = GetSystemMetrics(SM_CXSCREEN);
    const int screen_height = GetSystemMetrics(SM_CYSCREEN);

    resolution_ = { 640, 480 };
    window_area_ = { 0, 0, resolution_.x, resolution_.y };
    AdjustWindowRect(&window_area_, WS_OVERLAPPEDWINDOW, FALSE);

    hWnd_ = CreateWindowEx(
        0,
        class_name_,
        L"메이플스토리 정보",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX,
        (screen_width - (window_area_.right - window_area_.left)) / 2,
        (screen_height - (window_area_.bottom - window_area_.top)) / 2,
        window_area_.right - window_area_.left,
        window_area_.bottom - window_area_.top,
        nullptr,
        nullptr,
        hInstance,
        nullptr
    );

    if (!hWnd_) return FALSE;
    ShowWindow(hWnd_, nCmdShow);
    
    return TRUE;
}

bool Core::InitWindow(HINSTANCE hInstance, int nCmdShow)
{
    MyRegisterClass(hInstance);
    
    if (!InitInstance(hInstance, nCmdShow)) return false;
    if (!Graphics::GetInstance()->Init()) return false;

    logic_handle_ = CreateThread(nullptr, 0, LogicThread, nullptr, 0, nullptr);
    
    return true;
}

LRESULT Core::StaticWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return GetInstance()->WndProc(hWnd, message, wParam, lParam);
}

LRESULT Core::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if (message == WM_DESTROY)
    {
        is_running_ = false;
        WaitForSingleObject(logic_handle_, INFINITE);
        
        Graphics::GetInstance()->Release();
        GetInstance()->Release();
        
        PostQuitMessage(0);
        return 0;
    }
    
    return DefWindowProc(hWnd, message, wParam, lParam);
}

DWORD Core::LogicThread(LPVOID lpParam)
{
    GetInstance()->is_running_ = true;
    while (GetInstance()->is_running_)
    {
        GetInstance()->MainLogic();
    }

    return 0;
}

void Core::MainLogic()
{
    Graphics::GetInstance()->BeginRenderD3D();
    Graphics::GetInstance()->BeginRenderD2D();

    Render();

    Graphics::GetInstance()->EndRenderD2D();
    Graphics::GetInstance()->EndRenderD3D();
}

void Core::Tick(float delta_time)
{
}

void Core::Render()
{
}
