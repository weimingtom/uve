/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2013 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/

/**
 *  \file SDL_system.h
 *  
 *  Include file for platform specific SDL API functions
 */

#ifndef _SDL_system_h
#define _SDL_system_h

#include "SDL_stdinc.h"

#if defined(__IPHONEOS__) && __IPHONEOS__
#include "SDL_video.h"
#include "SDL_keyboard.h"
#endif

#include "begin_code.h"
/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
/* *INDENT-OFF* */
extern "C" {
/* *INDENT-ON* */
#endif

/* Platform specific functions for iOS */
#if defined(__IPHONEOS__) && __IPHONEOS__

extern DECLSPEC int SDLCALL SDL_iPhoneSetAnimationCallback(SDL_Window * window, int interval, void (*callback)(void*), void *callbackParam);
extern DECLSPEC void SDLCALL SDL_iPhoneSetEventPump(SDL_bool enabled);

#endif /* __IPHONEOS__ */


/* Platform specific functions for Android */
#if defined(__ANDROID__) && __ANDROID__

/* Get the JNI environment for the current thread
   This returns JNIEnv*, but the prototype is void* so we don't need jni.h
 */
extern DECLSPEC void * SDLCALL SDL_AndroidGetJNIEnv();

/* Get the SDL Activity object for the application
   This returns jobject, but the prototype is void* so we don't need jni.h
 */
extern DECLSPEC void * SDLCALL SDL_AndroidGetActivity();

/* See the official Android developer guide for more information:
   http://developer.android.com/guide/topics/data/data-storage.html
*/
#define SDL_ANDROID_EXTERNAL_STORAGE_READ   0x01
#define SDL_ANDROID_EXTERNAL_STORAGE_WRITE  0x02

/* Get the path used for internal storage for this application.
   This path is unique to your application and cannot be written to
   by other applications.
 */
extern DECLSPEC const char * SDLCALL SDL_AndroidGetInternalStoragePath();

/* Get the current state of external storage, a bitmask of these values:
    SDL_ANDROID_EXTERNAL_STORAGE_READ
    SDL_ANDROID_EXTERNAL_STORAGE_WRITE
   If external storage is currently unavailable, this will return 0.
*/
extern DECLSPEC int SDLCALL SDL_AndroidGetExternalStorageState();

/* Get the path used for external storage for this application.
   This path is unique to your application, but is public and can be
   written to by other applications.
 */
extern DECLSPEC const char * SDLCALL SDL_AndroidGetExternalStoragePath();

#endif /* __ANDROID__ */


/* Platform specific functions for Windows RT */
#if defined(__WINRT__) && __WINRT__

/**
 *  \brief Windows RT / Windows Phone path types
 */
typedef enum
{
    /** \brief The installed app's root directory.
        Files here are likely to be read-only. */
    SDL_WINRT_PATH_INSTALLED_LOCATION,

    /** \brief The app's local data store.  Files may be written here */
    SDL_WINRT_PATH_LOCAL_FOLDER,

    /** \brief The app's roaming data store.  Unsupported on Windows Phone.
        Files written here may be copied to other machines via a network
        connection.
    */
    SDL_WINRT_PATH_ROAMING_FOLDER,

    /** \brief The app's temporary data store.  Unsupported on Windows Phone.
        Files written here may be deleted at any time. */
    SDL_WINRT_PATH_TEMP_FOLDER
} SDL_WinRT_Path;


/**
 *  \brief Retrieves a Windows RT defined path on the local file system
 *
 *  \note Documentation on most app-specific path types on Windows RT
 *      can be found on MSDN, at the URL:
 *      http://msdn.microsoft.com/en-us/library/windows/apps/hh464917.aspx
 *
 *  \param pathType The type of path to retrieve.
 *  \ret A UCS-2 string (16-bit, wide-char) containing the path, or NULL
 *      if the path is not available for any reason.  Not all paths are
 *      available on all versions of Windows.  This is especially true on
 *      Windows Phone.  Check the documentation for the given
 *      SDL_WinRT_Path for more information on which path types are
 *      supported where.
 */
extern DECLSPEC const wchar_t * SDLCALL SDL_WinRTGetFSPathUNICODE(SDL_WinRT_Path pathType);

/**
 *  \brief Retrieves a Windows RT defined path on the local file system
 *
 *  \note Documentation on most app-specific path types on Windows RT
 *      can be found on MSDN, at the URL:
 *      http://msdn.microsoft.com/en-us/library/windows/apps/hh464917.aspx
 *
 *  \param pathType The type of path to retrieve.
 *  \ret A UTF-8 string (8-bit, multi-byte) containing the path, or NULL
 *      if the path is not available for any reason.  Not all paths are
 *      available on all versions of Windows.  This is especially true on
 *      Windows Phone.  Check the documentation for the given
 *      SDL_WinRT_Path for more information on which path types are
 *      supported where.
 */
extern DECLSPEC const char * SDLCALL SDL_WinRTGetFSPathUTF8(SDL_WinRT_Path pathType);


#endif /* __WINRT__ */


/* Ends C function definitions when using C++ */
#ifdef __cplusplus
/* *INDENT-OFF* */
}
/* *INDENT-ON* */
#endif
#include "close_code.h"

#endif /* _SDL_system_h */

/* vi: set ts=4 sw=4 expandtab: */
