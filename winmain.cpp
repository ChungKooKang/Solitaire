#include <Windows.h>
#include <gdiplus.h>
#include "Game.h"

#pragma comment (lib, "Gdiplus.lib")

const wchar_t gClassName[] = L"MyWindowClass";

solitaire::Game myGame;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevIstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nShowCmd
)
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);


	HWND hwnd;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = gClassName;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(nullptr, L"Failed to Register Window Class!", L"Error", MB_OK);
		return 0;
	}
	RECT wr = { 0, 0, 1024, 768 };
	AdjustWindowRect(&wr, WS_OVERLAPPED | WS_SYSMENU, FALSE);

	hwnd = CreateWindowEx(
		NULL,
		gClassName,
		L"Solitaire Game",
		WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL
	);
	if (!hwnd)
	{
		MessageBox(NULL, L"Failed To Create Window!", L"Error", MB_OK);
		return 0;
	}
	myGame.Init(hwnd);

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd);
	
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

	}
	
	myGame.Release();

	Gdiplus::GdiplusShutdown(gdiplusToken);
	return static_cast<int>(msg.wParam);
}

void OnPaint(HWND hwnd)
{
	HDC hdc;
	PAINTSTRUCT ps;

	hdc = BeginPaint(hwnd, &ps);

	Gdiplus::Graphics graphics(hdc);

	myGame.Draw(graphics);

	EndPaint(hwnd, &ps);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONUP :
		myGame.OnClick(LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_PAINT :
		OnPaint(hwnd);
		break;

	case WM_CLOSE :
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY :
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
		
	}
	return 0;
}