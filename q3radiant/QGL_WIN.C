/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.

This file is part of Quake III Arena source code.

Quake III Arena source code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Quake III Arena source code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/
/*
** QGL_WIN.C
**
** This file implements the operating system binding of GL to QGL function
** pointers.  When doing a port of Quake2 you must implement the following
** two functions:
**
** QGL_Init() - loads libraries, assigns function pointers, etc.
** QGL_Shutdown() - unloads libraries, NULLs function pointers
*/
#include "qgl.h"
#include <float.h>

HMODULE g_hGLDLL;
HMODULE g_hGLUDLL;

#	pragma warning (disable : 4113 4133 4047 )

int   ( WINAPI * qwglChoosePixelFormat )(HDC, CONST PIXELFORMATDESCRIPTOR *);
int   ( WINAPI * qwglDescribePixelFormat) (HDC, int, UINT, LPPIXELFORMATDESCRIPTOR);
int   ( WINAPI * qwglGetPixelFormat)(HDC);
BOOL  ( WINAPI * qwglSetPixelFormat)(HDC, int, CONST PIXELFORMATDESCRIPTOR *);
BOOL  ( WINAPI * qwglSwapBuffers)(HDC);

BOOL  ( WINAPI * qwglCopyContext)(HGLRC, HGLRC, UINT);
HGLRC ( WINAPI * qwglCreateContext)(HDC);
HGLRC ( WINAPI * qwglCreateLayerContext)(HDC, int);
BOOL  ( WINAPI * qwglDeleteContext)(HGLRC);
HGLRC ( WINAPI * qwglGetCurrentContext)(VOID);
HDC   ( WINAPI * qwglGetCurrentDC)(VOID);
PROC  ( WINAPI * qwglGetProcAddress)(LPCSTR);
BOOL  ( WINAPI * qwglMakeCurrent)(HDC, HGLRC);
BOOL  ( WINAPI * qwglShareLists)(HGLRC, HGLRC);
BOOL  ( WINAPI * qwglUseFontBitmaps)(HDC, DWORD, DWORD, DWORD);

BOOL  ( WINAPI * qwglUseFontOutlines)(HDC, DWORD, DWORD, DWORD, FLOAT,
                                           FLOAT, int, LPGLYPHMETRICSFLOAT);

BOOL ( WINAPI * qwglDescribeLayerPlane)(HDC, int, int, UINT,
                                            LPLAYERPLANEDESCRIPTOR);
int  ( WINAPI * qwglSetLayerPaletteEntries)(HDC, int, int, int,
                                                CONST COLORREF *);
int  ( WINAPI * qwglGetLayerPaletteEntries)(HDC, int, int, int,
                                                COLORREF *);
BOOL ( WINAPI * qwglRealizeLayerPalette)(HDC, int, BOOL);
BOOL ( WINAPI * qwglSwapLayerBuffers)(HDC, UINT);

BOOL ( WINAPI * qwglSwapIntervalEXT)( int interval );
BOOL ( WINAPI * qwglGetDeviceGammaRampEXT)( unsigned char *, unsigned char *, unsigned char * );
BOOL ( WINAPI * qwglSetDeviceGammaRampEXT)( const unsigned char *, const unsigned char *, const unsigned char * );
void ( APIENTRY * qglPointParameterfEXT)( GLenum param, GLfloat value );
void ( APIENTRY * qglPointParameterfvEXT)( GLenum param, const GLfloat *value );
void ( APIENTRY * qglColorTableEXT)( int, int, int, int, int, const void * );
void ( APIENTRY * qglSelectTextureSGIS)( GLenum );
void ( APIENTRY * qglMTexCoord2fSGIS)( GLenum, GLfloat, GLfloat );


// glu stuff
void (APIENTRY * qgluPerspective) (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
int  (APIENTRY * qgluBuild2DMipmaps) (GLenum target, GLint components, GLint width, GLint height, GLenum format, GLenum      type, const void  *data);
// added for plugins
void (APIENTRY * qgluLookAt)(
  GLdouble eyex,
  GLdouble eyey,
  GLdouble eyez,
  GLdouble centerx,
  GLdouble centery,
  GLdouble centerz,
  GLdouble upx,
  GLdouble upy,
  GLdouble upz);


/*
** QGL_Shutdown
**
** Unloads the specified DLL then nulls out all the proc pointers.
*/
void QGL_Shutdown()
{
	if (g_hGLDLL)
	{
    FreeLibrary(g_hGLDLL);
    g_hGLDLL = NULL;
	}
	if (g_hGLUDLL)
	{
    FreeLibrary(g_hGLUDLL);
    g_hGLUDLL = NULL;
	}

	qwglCopyContext              = NULL;
	qwglCreateContext            = NULL;
	qwglCreateLayerContext       = NULL;
	qwglDeleteContext            = NULL;
	qwglDescribeLayerPlane       = NULL;
	qwglGetCurrentContext        = NULL;
	qwglGetCurrentDC             = NULL;
	qwglGetLayerPaletteEntries   = NULL;
	qwglGetProcAddress           = NULL;
	qwglMakeCurrent              = NULL;
	qwglRealizeLayerPalette      = NULL;
	qwglSetLayerPaletteEntries   = NULL;
	qwglShareLists               = NULL;
	qwglSwapLayerBuffers         = NULL;
	qwglUseFontBitmaps           = NULL;
	qwglUseFontOutlines          = NULL;

	qwglChoosePixelFormat        = NULL;
	qwglDescribePixelFormat      = NULL;
	qwglGetPixelFormat           = NULL;
	qwglSetPixelFormat           = NULL;
	qwglSwapBuffers              = NULL;

	qwglSwapIntervalEXT	= NULL;

	qwglGetDeviceGammaRampEXT = NULL;
	qwglSetDeviceGammaRampEXT = NULL;

  qgluPerspective = NULL;
  qgluBuild2DMipmaps = NULL;

  qgluLookAt = NULL;
}

#	define GPA(h, a ) GetProcAddress( h, a )

/*
** QGL_Init
**
** This is responsible for binding our qgl function pointers to 
** the appropriate GL stuff.  In Windows this means doing a 
** LoadLibrary and a bunch of calls to GetProcAddress.  On other
** operating systems we need to do the right thing, whatever that
** might be.
** 
*/
int QGL_Init(const char *dllname, const char* pGluName )
{

  g_hGLDLL = LoadLibrary(dllname);
  if (g_hGLDLL == NULL)
  {
    return 0;
  }

  g_hGLUDLL = LoadLibrary(pGluName);
  if (g_hGLUDLL == NULL)
  {
    FreeLibrary(g_hGLDLL);
    g_hGLDLL = NULL;
    return 0;
  }


  qgluPerspective              = GPA(g_hGLUDLL, "gluPerspective");
  qgluBuild2DMipmaps           = GPA(g_hGLUDLL, "gluBuild2DMipmaps");
	// added for plugins
	qgluLookAt                   = GPA(g_hGLUDLL, "gluLookAt" );

	qwglCopyContext              = GPA(g_hGLDLL, "wglCopyContext" );
	qwglCreateContext            = GPA(g_hGLDLL, "wglCreateContext" );
	qwglCreateLayerContext       = GPA(g_hGLDLL, "wglCreateLayerContext" );
	qwglDeleteContext            = GPA(g_hGLDLL, "wglDeleteContext" );
	qwglDescribeLayerPlane       = GPA(g_hGLDLL, "wglDescribeLayerPlane" );
	qwglGetCurrentContext        = GPA(g_hGLDLL, "wglGetCurrentContext" );
	qwglGetCurrentDC             = GPA(g_hGLDLL, "wglGetCurrentDC" );
	qwglGetLayerPaletteEntries   = GPA(g_hGLDLL, "wglGetLayerPaletteEntries" );
	qwglGetProcAddress           = GPA(g_hGLDLL, "wglGetProcAddress" );
	qwglMakeCurrent              = GPA(g_hGLDLL, "wglMakeCurrent" );
	qwglRealizeLayerPalette      = GPA(g_hGLDLL, "wglRealizeLayerPalette" );
	qwglSetLayerPaletteEntries   = GPA(g_hGLDLL, "wglSetLayerPaletteEntries" );
	qwglShareLists               = GPA(g_hGLDLL, "wglShareLists" );
	qwglSwapLayerBuffers         = GPA(g_hGLDLL, "wglSwapLayerBuffers" );
	qwglUseFontBitmaps           = GPA(g_hGLDLL, "wglUseFontBitmapsA" );
	qwglUseFontOutlines          = GPA(g_hGLDLL, "wglUseFontOutlinesA" );

	qwglChoosePixelFormat        = GPA(g_hGLDLL, "wglChoosePixelFormat" );
	qwglDescribePixelFormat      = GPA(g_hGLDLL, "wglDescribePixelFormat" );
	qwglGetPixelFormat           = GPA(g_hGLDLL, "wglGetPixelFormat" );
	qwglSetPixelFormat           = GPA(g_hGLDLL, "wglSetPixelFormat" );
	qwglSwapBuffers              = GPA(g_hGLDLL, "wglSwapBuffers" );

	qwglSwapIntervalEXT = 0;
	qglPointParameterfEXT = 0;
	qglPointParameterfvEXT = 0;
	qglColorTableEXT = 0;
	qglSelectTextureSGIS = 0;
	qglMTexCoord2fSGIS = 0;

	return 1;
}

#pragma warning (default : 4113 4133 4047 )



