#pragma once

#ifndef WNDCLASS_H
#define WNDCLASS_H

#include <wtypes.h>
#include "WinUtilBase.h"

#define CS_RECOMMEND	0x0003

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	typedef WinUtilCallback MsgLoopIdle;

	WINUTIL_EXPORT ATOM RegisterWndClassA(UINT style, HINSTANCE hInstance, LPCSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HCURSOR hCursor, LPCSTR lpszMenuName);
	WINUTIL_EXPORT ATOM RegisterWndClassW(UINT style, HINSTANCE hInstance, LPCWSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HCURSOR hCursor, LPCWSTR lpszMenuName);
	WINUTIL_EXPORT ATOM RegisterWndClassExA(UINT style, HINSTANCE hInstance, LPCSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HICON hIconSm, HCURSOR hCursor, LPCSTR lpszMenuName);
	WINUTIL_EXPORT ATOM RegisterWndClassExW(UINT style, HINSTANCE hInstance, LPCWSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HICON hIconSm, HCURSOR hCursor, LPCWSTR lpszMenuName);

	WINUTIL_EXPORT int StartMsgLoop(MsgLoopIdle IdleCallback, void* IdleParam);
	WINUTIL_EXPORT void SetMsgLoopIdleCallback(MsgLoopIdle IdleCallback);
	WINUTIL_EXPORT void SetMsgLoopIdleParam(void* IdleParam);
	WINUTIL_EXPORT void SetMsgLoopIdleAttributes(MsgLoopIdle IdleCallback, void* IdleParam);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !WNDCLASS_H
