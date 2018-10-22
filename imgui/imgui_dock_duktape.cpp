#include "stdafx.h"
#include "imgui_dock_duktape.h"
#include "imgui.h"
#include "imgui_dock_console.h"
#include "../duktape-2-3-0/duktape.h"
#include <string>

bool IsKeyPressedMap(ImGuiKey key, bool repeat = true);
void SaveIniSettingsToDisk(const char* ini_filename); // was a static function in ImGui
void ImStrncpy(char* dst, const char* src, int count);

DockDuktape::DockDuktape() {
}

const char *DockDuktape::label() {
	return "Duktape";
}

static int repl_callback(ImGuiTextEditCallbackData *data) {
	DockDuktape *dock = (DockDuktape *)data->UserData;
	if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(' ', false)) {
		char returnbuffer[256] = {0};
		//js_call(ctx, "get_auto_completion", "siiii", dock->replbuffer, data->CursorPos, data->SelectionStart, data->SelectionEnd, returnbuffer);
		imgui_log("got returnbuffer: %s\n", returnbuffer);
		data->InsertChars(data->CursorPos, returnbuffer);
	}
	//log("callback called\n");
	return 0;
}

void DockDuktape::imgui() {
	//ImGui::InputTextMultiline("", replbuffer, sizeof replbuffer, ImGui::GetWindowSize() + ImVec2(-15, -35 - 20), ImGuiInputTextFlags_CallbackAlways | ImGuiInputTextFlags_AllowTabInput, repl_callback, (void *)this);

	if (ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed('r', false)) {
		//js_call(ctx, "reload", "");
	}
	
#if 0
	if (ImGui::IsItemActive()) {
		//log("active\n");
		/*
		comment this shit in imgui.c to make this work:
		bool ctrl_enter_for_new_line = (flags & ImGuiInputTextFlags_CtrlEnterForNewLine) != 0;
        if (!is_multiline || (ctrl_enter_for_new_line && !io.KeyCtrl) || (!ctrl_enter_for_new_line && io.KeyCtrl))
        {
            //SetActiveID(0);
            //enter_pressed = true;
        }
		*/

		if (ImGui::GetIO().KeyCtrl && IsKeyPressedMap(ImGuiKey_Enter, 0)) {
			ImGuiContext *g = ImGui::GetCurrentContext();
			int select_start = g->InputTextState.StbState.select_start;
			int select_end = g->InputTextState.StbState.select_end;
			//js_call(ctx, "shittyconsole", "sii", replbuffer, select_start, select_end);
		}

	}
#endif

	//ImGui::InputText("Filename", repl_filename, sizeof repl_filename);
	//ImGui::SameLine(0, 5);
	//if (ImGui::Button("Save")) {
	//	if (file_put_contents(repl_filename, replbuffer, strlen(replbuffer))) {
	//		imgui_log("[success] REPL content saved to %s\n", repl_filename);
	//	} else {
	//		imgui_log("[fail] couldnt save %s\n", repl_filename);
	//	}
	//	//js_call(ctx, "file_put_contents", "ss", repl_filename, replbuffer);
	//}
	//ImGui::SameLine(0, 5);
	//if (ImGui::Button("Load")) {
	//	auto meh = &replbuffer;
	//	char *buffer;
	//	size_t buffer_length;
	//	if (file_get_contents(repl_filename, &buffer, &buffer_length)) {
	//		imgui_log("[success] REPL content loaded from %s\n", repl_filename);
	//		strncpy(replbuffer, buffer, buffer_length);
	//		free(buffer);
	//	} else {
	//		imgui_log("[fail] couldnt load %s\n", repl_filename);
	//	}
	//	//js_eval((char *)std::string("repl_set_text(file_get_contents(\""+std::string(repl_filename)+"\"))").c_str());
	//}
}