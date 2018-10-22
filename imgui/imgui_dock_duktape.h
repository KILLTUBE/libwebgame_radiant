#include "imgui_dock.h"
#include <string>

class DockDuktape : public Dock {
public:
	char repl_filename[128] = {"tmp.txt"};
	char replbuffer[4096] = {0};
	DockDuktape();
	virtual const char *label();
	virtual void imgui();
};