#pragma once

#ifndef WINUTIL_H
#define WINUTIL_H

#include <stdint.h>

#ifdef WINUTIL_EXPORT
#error Macro 'WINUTIL_EXPORT' already defined.
#endif // WINUTIL_EXPORT

#ifdef WINUTIL_BUILD
#define WINUTIL_EXPORT	__declspec(dllexport)
#else
#define WINUTIL_EXPORT	__declspec(dllimport)
#endif // WINUTIL_BUILD

#ifdef WINUTIL_STATIC
#undef WINUTIL_EXPORT
#define WINUTIL_EXPORT
#endif // WINUTIL_STATIC

#define WINUTIL_API			__stdcall
#define WINUTIL_CALLBACK	__stdcall

#define WINUTIL_TYPENAME(name)	struct __##name { int unused; }; typedef struct __##name* name

typedef char ubool;

typedef enum Access
{
	AccessNone,
	AccessRead,
	AccessWrite,
	AccessAll
} Access;

typedef enum ConflictBehavior
{
	ConflictError,
	ConflictForce,
	ConflictSkip,
	ConflictCancel
} ConflictBehavior;

typedef void(WINUTIL_CALLBACK* WinUtilCallback)(void*);

#endif // !WINUTIL_H
