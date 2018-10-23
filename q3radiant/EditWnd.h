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

#pragma once

class CTexWnd : public CWnd { public:
	DECLARE_DYNCREATE(CTexWnd);
	CTexWnd();
	void UpdateFilter(const char* pFilter);
	void UpdatePrefs();
	void FocusEdit();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual LRESULT CTexWnd::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual ~CTexWnd();
	bool m_bNeedRange;
	virtual int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnSize(UINT nType, int cx, int cy);
	virtual void OnTimer(UINT nIDEvent);
	virtual void OnPaint();
	virtual void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual void OnTexturesFlush();
	virtual void OnShaderClick();
	DECLARE_MESSAGE_MAP()
};
