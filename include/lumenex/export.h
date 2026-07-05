#ifndef LUMENEX_EXPORT_H
#define LUMENEX_EXPORT_H

#if defined(_WIN32) && defined(LUMENEX_SHARED)
#if defined(LUMENEX_BUILDING)
#define LUMENEX_API __declspec(dllexport)
#else
#define LUMENEX_API __declspec(dllimport)
#endif
#else
#define LUMENEX_API
#endif

#endif
