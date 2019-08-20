// Desktop.SampleApp.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "Desktop.SampleApp.h"
#include <memory>

#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.Controls.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>
#include <winrt/Windows.UI.Xaml.Media.h>
#include <Windows.UI.Xaml.Hosting.DesktopWindowXamlSource.h>

namespace WU = winrt::Windows::UI;
namespace WUX = WU::Xaml;
namespace WUXC = ::WUX::Controls;
namespace WUXH = ::WUX::Hosting;
namespace WUXM = ::WUX::Media;

HINSTANCE hinst;

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
  UNREFERENCED_PARAMETER(lParam);
  switch (message)
  {
    case WM_INITDIALOG:
      return (INT_PTR)TRUE;

    case WM_COMMAND:
      if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
      {
        EndDialog(hDlg, LOWORD(wParam));
        return (INT_PTR)TRUE;
      }
      break;
  }
  return (INT_PTR)FALSE;
}

LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  switch (message)
  {
    case WM_CREATE: {
      auto createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
      auto dwxsn = static_cast<IDesktopWindowXamlSourceNative*>(createStruct->lpCreateParams);

      winrt::check_hresult(dwxsn->AttachToWindow(hWnd));

      HWND interopHwnd;
      winrt::check_hresult(dwxsn->get_WindowHandle(&interopHwnd));

      SetWindowPos(interopHwnd, 0, 0, 0, createStruct->cx, createStruct->cy, SWP_SHOWWINDOW);

      SetWindowLongPtrW(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(dwxsn));
    } break;
    case WM_WINDOWPOSCHANGED: {
      auto windowPos = reinterpret_cast<WINDOWPOS*>(lParam);
      auto dwxsn = reinterpret_cast<IDesktopWindowXamlSourceNative*>(GetWindowLongPtrW(hWnd, GWLP_USERDATA));

      HWND interopHwnd;
      winrt::check_hresult(dwxsn->get_WindowHandle(&interopHwnd));

      SetWindowPos(interopHwnd, 0, 0, 0, windowPos->cx, windowPos->cy, SWP_SHOWWINDOW);
    } break;
    case WM_COMMAND: {
      int wmId = LOWORD(wParam);
      // Parse the menu selections:
      switch (wmId)
      {
        case IDM_ABOUT:
          DialogBox(hinst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
          break;
        case IDM_EXIT:
          DestroyWindow(hWnd);
          break;
        default:
          return DefWindowProc(hWnd, message, wParam, lParam);
      }
    } break;
    case WM_PAINT: {
      PAINTSTRUCT ps;
      HDC hdc = BeginPaint(hWnd, &ps);
      EndPaint(hWnd, &ps);
    } break;
    case WM_DESTROY: {
      PostQuitMessage(0);
    } break;
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}


ATOM RegisterWindowClass()
{
  WNDCLASSEXW wcex;

  wcex.cbSize = sizeof(WNDCLASSEX);

  wcex.style = CS_HREDRAW | CS_VREDRAW;
  wcex.lpfnWndProc = WndProc;
  wcex.cbClsExtra = 0;
  wcex.cbWndExtra = 0;
  wcex.hInstance = hinst;
  wcex.hIcon = LoadIcon(hinst, MAKEINTRESOURCE(IDI_DESKTOPSAMPLEAPP));
  wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
  wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
  wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DESKTOPSAMPLEAPP);
  wcex.lpszClassName = L"DESKTOPSAMPLEAPP";
  wcex.hIconSm = LoadIcon(hinst, MAKEINTRESOURCE(IDI_SMALL));

  return ::RegisterClassExW(&wcex);
}

int APIENTRY
wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
  hinst = hInstance;

  winrt::init_apartment(winrt::apartment_type::single_threaded);

  WUXH::DesktopWindowXamlSource windowsXamlDesktopSource {};

  auto textBlock = WUXC::TextBlock();

  textBlock.Text(winrt::hstring(L"test!"));
  textBlock.Foreground(WUXM::SolidColorBrush(winrt::Windows::UI::Color { 0xFF, 0x75, 0xD5, 0x5 }));

  windowsXamlDesktopSource.Content(textBlock);

  auto windowAtom = RegisterWindowClass();

  auto desktopWindowXamlSourceNative = windowsXamlDesktopSource.as<IDesktopWindowXamlSourceNative>();

  HWND hWnd = CreateWindowW(
    MAKEINTATOM(windowAtom),
    L"Desktop.SampleApp",
    WS_OVERLAPPEDWINDOW,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    CW_USEDEFAULT,
    nullptr,
    nullptr,
    hinst,
    desktopWindowXamlSourceNative.get());

  if (!hWnd)
  {
    return -1;
  }

  auto desktopWindowXamlSourceNative2 = windowsXamlDesktopSource.try_as<IDesktopWindowXamlSourceNative2>();

  ShowWindow(hWnd, nCmdShow);
  UpdateWindow(hWnd);
  SetFocus(hWnd);

  HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DESKTOPSAMPLEAPP));

  MSG msg;
  // Main message loop:
  while (GetMessage(&msg, nullptr, 0, 0))
  {
    if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
    {
      BOOL translated = FALSE;
      if (desktopWindowXamlSourceNative2)
      {
        winrt::check_hresult(desktopWindowXamlSourceNative2->PreTranslateMessage(&msg, &translated));
      }

      if (!translated)
      {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
      }
    }
  }

  desktopWindowXamlSourceNative = nullptr;
  desktopWindowXamlSourceNative2 = nullptr;
  windowsXamlDesktopSource = nullptr;

  winrt::uninit_apartment();

  return (int)msg.wParam;
}
