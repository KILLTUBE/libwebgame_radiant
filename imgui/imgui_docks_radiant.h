#include "../ccall.h"
#include "imgui_dock.h"

CCALL int add_dock(Dock *dock);
CCALL int imgui_docks_radiant_show();
CCALL Dock *getHoveredDock(ImVec2 screenpos);