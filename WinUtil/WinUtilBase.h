#pragma once

#ifndef WINUTIL_H
#define WINUTIL_H

#include <stdint.h>

#ifdef WINUTIL_EXPORT
#undef WINUTIL_EXPORT
#endif // WINUTIL_EXPORT
#ifdef WINUTIL_BUILD
#define WINUTIL_EXPORT __declspec(dllexport)
#else
#define WINUTIL_EXPORT __declspec(dllimport)
#endif // WINUTIL_BUILD

#define TYPEHANDLE(name)	typedef struct __##name { int unused; } *name

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

typedef void(*WinUtilCallback)(void*);

#endif // !WINUTIL_H
