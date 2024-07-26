#include "WndFrame.h"

ATOM RegisterWndClassA(UINT style, HINSTANCE hInstance, LPCSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HCURSOR hCursor, LPCSTR lpszMenuName)
{
	return RegisterWndClassExA(style, hInstance, lpszClassName, lpfnWndProc, hIcon, nullptr, hCursor, lpszMenuName);
}

ATOM RegisterWndClassW(UINT style, HINSTANCE hInstance, LPCWSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HCURSOR hCursor, LPCWSTR lpszMenuName)
{
	return RegisterWndClassExW(style, hInstance, lpszClassName, lpfnWndProc, hIcon, nullptr, hCursor, lpszMenuName);
}

ATOM RegisterWndClassExA(UINT style, HINSTANCE hInstance, LPCSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HICON hIconSm, HCURSOR hCursor, LPCSTR lpszMenuName)
{
	WNDCLASSEXA wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEXA);
	wcex.style = style;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = lpszClassName;
	wcex.lpfnWndProc = lpfnWndProc;
	wcex.hIcon = hIcon;
	wcex.hIconSm = hIconSm;
	wcex.hCursor = hCursor ? hCursor : LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW));
	wcex.lpszMenuName = lpszMenuName;
	return RegisterClassExA(&wcex);
}

ATOM RegisterWndClassExW(UINT style, HINSTANCE hInstance, LPCWSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HICON hIconSm, HCURSOR hCursor, LPCWSTR lpszMenuName)
{
	WNDCLASSEXW wcex = { 0 };
	wcex.cbSize = sizeof(WNDCLASSEXW);
	wcex.style = style;
	wcex.hInstance = hInstance;
	wcex.lpszClassName = lpszClassName;
	wcex.lpfnWndProc = lpfnWndProc;
	wcex.hIcon = hIcon;
	wcex.hIconSm = hIconSm;
	wcex.hCursor = hCursor ? hCursor : LoadCursorW(nullptr, MAKEINTRESOURCEW(IDC_ARROW));
	wcex.lpszMenuName = lpszMenuName;
	return RegisterClassExW(&wcex);
}

#define MAX_HWNDATTRIBUTES_NUMBER	4096

thread_local struct
{
	MsgLoopIdle IdleCallback;
	void* IdleParam;
} MsgLoopAttributes;

thread_local bool MsgLoopStatus = false;

struct NativeHWNDAttributes
{
	HWND hWnd;
	HACCEL hAccel;
	//WNDPROC hHook;
};

thread_local NativeHWNDAttributes* HWNDAttributes = nullptr;

static void WINUTIL_CALLBACK DefaultMsgLoopIdle(void*)
{
	Sleep(10);
}

int StartMsgLoop(MsgLoopInit InitCallback, void* InitParam, MsgLoopIdle IdleCallback, void* IdleParam)
{
	if (MsgLoopStatus) return -1;
	MsgLoopStatus = true;
	HWNDAttributes = new NativeHWNDAttributes[MAX_HWNDATTRIBUTES_NUMBER];
	memset(HWNDAttributes, 0, sizeof(NativeHWNDAttributes) * MAX_HWNDATTRIBUTES_NUMBER);
	MsgLoopAttributes.IdleCallback = IdleCallback ? IdleCallback : DefaultMsgLoopIdle;
	MsgLoopAttributes.IdleParam = IdleParam;
	if (InitCallback) InitCallback(InitParam);
	MSG Msg;
	while (true) {
	QueryMessage:
		if (PeekMessageW(&Msg, nullptr, 0, 0, PM_REMOVE)) {
			if (Msg.message == WM_QUIT) break;
			// TODO: Translate...
			for (size_t i = 0; i < MAX_HWNDATTRIBUTES_NUMBER; i++) {
				if (HWNDAttributes[i].hWnd == Msg.hwnd && HWNDAttributes[i].hAccel && TranslateAcceleratorW(HWNDAttributes[i].hWnd, HWNDAttributes[i].hAccel, &Msg))
					goto QueryMessage;
			}
			TranslateMessage(&Msg);
			DispatchMessageW(&Msg);
		}
		else MsgLoopAttributes.IdleCallback(MsgLoopAttributes.IdleParam);
	}
	delete[] HWNDAttributes;
	MsgLoopStatus = false;
	return Msg.wParam;
}

void SetMsgLoopIdleCallback(MsgLoopIdle IdleCallback)
{
	if (!MsgLoopStatus) return;
	MsgLoopAttributes.IdleCallback = IdleCallback ? IdleCallback : DefaultMsgLoopIdle;
}

void SetMsgLoopIdleParam(void* IdleParam)
{
	if (!MsgLoopStatus) return;
	MsgLoopAttributes.IdleParam = IdleParam;
}

void SetMsgLoopIdleAttributes(MsgLoopIdle IdleCallback, void* IdleParam)
{
	if (!MsgLoopStatus) return;
	MsgLoopAttributes.IdleCallback = IdleCallback ? IdleCallback : DefaultMsgLoopIdle;
	MsgLoopAttributes.IdleParam = IdleParam;
}

ubool WINUTIL_API RegisterHWNDAttributes(HWND hWnd)
{
	if (!MsgLoopStatus) return false;
	for (size_t i = 0; i < MAX_HWNDATTRIBUTES_NUMBER; i++) {
		if (HWNDAttributes[i].hWnd == hWnd) return false;
	}
	for (size_t i = 0; i < MAX_HWNDATTRIBUTES_NUMBER; i++) {
		if (!HWNDAttributes[i].hWnd) HWNDAttributes[i].hWnd = hWnd;
		return true;
	}
	return false;
}

ubool WINUTIL_API UnregisterHWNDAttributes(HWND hWnd)
{
	if (!MsgLoopStatus) return false;
	for (size_t i = 0; i < MAX_HWNDATTRIBUTES_NUMBER; i++) {
		if (HWNDAttributes[i].hWnd == hWnd) {
			memset(&HWNDAttributes[i], 0, sizeof(NativeHWNDAttributes));
			return true;
		}
	}
	return false;
}

ubool SetHWNDAccelerator(HWND hWnd, HACCEL hAccel)
{
	if (!MsgLoopStatus) return false;
	for (size_t i = 0; i < MAX_HWNDATTRIBUTES_NUMBER; i++) {
		if (HWNDAttributes[i].hWnd == hWnd) HWNDAttributes[i].hAccel = hAccel;
		return true;
	}
	return false;
}
