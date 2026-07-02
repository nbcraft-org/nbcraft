/********************************************************************
	Minecraft: Pocket Edition - Decompilation Project
	Copyright (C) 2023 iProgramInCpp

	The following code is licensed under the BSD 1 clause license.
	SPDX-License-Identifier: BSD-1-Clause
 ********************************************************************/

#pragma once

#include "compat/PlatformDefinitions.h"

#ifdef USE_SDL
    #include "thirdparty/SDL/SDL.h"

    // because SDL sucks and makes no sense and sets bit 1<<30 for some keycodes for some godamn reason
    enum eSDLVirtualKeys
    {
        #define CODE(x) SDLVK_ ## x,
        #define _CODE(x) SDLVK_ ## x
        #include "SDLKeyCodes.h"
        #undef  CODE
        #undef  _CODE
    };
#endif

#if MC_PLATFORM_WINPC
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#elif defined(__APPLE__) && !defined(USE_SDL)
    // https://i.stack.imgur.com/LD8pT.png
    #define AKEYCODE_FORWARD_DEL 0x75
    #define AKEYCODE_ARROW_LEFT  0x7B
    #define AKEYCODE_ARROW_RIGHT 0x7C
    #define AKEYCODE_DEL         0x33
    #define AKEYCODE_ENTER       0x24
#endif

#ifdef USE_SDL
#define AKEYCODE_FORWARD_DEL   SDLK_DELETE
#define AKEYCODE_ARROW_LEFT    SDLK_LEFT
#define AKEYCODE_ARROW_RIGHT   SDLK_RIGHT
#define AKEYCODE_DEL           SDLK_BACKSPACE
#define AKEYCODE_ENTER         SDLK_RETURN
#define AKEYCODE_A             SDLK_a
#define AKEYCODE_Z             SDLK_z
#define AKEYCODE_0             SDLK_0
#define AKEYCODE_9             SDLK_9
#define AKEYCODE_SPACE         SDLK_SPACE
#define AKEYCODE_COMMA         SDLK_COMMA
#define AKEYCODE_PERIOD        SDLK_PERIOD
#define AKEYCODE_PLUS          SDLK_PLUS
#define AKEYCODE_MINUS         SDLK_MINUS
#define AKEYCODE_SEMICOLON     SDLK_SEMICOLON
#define AKEYCODE_SLASH         SDLK_SLASH
#define AKEYCODE_GRAVE         SDLK_BACKQUOTE
#define AKEYCODE_BACKSLASH     SDLK_BACKSLASH
#define AKEYCODE_APOSTROPHE    SDLK_QUOTE
#define AKEYCODE_LEFT_BRACKET  SDLK_LEFTBRACKET
#define AKEYCODE_RIGHT_BRACKET SDLK_RIGHTBRACKET
#elif defined(_WIN32)
// See https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
#define AKEYCODE_FORWARD_DEL   VK_DELETE
#define AKEYCODE_ARROW_LEFT    VK_LEFT
#define AKEYCODE_ARROW_RIGHT   VK_RIGHT
#define AKEYCODE_DEL               VK_BACK
#define AKEYCODE_ENTER         VK_RETURN
#define AKEYCODE_A             'A'
#define AKEYCODE_Z             'Z'
#define AKEYCODE_0             '0'
#define AKEYCODE_9             '9'
#define AKEYCODE_SPACE         VK_SPACE
#define AKEYCODE_COMMA         VK_OEM_COMMA
#define AKEYCODE_PERIOD        VK_OEM_PERIOD
#define AKEYCODE_PLUS          VK_OEM_PLUS
#define AKEYCODE_MINUS         VK_OEM_MINUS
#define AKEYCODE_SEMICOLON     VK_OEM_1
#define AKEYCODE_SLASH         VK_OEM_2
#define AKEYCODE_GRAVE         VK_OEM_3
#define AKEYCODE_BACKSLASH     VK_OEM_5
#define AKEYCODE_APOSTROPHE    VK_OEM_7
#define AKEYCODE_LEFT_BRACKET  VK_OEM_4
#define AKEYCODE_RIGHT_BRACKET VK_OEM_6
#endif

#ifdef USE_NATIVE_ANDROID
    #include <android/keycodes.h>

    #define AKEYCODE_ARROW_LEFT  AKEYCODE_DPAD_LEFT
    #define AKEYCODE_ARROW_RIGHT AKEYCODE_DPAD_RIGHT
#endif
