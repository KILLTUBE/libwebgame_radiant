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
//-----------------------------------------------------------------------------
//
// $LogFile$
// $Revision: 1.7.2.3 $
// $Author: ttimo $
// $Date: 2000/02/16 15:43:14 $
// $Log: igl.h,v $
// Revision 1.7.2.3  2000/02/16 15:43:14  ttimo
// Curry bug #101996: added glTexParameterf and glNormal3f
//
// Revision 1.7.2.2  2000/02/13 17:48:07  ttimo
// Curry bug #101874 - added glTexGenf to GL interface
//
// Revision 1.7.2.1  2000/02/04 22:59:34  ttimo
// messaging API preview
//
// Revision 1.7  2000/01/18 00:43:59  ttimo
// RC
//
// Revision 1.6  2000/01/13 00:46:41  ttimo
// Merged in patches in / out
// Revision 1.1.1.4  2000/01/18 00:17:12  ttimo
// merging in for RC
//
// Revision 1.5  2000/01/11 22:14:48  ttimo
// cleaning up after merging and thinks badly fucked up
//
// Revision 1.2  2000/01/09 04:39:14  ttimo
// added some bindings to the GL table
//
// Revision 1.1.1.3  2000/01/11 16:43:15  ttimo
// merged version from old reposit and Robert update
//
// Revision 1.1.1.1  2000/01/07 17:17:30  ttimo
// initial import of Q3Radiant module
//
// Revision 1.1.1.1  2000/01/07 17:17:30  ttimo
// initial import of Q3Radiant module
//
// Revision 1.3  2000/01/17 23:53:44  TBesset
// ready for merge in sourceforge (RC candidate)
//
// Revision 1.2  2000/01/07 16:40:13  TBesset
// merged from BSP frontend
//
// Revision 1.1.1.3  1999/12/29 18:31:27  TBesset
// Q3Radiant public version
//
// Revision 1.1.1.1.2.1  1999/12/29 21:39:45  TBesset
// updated to update3 from Robert
//
// Revision 1.1.1.3  1999/12/29 18:31:27  TBesset
// Q3Radiant public version
// Revision 1.1.1.3  1999/12/29 18:31:27  TBesset
// Q3Radiant public version
//
// Revision 1.2  1999/11/22 17:46:48  Timo & Christine
// merged EARadiant into the main tree
// bug fixes for Q3Plugin / EAPlugin
// export for Robert
//
// Revision 1.1.2.2  1999/11/03 20:38:04  Timo & Christine
// MEAN plugin for Q3Radiant, alpha version
//
// Revision 1.1.2.1.2.1  1999/10/27 08:34:29  Timo & Christine
// preview version of the texture tools plugin is ready
// ( TexTool.dll plugin is in Q3Plugin module )
// plugins can draw in their own window using Radiant's qgl bindings
//
// Revision 1.1.2.1  1999/10/08 16:28:14  Timo & Christine
// started plugin extensions for EA features support in Q3Radiant
// MEAN files plugin, and Surface Properties plugin
//
//
// DESCRIPTION:
// all purpose OpenGL interface for Q3Radiant plugins
//

#ifndef __IGL_H__
#define __IGL_H__

// we use this class to let plugins draw inside the Radiant windows
class IGLWindow
{
public:
	// Increment the number of references to this object
	virtual void IncRef () = 0;
	// Decrement the reference count
	virtual void DecRef () = 0;
	virtual void Draw( VIEWTYPE vt ) = 0;
};

// define a GUID for this interface so plugins can access and reference it
// {0F237620-854B-11d3-8EF3-A2DFBD53251B}
static const GUID QERQglTable_GUID = 
{ 0xf237620, 0x854b, 0x11d3, { 0x8e, 0xf3, 0xa2, 0xdf, 0xbd, 0x53, 0x25, 0x1b } };

#include <gl/gl.h>
#include <gl/glu.h>

typedef void (APIENTRY* PFN_QGLALPHAFUNC)		(GLenum func, GLclampf ref);
typedef void (APIENTRY* PFN_glBegin)			(GLenum);
typedef void (APIENTRY* PFN_glBindTexture)		(GLenum target, GLuint texture);
typedef void (APIENTRY* PFN_glBlendFunc)       (GLenum sfactor, GLenum dfactor);
typedef void (APIENTRY* PFN_glCallList)        (GLuint list);
typedef void (APIENTRY* PFN_glCallLists)       (GLsizei n, GLenum type, const GLvoid *lists);
typedef void (APIENTRY* PFN_glClear)			(GLbitfield mask);
typedef void (APIENTRY* PFN_glClearColor)		(GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (APIENTRY* PFN_glClearDEPTH)      (GLclampd depth);
typedef void (APIENTRY* PFN_glColor3f)			(GLfloat red, GLfloat green, GLfloat blue);
typedef void (APIENTRY* PFN_glColor4f)			(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
typedef void (APIENTRY* PFN_glCullFace)        (GLenum mode);
typedef void (APIENTRY* PFN_glDeleteLists)     (GLuint list, GLsizei range);
typedef void (APIENTRY* PFN_glDisable)			(GLenum cap);
typedef void (APIENTRY* PFN_glEnable)			(GLenum cap);
typedef void (APIENTRY* PFN_glEnd)				();
typedef void (APIENTRY* PFN_glEndLIST)         (void);
typedef GLuint (APIENTRY* PFN_glGenLists)      (GLsizei range);
typedef void (APIENTRY* PFN_glListBase)        (GLuint base);
typedef void (APIENTRY* PFN_glLoadIdentity)	();
typedef void (APIENTRY* PFN_glMatrixMode)		(GLenum mode);
typedef void (APIENTRY* PFN_glNewList)         (GLuint list, GLenum mode);
typedef void (APIENTRY* PFN_QGLNORMAL3F)		(GLfloat nx, GLfloat ny, GLfloat nz);
typedef void (APIENTRY* PFN_glOrtho)			(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble zNear, GLdouble zFar);
typedef void (APIENTRY*	PFN_glPointSize)		(GLfloat size);
typedef void (APIENTRY* PFN_glPolygonMode)		(GLenum face, GLenum mode);
typedef void (APIENTRY* PFN_glPopMatrix)		();
typedef void (APIENTRY* PFN_glPushMatrix)		();
typedef void (APIENTRY* PFN_QGLROTATED)         (GLdouble angle, GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY* PFN_glRotatef)			(GLfloat angle, GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY* PFN_QGLSCALEF)			(GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY* PFN_glTexCoord2f)		(GLfloat s, GLfloat t);
typedef void (APIENTRY* PFN_glTexEnvf)			(GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRY* PFN_QGLTEXGENF)			(GLenum coord, GLenum pname, GLfloat param);
typedef void (APIENTRY* PFN_glTexParameterf)	(GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRY* PFN_QGLTRANSLATED)      (GLdouble x, GLdouble y, GLdouble z);
typedef void (APIENTRY* PFN_glTranslatef)		(GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY* PFN_glVertex2f)		(GLfloat x, GLfloat y);
typedef void (APIENTRY* PFN_glVertex3f)		(GLfloat x, GLfloat y, GLfloat z);
typedef void (APIENTRY* PFN_glViewport)		(GLint x, GLint y, GLsizei width, GLsizei height);

typedef void	(WINAPI* PFN_QE_CHECKOPENGLFORERRORS)	();
typedef int		(WINAPI* PFN_QEW_SETUPPIXELFORMAT)		(HDC hDC, qboolean zbuffer );
typedef HGLRC	(WINAPI* PFN_QWGL_CREATECONTEXT)		(HDC);
typedef BOOL	(WINAPI* PFN_QWGL_DELETECONTEXT)        (HGLRC);
typedef BOOL	(WINAPI* PFN_QWGL_MAKECURRENT)			(HDC, HGLRC);
typedef BOOL	(WINAPI* PFN_QWGL_SHARELISTS)			(HGLRC, HGLRC);
typedef BOOL	(WINAPI* PFN_QWGL_SWAPBUFFERS)			(HDC);
typedef BOOL    (WINAPI* PFN_QWGL_USEFONTBITMAPS)       (HDC, DWORD, DWORD, DWORD);

// glu stuff
typedef void (APIENTRY * PFN_QGLUPERSPECTIVE) (GLdouble fovy, GLdouble aspect, GLdouble zNear, GLdouble zFar);
typedef void (APIENTRY * PFN_QGLULOOKAT)(
  GLdouble eyex,
  GLdouble eyey,
  GLdouble eyez,
  GLdouble centerx,
  GLdouble centery,
  GLdouble centerz,
  GLdouble upx,
  GLdouble upy,
  GLdouble upz);


// specially provided by the editor for plugins
typedef HGLRC	(WINAPI* PFN_QERAPP_GETQEGLOBALSHGLRC)	();

// plugins drawing inside the GL windows
//++timo TODO: add hooking into other windows
//+timo NOTE: this could be moved to the messaging system instead of having a dedicated interface
typedef void (WINAPI* PFN_QERAPP_HOOKXYGLWINDOW) (IGLWindow *);
typedef void (WINAPI* PFN_QERAPP_UNHOOKGLWINDOW) (IGLWindow *);

struct _QERQglTable
{
	//++timo do we really wanna play with versions ?
//	float m_fVersion;
	int m_nSize;
	PFN_QGLALPHAFUNC		m_pfn_qglAlphaFunc;
	PFN_glBegin			m_pfn_glBegin;
	PFN_glBindTexture		m_pfn_glBindTexture;
	PFN_glBlendFunc		m_pfn_glBlendFunc;
	PFN_glCallList			m_pfn_glCallList;
	PFN_glClear			m_pfn_glClear;
	PFN_glClearColor		m_pfn_glClearColor;
	PFN_glCallLists		m_pfn_glCallLists;
	PFN_glClearDEPTH		m_pfn_glClearDepth;
	PFN_glColor3f			m_pfn_glColor3f;
	PFN_glColor4f			m_pfn_glColor4f;
	PFN_glCullFace			m_pfn_glCullFace;
	PFN_glDeleteLists		m_pfn_glDeleteLists;
	PFN_glDisable			m_pfn_glDisable;
	PFN_glEnable			m_pfn_glEnable;
	PFN_glEnd				m_pfn_glEnd;
	PFN_glEndLIST			m_pfn_glEndList;
	PFN_glGenLists			m_pfn_glGenLists;
	PFN_glListBase			m_pfn_glListBase;
	PFN_glLoadIdentity		m_pfn_glLoadIdentity;
	PFN_glMatrixMode		m_pfn_glMatrixMode;
	PFN_glNewList			m_pfn_glNewList;
	PFN_QGLNORMAL3F			m_pfn_qglNormal3f;
	PFN_glOrtho			m_pfn_glOrtho;
	PFN_glPointSize		m_pfn_glPointSize;
	PFN_glPolygonMode		m_pfn_glPolygonMode;
	PFN_glPopMatrix		m_pfn_glPopMatrix;
	PFN_glPushMatrix		m_pfn_glPushMatrix;
	PFN_QGLROTATED			m_pfn_qglRotated;
	PFN_glRotatef			m_pfn_glRotatef;
	PFN_QGLSCALEF			m_pfn_qglScalef;
	PFN_glTexCoord2f		m_pfn_glTexCoord2f;
	PFN_glTexEnvf			m_pfn_glTexEnvf;
	PFN_QGLTEXGENF			m_pfn_qglTexGenf;
	PFN_glTexParameterf	m_pfn_glTexParameterf;
	PFN_QGLTRANSLATED		m_pfn_qglTranslated;
	PFN_glTranslatef		m_pfn_glTranslatef;
	PFN_glVertex2f			m_pfn_glVertex2f;
	PFN_glVertex3f			m_pfn_glVertex3f;
	PFN_glViewport			m_pfn_glViewport;

	PFN_QE_CHECKOPENGLFORERRORS	m_pfn_QE_CheckOpenGLForErrors;
	PFN_QEW_SETUPPIXELFORMAT	m_pfn_QEW_SetupPixelFormat;
	PFN_QWGL_CREATECONTEXT		m_pfn_qwglCreateContext;
	PFN_QWGL_DELETECONTEXT      m_pfn_qwglDeleteContext;
	PFN_QWGL_MAKECURRENT		m_pfn_qwglMakeCurrent;
	PFN_QWGL_SHARELISTS			m_pfn_qwglShareLists;
	PFN_QWGL_SWAPBUFFERS		m_pfn_qwglSwapBuffers;
	PFN_QWGL_USEFONTBITMAPS		m_pfn_qwglUseFontBitmaps;

	PFN_QERAPP_GETQEGLOBALSHGLRC	m_pfnGetQeglobalsHGLRC;

	// glu stuff
	PFN_QGLUPERSPECTIVE			m_pfn_qgluPerspective;
	PFN_QGLULOOKAT				m_pfn_qgluLookAt;

	// plugin entities drawing inside Radiant windows
	PFN_QERAPP_HOOKXYGLWINDOW	m_pfnHookXYGLWindow;
	PFN_QERAPP_UNHOOKGLWINDOW	m_pfnUnHookGLWindow;
};

#endif