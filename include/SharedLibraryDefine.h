#ifndef SHAREDLIBRARYDEFINE_H
#define SHAREDLIBRARYDEFINE_H

#if defined( _WIN32 ) || defined( _WIN64 )
    #define Q_DECL_EXPORT __declspec(dllexport)
    #define Q_DECL_IMPORT __declspec(dllimport)
#else
    #define Q_DECL_EXPORT
    #define Q_DECL_IMPORT
#endif

#if defined( _WIN32 ) || defined( _WIN64 )
    #ifdef ENABLE_SHARED_LIBRARY
    #  define SHAREDLIBRARYDEFINE_EXPORT Q_DECL_EXPORT
    #  pragma warning(disable:4251)
    #else
    #  define SHAREDLIBRARYDEFINE_EXPORT Q_DECL_IMPORT
    #endif
#else
    #define SHAREDLIBRARYDEFINE_EXPORT
#endif

//! 任意のnamespaceを定義する。
#define _MYNAMESPACE_ MyMPI

#endif // SHAREDLIBRARYDEFINE_H

