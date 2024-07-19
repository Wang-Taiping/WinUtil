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

#define MAX_ACCELTABLE_ARRAY_ELEM	4096

thread_local struct
{
	MsgLoopIdle IdleCallback;
	void* IdleParam;
} MsgLoopIdleAttributes;

thread_local bool MsgLoopStatus = false;

struct NativeAccelTable
{
	HWND hWnd;
	HACCEL hAccel;
};

thread_local NativeAccelTable* AccelTable = nullptr;

static void DefaultMsgLoopIdle(void*)
{
	Sleep(10);
}

int StartMsgLoop(MsgLoopIdle IdleCallback, void* IdleParam)
{
	if (MsgLoopStatus) return -1;
	AccelTable = new NativeAccelTable[MAX_ACCELTABLE_ARRAY_ELEM];
	memset(AccelTable, 0, sizeof(NativeAccelTable) * MAX_ACCELTABLE_ARRAY_ELEM);
	MsgLoopIdleAttributes.IdleCallback = IdleCallback ? IdleCallback : DefaultMsgLoopIdle;
	MsgLoopIdleAttributes.IdleParam = IdleParam;
	MSG Msg;
	while (true) {
	QueryMessage:
		if (PeekMessageW(&Msg, nullptr, 0, 0, PM_REMOVE)) {
			if (Msg.message == WM_QUIT) break;
			// TODO: Translate...
			for (size_t i = 0; i < MAX_ACCELTABLE_ARRAY_ELEM; i++) {
				if (AccelTable[i].hWnd == Msg.hwnd && AccelTable[i].hAccel && TranslateAcceleratorW(AccelTable[i].hWnd, AccelTable[i].hAccel, &Msg))
					goto QueryMessage;
			}
			TranslateMessage(&Msg);
			DispatchMessageW(&Msg);
			if (Msg.message == WM_NCDESTROY) {
				for (size_t i = 0; i < MAX_ACCELTABLE_ARRAY_ELEM; i++) {
					if (AccelTable[i].hWnd == Msg.hwnd) {
						AccelTable[i].hAccel = nullptr;
						AccelTable[i].hWnd = nullptr;
					}
				}
			}
		}
		else MsgLoopIdleAttributes.IdleCallback(MsgLoopIdleAttributes.IdleParam);
	}
	delete[] AccelTable;
	return Msg.wParam;
}

void SetMsgLoopIdleCallback(MsgLoopIdle IdleCallback)
{
	MsgLoopIdleAttributes.IdleCallback = IdleCallback ? IdleCallback : DefaultMsgLoopIdle;
}

void SetMsgLoopIdleParam(void* IdleParam)
{
	MsgLoopIdleAttributes.IdleParam = IdleParam;
}

void SetMsgLoopIdleAttributes(MsgLoopIdle IdleCallback, void* IdleParam)
{
	MsgLoopIdleAttributes.IdleCallback = IdleCallback ? IdleCallback : DefaultMsgLoopIdle;
	MsgLoopIdleAttributes.IdleParam = IdleParam;
}
