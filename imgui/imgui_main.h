// kung foo man
// 2018
// MIT LICENSE

#pragma once

#include <Windows.h>
#include "../ccall.h"

CCALL void imgui_init();
CCALL void imgui_step();
CCALL void imgui_newframe();
CCALL void imgui_endframe();
CCALL void imgui_set_hwnd(HWND hwnd);
CCALL void imgui_set_mousepos(int left, int top);
CCALL void imgui_set_widthheight(int width, int height);