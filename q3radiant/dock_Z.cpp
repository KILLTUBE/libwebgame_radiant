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

#include "stdafx.h"
#include "Radiant.h"
#include "ZWnd.h"
#include "qe3.h"

#define	PAGEFLIPS	2
#define CAM_HEIGHT		48 // height of main part
#define CAM_GIZMO		8	// height of the gizmo

z_t		z;
static	int	cursorx, cursory;

void Z_Init (void) {
	z.origin[0] = 0;
	z.origin[1] = 20;
	z.origin[2] = 46;
	z.scale = 1;
}

void Z_MouseDown(int x, int y, int buttons) {
	vec3_t	org, dir, vup, vright;
	brush_t	*b;
	Sys_GetCursorPos (&cursorx, &cursory);
	vup[0] = 0; vup[1] = 0; vup[2] = 1/z.scale;
	VectorCopy (z.origin, org);
	org[2] += (y - (z.height/2))/z.scale;
	org[1] = -8192;
	b = selected_brushes.next;
	if (b != &selected_brushes) {
		org[0] = (b->mins[0] + b->maxs[0])/2;
	}
	dir[0] = 0; dir[1] = 1; dir[2] = 0;
	vright[0] = 0; vright[1] = 0; vright[2] = 0;
	// LBUTTON = manipulate selection
	// shift-LBUTTON = select
	// middle button = grab texture
	// ctrl-middle button = set entire brush to texture
	// ctrl-shift-middle button = set single face to texture

	int nMouseButton = g_PrefsDlg.m_nMouseButtons == 2 ? MK_RBUTTON : MK_MBUTTON;
	if ( (buttons == MK_LBUTTON)
		|| (buttons == (MK_LBUTTON | MK_SHIFT))
		|| (buttons == MK_MBUTTON)
//		|| (buttons == (MK_MBUTTON|MK_CONTROL))
		|| (buttons == (nMouseButton|MK_SHIFT|MK_CONTROL)) )
	{
		Drag_Begin (x, y, buttons,
			vright, vup,
			org, dir);
		return;
	}

	// control mbutton = move camera
	if ((buttons == (MK_CONTROL|nMouseButton) ) || (buttons == (MK_CONTROL|MK_LBUTTON)))
	{	
		g_pParentWnd->GetCamera()->Camera().origin[2] = org[2] ;
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY|W_Z);
	}
}

void Z_MouseUp (int x, int y, int buttons) {
	Drag_MouseUp ();
}

void Z_MouseMoved (int x, int y, int buttons) {
	if (!buttons)
		return;
	if (buttons == MK_LBUTTON)
	{
		Drag_MouseMoved (x, y, buttons);
		Sys_UpdateWindows (W_Z|W_CAMERA_IFON|W_XY);
		return;
	}
	// rbutton = drag z origin
	if (buttons == MK_RBUTTON)
	{
		Sys_GetCursorPos (&x, &y);
		if ( y != cursory)
		{
			z.origin[2] += y-cursory;
			Sys_SetCursorPos (cursorx, cursory);
			Sys_UpdateWindows (W_Z);
		}
		return;
	}
	// control mbutton = move camera
	int nMouseButton = g_PrefsDlg.m_nMouseButtons == 2 ? MK_RBUTTON : MK_MBUTTON;
	if ((buttons == (MK_CONTROL|nMouseButton) ) || (buttons == (MK_CONTROL|MK_LBUTTON))) {	
		g_pParentWnd->GetCamera()->Camera().origin[2] = (y - (z.height/2))/z.scale;
		Sys_UpdateWindows (W_CAMERA|W_XY_OVERLAY|W_Z);
	}
}

void Z_DrawGrid () {
	float	zz, zb, ze;
	char	text[32];
	int w = z.width/2 / z.scale;
	int h = z.height/2 / z.scale;

	zb = z.origin[2] - h;
	if (zb < region_mins[2])
		zb = region_mins[2];
	zb = 64 * floor (zb/64);

	ze = z.origin[2] + h;
	if (ze > region_maxs[2])
		ze = region_maxs[2];
	ze = 64 * ceil (ze/64);

	// draw major blocks
	qglColor3fv(g_qeglobals.d_savedinfo.colors[COLOR_GRIDMAJOR]);
	qglBegin (GL_LINES);
	qglVertex2f(0, zb);
	qglVertex2f(0, ze);
	for (zz=zb ; zz<ze ; zz+=64) {
		qglVertex2f (-w, zz);
		qglVertex2f (w, zz);
	}
	qglEnd ();
	// draw minor blocks
	if (g_qeglobals.d_showgrid && g_qeglobals.d_gridsize*z.scale >= 4 &&
			g_qeglobals.d_savedinfo.colors[COLOR_GRIDMINOR] != g_qeglobals.d_savedinfo.colors[COLOR_GRIDBACK]) {
		qglColor3fv(g_qeglobals.d_savedinfo.colors[COLOR_GRIDMINOR]);
		qglBegin (GL_LINES);
		for (zz=zb ; zz<ze ; zz+=g_qeglobals.d_gridsize)
		{
			if ( ! ((int)zz & 63) )
				continue;
			qglVertex2f (-w, zz);
			qglVertex2f (w, zz);
		}
		qglEnd ();
	}
	// draw coordinate text if needed
	qglColor3fv(g_qeglobals.d_savedinfo.colors[COLOR_GRIDTEXT]);
	for (zz=zb ; zz<ze ; zz+=64) {
		qglRasterPos2f (-w+1, zz);
		sprintf (text, "%i",(int)zz);
		qglCallLists (strlen(text), GL_UNSIGNED_BYTE, text);
	}
}

void ZDrawCameraIcon() {
	float	x, y;
	int	xCam = z.width/4;
	x = 0;
	y = g_pParentWnd->GetCamera()->Camera().origin[2];
	qglColor3f (0.0, 0.0, 1.0);
	qglBegin(GL_LINE_STRIP);
	qglVertex3f(x-xCam,y,0);
	qglVertex3f(x,y+CAM_GIZMO,0);
	qglVertex3f(x+xCam,y,0);
	qglVertex3f(x,y-CAM_GIZMO,0);
	qglVertex3f(x-xCam,y,0);
	qglVertex3f(x+xCam,y,0);
	qglVertex3f(x+xCam,y-CAM_HEIGHT,0);
	qglVertex3f(x-xCam,y-CAM_HEIGHT,0);
	qglVertex3f(x-xCam,y,0);
	qglEnd();
}

void Z_Draw() {
	brush_t	*brush;
	float	w, h;
	double	start, end;
	qtexture_t	*q;
	float	top, bottom;
	vec3_t	org_top, org_bottom, dir_up, dir_down;
	int xCam = z.width / 3;
	if (!active_brushes.next)
		return;	// not valid yet
	qglViewport(0, 0, z.width, z.height);
	qglClearColor(
		g_qeglobals.d_savedinfo.colors[COLOR_GRIDBACK][0],
		g_qeglobals.d_savedinfo.colors[COLOR_GRIDBACK][1],
		g_qeglobals.d_savedinfo.colors[COLOR_GRIDBACK][2], 0);
	qglClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
	qglMatrixMode(GL_PROJECTION);
	qglLoadIdentity ();
	w = z.width/2 / z.scale;
	h = z.height/2 / z.scale;
	qglOrtho(-w, w, z.origin[2]-h, z.origin[2]+h, -8, 8);
	qglDisable(GL_TEXTURE_2D);
	qglDisable(GL_TEXTURE_1D);
	qglDisable(GL_DEPTH_TEST);
	qglDisable(GL_BLEND);
	Z_DrawGrid ();
	qglDisable(GL_CULL_FACE);
	qglShadeModel (GL_FLAT);
	qglPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	qglDisable(GL_TEXTURE_2D);
	qglDisable(GL_BLEND);
	qglDisable(GL_DEPTH_TEST);
	// draw filled interiors and edges
	dir_up[0] = 0 ; dir_up[1] = 0; dir_up[2] = 1;
	dir_down[0] = 0 ; dir_down[1] = 0; dir_down[2] = -1;
	VectorCopy (z.origin, org_top);
	org_top[2] = 4096;
	VectorCopy (z.origin, org_bottom);
	org_bottom[2] = -4096;
	for (brush = active_brushes.next ; brush != &active_brushes ; brush=brush->next) {
		if (brush->mins[0] >= z.origin[0]
			|| brush->maxs[0] <= z.origin[0]
			|| brush->mins[1] >= z.origin[1]
			|| brush->maxs[1] <= z.origin[1])
			continue;
		if (!Brush_Ray (org_top, dir_down, brush, &top))
			continue;
		top = org_top[2] - top;
		if (!Brush_Ray (org_bottom, dir_up, brush, &bottom))
			continue;
		bottom = org_bottom[2] + bottom;
		q = Texture_ForName (brush->brush_faces->texdef.name);
		qglColor3f (q->color[0], q->color[1], q->color[2]);
		qglBegin (GL_QUADS);
		qglVertex2f (-xCam, bottom);
		qglVertex2f (xCam, bottom);
		qglVertex2f (xCam, top);
		qglVertex2f (-xCam, top);
		qglEnd();
		qglColor3f (1,1,1);
		qglBegin (GL_LINE_LOOP);
		qglVertex2f (-xCam, bottom);
		qglVertex2f (xCam, bottom);
		qglVertex2f (xCam, top);
		qglVertex2f (-xCam, top);
		qglEnd ();
	}
	// now draw selected brushes
	for (brush = selected_brushes.next ; brush != &selected_brushes ; brush=brush->next) {
		if ( !(brush->mins[0] >= z.origin[0]
			|| brush->maxs[0] <= z.origin[0]
			|| brush->mins[1] >= z.origin[1]
			|| brush->maxs[1] <= z.origin[1]) ) {
			if (Brush_Ray(org_top, dir_down, brush, &top)) {
				top = org_top[2] - top;
				if (Brush_Ray (org_bottom, dir_up, brush, &bottom)) {
					bottom = org_bottom[2] + bottom;
					q = Texture_ForName (brush->brush_faces->texdef.name);
					qglColor3f (q->color[0], q->color[1], q->color[2]);
					qglBegin (GL_QUADS);
					qglVertex2f (-xCam, bottom);
					qglVertex2f (xCam, bottom);
					qglVertex2f (xCam, top);
					qglVertex2f (-xCam, top);
					qglEnd ();
				}
			}
		}
		qglColor3fv(g_qeglobals.d_savedinfo.colors[COLOR_SELBRUSHES]);
		qglBegin (GL_LINE_LOOP);
		qglVertex2f (-xCam, brush->mins[2]);
		qglVertex2f (xCam, brush->mins[2]);
		qglVertex2f (xCam, brush->maxs[2]);
		qglVertex2f (-xCam, brush->maxs[2]);
		qglEnd ();
	}
	ZDrawCameraIcon();
	qglFinish();
	QE_CheckOpenGLForErrors();
	if (z.timing) {
		end = Sys_DoubleTime ();
		Sys_Printf ("z: %i ms\n", (int)(1000*(end-start)));
	}
}


IMPLEMENT_DYNCREATE(CZWnd, CWnd);

CZWnd::CZWnd() {
}

CZWnd::~CZWnd() {
}

BEGIN_MESSAGE_MAP(CZWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_GETMINMAXINFO()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_WM_NCCALCSIZE()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CLOSE()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONUP()
	ON_WM_KEYUP()
END_MESSAGE_MAP()

int CZWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) {
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	g_qeglobals.d_hwndZ = GetSafeHwnd();
	m_dcZ = ::GetDC(GetSafeHwnd());
	QEW_SetupPixelFormat(m_dcZ, false);
	if ((m_hglrcZ = qwglCreateContext(m_dcZ )) == 0)
		Error("wglCreateContext in CZWnd::OnCreate failed");
	if (!qwglShareLists(g_qeglobals.d_hglrcBase, m_hglrcZ))
		Error( "wglShareLists in CZWnd::OnCreate failed");
	if (!qwglMakeCurrent(m_dcZ, m_hglrcZ))
		Error ("wglMakeCurrent in CZWnd::OnCreate failed");
	return 0;
}

void CZWnd::OnDestroy() 
{
	QEW_StopGL(GetSafeHwnd(), m_hglrcZ, m_dcZ);
	CWnd::OnDestroy();
}

void CZWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) {
	g_pParentWnd->HandleKey(nChar, nRepCnt, nFlags);
}

void CZWnd::OnLButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	SetCapture();
	CRect rctZ;
	GetClientRect(rctZ);
	Z_MouseDown (point.x, rctZ.Height() - 1 - point.y , nFlags);
}

void CZWnd::OnMButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	SetCapture();
	CRect rctZ;
	GetClientRect(rctZ);
	Z_MouseDown (point.x, rctZ.Height() - 1 - point.y , nFlags);
}

void CZWnd::OnRButtonDown(UINT nFlags, CPoint point) {
	SetFocus();
	SetCapture();
	CRect rctZ;
	GetClientRect(rctZ);
	Z_MouseDown (point.x, rctZ.Height() - 1 - point.y , nFlags);
}

void CZWnd::OnPaint() {
	CPaintDC dc(this); // device context for painting
	//if (!wglMakeCurrent(m_dcZ, m_hglrcZ))
	if (!qwglMakeCurrent(dc.m_hDC, m_hglrcZ)) {
		Sys_Printf("ERROR: wglMakeCurrent failed..\n ");
		Sys_Printf("Please restart Q3Radiant if the Z view is not working\n");
	} else {
		QE_CheckOpenGLForErrors();
		Z_Draw();
		//qwglSwapBuffers(m_dcZ);
		qwglSwapBuffers(dc.m_hDC);
		TRACE("Z Paint\n");
	}
}

void CZWnd::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) {
	lpMMI->ptMinTrackSize.x = ZWIN_WIDTH;
}

void CZWnd::OnMouseMove(UINT nFlags, CPoint point) {
	CRect rctZ;
	GetClientRect(rctZ);
	float fz = z.origin[2] + ((rctZ.Height() - 1 - point.y) - (z.height/2)) / z.scale;
	fz = floor(fz / g_qeglobals.d_gridsize + 0.5) * g_qeglobals.d_gridsize;
	CString strStatus;
	strStatus.Format("Z:: %.1f", fz);
	g_pParentWnd->SetStatusText(1, strStatus);
	Z_MouseMoved (point.x, rctZ.Height() - 1 - point.y, nFlags);
}

void CZWnd::OnSize(UINT nType, int cx, int cy) {
	CWnd::OnSize(nType, cx, cy);
	CRect rctZ;
	GetClientRect(rctZ);
	z.width = rctZ.right;
	z.height = rctZ.bottom;
	if (z.width < 10)
		z.width = 10;
	if (z.height < 10)
		z.height = 10;
	Invalidate();
}

void CZWnd::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) {
	CWnd::OnNcCalcSize(bCalcValidRects, lpncsp);
}

void CZWnd::OnKillFocus(CWnd* pNewWnd) {
	CWnd::OnKillFocus(pNewWnd);
	SendMessage(WM_NCACTIVATE, FALSE , 0 );
}

void CZWnd::OnSetFocus(CWnd* pOldWnd) {
	CWnd::OnSetFocus(pOldWnd);
	SendMessage(WM_NCACTIVATE, TRUE , 0 );
}

void CZWnd::OnClose() {
	CWnd::OnClose();
}

void CZWnd::OnLButtonUp(UINT nFlags, CPoint point) {
	CRect rctZ;
	GetClientRect(rctZ);
	Z_MouseUp (point.x, rctZ.bottom - 1 - point.y, nFlags);
	if (! (nFlags & (MK_LBUTTON|MK_RBUTTON|MK_MBUTTON)))
		ReleaseCapture ();
}

void CZWnd::OnMButtonUp(UINT nFlags, CPoint point) {
	CRect rctZ;
	GetClientRect(rctZ);
	Z_MouseUp (point.x, rctZ.bottom - 1 - point.y, nFlags);
	if (! (nFlags & (MK_LBUTTON|MK_RBUTTON|MK_MBUTTON)))
		ReleaseCapture ();
}

void CZWnd::OnRButtonUp(UINT nFlags, CPoint point) {
	CRect rctZ;
	GetClientRect(rctZ);
	Z_MouseUp (point.x, rctZ.bottom - 1 - point.y, nFlags);
	if (! (nFlags & (MK_LBUTTON|MK_RBUTTON|MK_MBUTTON)))
		ReleaseCapture ();
}

BOOL CZWnd::PreCreateWindow(CREATESTRUCT& cs) {
	WNDCLASS wc;
	HINSTANCE hInstance = AfxGetInstanceHandle();
	if (::GetClassInfo(hInstance, Z_WINDOW_CLASS, &wc) == FALSE) {
		// Register a new class
		memset (&wc, 0, sizeof(wc));
		wc.style				 = CS_NOCLOSE | CS_OWNDC;
		wc.lpszClassName = Z_WINDOW_CLASS;
		wc.hCursor			 = LoadCursor (NULL,IDC_ARROW);
		wc.lpfnWndProc = ::DefWindowProc;
		if (AfxRegisterClass(&wc) == FALSE)
			Error ("CZWnd RegisterClass: failed");
	}
	cs.lpszClass = Z_WINDOW_CLASS;
	cs.lpszName = "Z";
	if (cs.style != QE3_CHILDSTYLE)
		cs.style = QE3_SPLITTER_STYLE;
	return CWnd::PreCreateWindow(cs);
}

void CZWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) {
	g_pParentWnd->HandleKey(nChar, nRepCnt, nFlags, false);
}