#pragma once

#ifndef WNDCLASS_H
#define WNDCLASS_H

#include <wtypes.h>
#include "WinUtilBase.h"

#define CreateCentralWindowA(lpClassName, lpWindowName, dwStyle, nWidth, nHeight,\
	hWndParent, hMenu, hInstance, lpParam)\
	CreateWindowExA(0L, lpClassName, lpWindowName, dwStyle, \
	(GetSystemMetrics(SM_CXFULLSCREEN) - (nWidth)) / 2, \
	(GetSystemMetrics(SM_CYFULLSCREEN) - (nHeight)) / 2, \
	nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
#define CreateCentralWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, nWidth, nHeight,\
	hWndParent, hMenu, hInstance, lpParam)\
	CreateWindowExA(dwExStyle, lpClassName, lpWindowName, dwStyle, \
	(GetSystemMetrics(SM_CXFULLSCREEN) - (nWidth)) / 2, \
	(GetSystemMetrics(SM_CYFULLSCREEN) - (nHeight)) / 2, \
	nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
#define CreateCentralWindowW(lpszClassName, lpszWindowName, dwStyle, nWidth, nHeight,\
	hWndParent, hMenu, hInstance, lpParam)\
	CreateWindowExW(0L, lpszClassName, lpszWindowName, dwStyle, \
	(GetSystemMetrics(SM_CXFULLSCREEN) - (nWidth)) / 2, \
	(GetSystemMetrics(SM_CYFULLSCREEN) - (nHeight)) / 2, \
	nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
#define CreateCentralWindowExW(dwExStyle, lpszClassName, lpszWindowName, dwStyle, nWidth, nHeight,\
	hWndParent, hMenu, hInstance, lpParam)\
	CreateWindowExW(dwExStyle, lpszClassName, lpszWindowName, dwStyle, \
	(GetSystemMetrics(SM_CXFULLSCREEN) - (nWidth)) / 2, \
	(GetSystemMetrics(SM_CYFULLSCREEN) - (nHeight)) / 2, \
	nWidth, nHeight, hWndParent, hMenu, hInstance, lpParam)
#ifdef UNICODE
#define CreateCentralWindow		CreateCentralWindowW
#define CreateCentralWindowEx	CreateCentralWindowExW
#else
#define CreateCentralWindow		CreateCentralWindowA
#define CreateCentralWindowEx	CreateCentralWindowExA
#endif // !UNICODE

#define CS_RECOMMEND	0x0003

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
	typedef WinUtilCallback MsgLoopInit, MsgLoopIdle;

	WINUTIL_EXPORT ATOM WINUTIL_API RegisterWndClassA(UINT style, HINSTANCE hInstance, LPCSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HCURSOR hCursor, LPCSTR lpszMenuName);
	WINUTIL_EXPORT ATOM WINUTIL_API RegisterWndClassW(UINT style, HINSTANCE hInstance, LPCWSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HCURSOR hCursor, LPCWSTR lpszMenuName);
	WINUTIL_EXPORT ATOM WINUTIL_API RegisterWndClassExA(UINT style, HINSTANCE hInstance, LPCSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HICON hIconSm, HCURSOR hCursor, LPCSTR lpszMenuName);
	WINUTIL_EXPORT ATOM WINUTIL_API RegisterWndClassExW(UINT style, HINSTANCE hInstance, LPCWSTR lpszClassName, WNDPROC lpfnWndProc, HICON hIcon, HICON hIconSm, HCURSOR hCursor, LPCWSTR lpszMenuName);

	WINUTIL_EXPORT int WINUTIL_API StartMsgLoop(MsgLoopInit InitCallback, void* InitParam, MsgLoopIdle IdleCallback, void* IdleParam);
	WINUTIL_EXPORT void WINUTIL_API SetMsgLoopIdleCallback(MsgLoopIdle IdleCallback);
	WINUTIL_EXPORT void WINUTIL_API SetMsgLoopIdleParam(void* IdleParam);
	WINUTIL_EXPORT void WINUTIL_API SetMsgLoopIdleAttributes(MsgLoopIdle IdleCallback, void* IdleParam);
	WINUTIL_EXPORT ubool WINUTIL_API RegisterHWNDAttributes(HWND hWnd);
	WINUTIL_EXPORT ubool WINUTIL_API UnregisterHWNDAttributes(HWND hWnd);
	WINUTIL_EXPORT ubool WINUTIL_API SetHWNDAccelerator(HWND hWnd, HACCEL hAccel);
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // !WNDCLASS_H
