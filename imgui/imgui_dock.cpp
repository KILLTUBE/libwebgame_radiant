#include "stdafx.h"
#include "imgui_dock.h"
#include <Windows.h>

const char *Dock::label() {
	return "implement Dock::label()";
}

void Dock::imgui() {
	ImGui::Button("implement Dock::imgui()");
}

void Dock::OnLeftMouseDown(ImVec2 posLeftTop) {}
void Dock::OnLeftMouseUp(ImVec2 posLeftTop) {}
void Dock::OnRightMouseDown(ImVec2 posLeftTop) {}
void Dock::OnRightMouseUp(ImVec2 posLeftTop) {}
void Dock::OnMiddleMouseDown(ImVec2 posLeftTop) {}
void Dock::OnMiddleMouseUp(ImVec2 posLeftTop) {}
void Dock::OnMouseMove(ImVec2 posLeftTop) {}
void Dock::OnKeyDown(int key) {}
void Dock::OnMouseWheelUp(ImVec2 posLeftTop) {}
void Dock::OnMouseWheelDown(ImVec2 posLeftTop) {}
LONG Dock::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) { return 0; }