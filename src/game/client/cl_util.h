/***
*
*	Copyright (c) 1999, Valve LLC. All rights reserved.
*	
*	This product contains software technology licensed from Id 
*	Software, Inc. ("Id Technology").  Id Technology (c) 1996 Id Software, Inc. 
*	All Rights Reserved.
*
*   Use, distribution, and modification of this source code and/or resulting
*   object code is restricted to non-commercial enhancements to products from
*   Valve LLC.  All other use, distribution, or modification is prohibited
*   without written permission from Valve LLC.
*
****/
//
// cl_util.h
//

#ifndef CL_UTIL_H
#define CL_UTIL_H
#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif

#include <cstdio>
#include <cstdarg>
#include <cstring>
#include <algorithm>
#include <tier0/dbg.h>
#include "cvardef.h"
#include <convar.h>
#include "console.h"
#include "sprite_hook.h"

inline void HookMessage(const char *pszName, UserMsgHookFn fn)
{
	gEngfuncs.pfnHookUserMsg((char *)pszName, fn);
}

inline void HookCommand(const char *pszName, CmdFunction fn)
{
	gEngfuncs.pfnAddCommand((char *)pszName, fn);
}

inline float CVAR_GET_FLOAT(const char *x)
{
	return gEngfuncs.pfnGetCvarFloat(x);
}
inline const char *CVAR_GET_STRING(const char *x)
{
	return gEngfuncs.pfnGetCvarString(x);
}
inline struct cvar_s *CVAR_CREATE(const char *cv, const char *val, const int flags)
{
	return gEngfuncs.pfnRegisterVariable(cv, val, flags);
}

inline HSPRITE SPR_Load(const char *szPicName)
{
	return gEngfuncs.pfnSPR_Load(szPicName);
}

inline void SPR_Set(HSPRITE hPic, int r, int g, int b)
{
	gEngfuncs.pfnSPR_Set(hPic, r, g, b);
}

inline int SPR_Frames(HSPRITE hPic)
{
	return gEngfuncs.pfnSPR_Frames(hPic);
}

inline client_sprite_t *SPR_GetList(const char *const pszName, int *piCount)
{
	return gEngfuncs.pfnSPR_GetList(pszName, piCount);
}

// SPR_Draw  draws a the current sprite as solid
inline void SPR_Draw(int frame, int x, int y, const wrect_t *prc)
{
	gEngfuncs.pfnSPR_Draw(frame, x, y, prc);
}

// SPR_DrawHoles  draws the current sprites,  with color index255 not drawn (transparent)
inline void SPR_DrawHoles(int frame, int x, int y, const wrect_t *prc)
{
	gEngfuncs.pfnSPR_DrawHoles(frame, x, y, prc);
}

// SPR_DrawAdditive  adds the sprites RGB values to the background  (additive transulency)
inline void SPR_DrawAdditive(int frame, int x, int y, const wrect_t *prc)
{
	gEngfuncs.pfnSPR_DrawAdditive(frame, x, y, prc);
}

// SPR_EnableScissor  sets a clipping rect for HUD sprites.  (0,0) is the top-left hand corner of the screen.
inline void SPR_EnableScissor(int x, int y, int width, int height)
{
	gEngfuncs.pfnSPR_EnableScissor(x, y, width, height);
}

// SPR_DisableScissor  disables the clipping rect
inline void SPR_DisableScissor()
{
	gEngfuncs.pfnSPR_DisableScissor(); 
}

//
inline void FillRGBA(int x, int y, int width, int height, int r, int g, int b, int a)
{
	gEngfuncs.pfnFillRGBA(x, y, width, height, r, g, b, a);
}

// ScreenHeight returns the height of the screen, in pixels
#define ScreenHeight (CSpriteHook::Get().GetHudScreenHeight())

// ScreenWidth returns the width of the screen, in pixels
#define ScreenWidth (CSpriteHook::Get().GetHudScreenWidth())

#define BASE_XRES 640.f

// use this to project world coordinates to screen coordinates
#define XPROJECT(x) ((1.0f + (x)) * ScreenWidth * 0.5f)
#define YPROJECT(y) ((1.0f - (y)) * ScreenHeight * 0.5f)

#define XRES(x) (x * ((float)ScreenWidth / 640))
#define YRES(y) (y * ((float)ScreenHeight / 480))

#define GetScreenInfo   (*gEngfuncs.pfnGetScreenInfo)
#define ServerCmd       (*gEngfuncs.pfnServerCmd)
#define EngineClientCmd (*gEngfuncs.pfnClientCmd)
#define SetCrosshair    (*gEngfuncs.pfnSetCrosshair)
#define AngleVectors    (*gEngfuncs.pfnAngleVectors)

// Gets the height & width of a sprite,  at the specified frame
inline int SPR_Height(HSPRITE x, int f) { return gEngfuncs.pfnSPR_Height(x, f); }
inline int SPR_Width(HSPRITE x, int f) { return gEngfuncs.pfnSPR_Width(x, f); }

inline client_textmessage_t *TextMessageGet(const char *pName) { return gEngfuncs.pfnTextMessageGet(pName); }
inline int TextMessageDrawChar(int x, int y, int number, int r, int g, int b)
{
	return gEngfuncs.pfnDrawCharacter(x, y, number, r, g, b);
}

inline int TextMessageDrawString(int x, int y, const char *string, int r, int g, int b)
{
	return gEngfuncs.pfnDrawString(x, y, string, r, g, b);
}

inline int TextMessageDrawStringReverse(int x, int y, const char *string, int r, int g, int b)
{
	return gEngfuncs.pfnDrawStringReverse(x, y, string, r, g, b);
}

void ConsolePrint(const char *string);

inline void CenterPrint(const char *string)
{
	gEngfuncs.pfnCenterPrint(string);
}

inline char *safe_strcpy(char *dst, const char *src, int len_dst)
{
	if (len_dst <= 0)
	{
		return NULL; // this is bad
	}

	strncpy(dst, src, len_dst);
	dst[len_dst - 1] = '\0';

	return dst;
}

// sound functions
inline void PlaySound(char *szSound, float vol) { gEngfuncs.pfnPlaySoundByName(szSound, vol); }
inline void PlaySound(int iSound, float vol) { gEngfuncs.pfnPlaySoundByIndex(iSound, vol); }

using std::abs;
using std::max;
using std::min;

void ScaleColors(int &r, int &g, int &b, int a);

float Length(const float *v);
void VectorInverse(float *v);

// disable 'possible loss of data converting float to int' warning message
#pragma warning(disable : 4244)
// disable 'truncation from 'const double' to 'float' warning message
#pragma warning(disable : 4305)

inline void UnpackRGB(int &r, int &g, int &b, unsigned long ulRGB)
{
	r = (ulRGB & 0xFF0000) >> 16;
	g = (ulRGB & 0xFF00) >> 8;
	b = ulRGB & 0xFF;
}

HSPRITE LoadSprite(const char *pszName);

/**
 * Parses a string in format "RRR GGG BBB" where each component is integer [0; 255].
 * @param	string	Input string
 * @param	color	Output color (unchanged if returned false)
 * @return	Whether or not string was parsed successfully.
 */
bool ParseColor(const char *string, Color &color);

//-------------------------------------------------------------------
// Text drawing in console font
//-------------------------------------------------------------------

/**
 * Draws a string with console font
 * @param	x				Left position.
 * @param	y				Top position.
 * @param	string			String to draw. Note that it is temporarily modified while drawing.
 * @param	colorOverride	If set, color codes don't change color of the string.
 * @return Total width of the drawn string.
 */
int DrawConsoleString(int x, int y, char *string, const float *colorOverride = nullptr);

/**
 * Gets the dimensions in pixels of a console string if it were drawn onscreen.
 * @param	pszString	String to check.
 * @param	piLength	Pointer to a variable that will contain the total width of the string.
 * @param	piHeight	Pointer to a variable that will contain the height of the string.
*/
void GetConsoleStringSize(const char *string, int *width, int *height);

/**
 * Returns length of a console string if it were drawn on the screen.
 */
inline int ConsoleStringLen(const char *string)
{
	int _width, _height;
	GetConsoleStringSize(string, &_width, &_height);
	return _width;
}

//-------------------------------------------------------------------
// Color code utilities
//-------------------------------------------------------------------

/**
 * Checks c[0] and c[1] if they are '^' and a digit.
 */
inline bool IsColorCode(const char *s)
{
	return (s[0] == '^' && s[1] >= '0' && s[1] <= '9');
}

/**
 * Checks c[0] and c[1] if they are '^' and a digit.
 */
inline bool IsColorCode(const wchar_t *s)
{
	return (s[0] == L'^' && s[1] >= L'0' && s[1] <= L'9');
}

/**
 * Removes color codes from specified string.
 * @param	string	Input string.
 */
void RemoveColorCodesInPlace(char *string);

/**
 * Removes color codes from specified string.
 * @param	string	Input string.
 * @param	buf		Output buffer.
 * @param	bufSize	Output buffer size.
 * @return	Pointer to a static buffer.
 */
void RemoveColorCodes(const char *string, char *buf, size_t bufSize);

/**
 * Removes color codes from specified string.
 * @param	string	Input string.
 * @return	Pointer to a static buffer.
 */
const char *RemoveColorCodes(const char *string);

#endif
