/*
	You can use this library like this:
		#include <stdio.h>
		#include <stdlib.h>
		#include <stdarg.h>
		#include <js_stuff.h>
		int js_printf(char *msg, ...) {
			va_list argptr;
			va_start(argptr, msg);
			int ret = printf(msg, argptr);
			va_end(argptr);
			return ret;
		}
*/
#ifndef DUKTAPESTUFF_STUFF
#define DUKTAPESTUFF_STUFF


//#include "../radiant/stdafx.h"


//#include <string>

#include "../duktape-2-3-0/duktape.h"
//#include "../radiant/qe3.h"
#include <../ccall.h>

#ifdef __cplusplus
	extern "C" {
#endif

//#include "../radiant/kung/easygtkwidget.hpp"


struct funcis {
	char *name;
	int (*func)(duk_context *ctx);
};

/*
	<example>
		duk_push_global_by_name(ctx, "handle_input");
		duk_push_string(ctx, code);
		duk_push_global_object(ctx);
		duk_pcall(ctx, 2);
	</example>
*/

// js_stuff.cpp
extern duk_context *ctx;
extern int js_printf(char *msg, ...);

void js_register_method(duk_context *ctx, char *name, int (*func)(duk_context *ctx));
int js_push_global_by_name(duk_context *ctx, char *name);
int js_eval_file_safe(duk_context *ctx, char *filename);
int js_call(duk_context *ctx, char *function, char *params, ...);
int js_eval(char *msg);
int duk_func_log(duk_context *ctx);
int duk_func_file_get_contents(duk_context *ctx);
int duk_func_get_global(duk_context *ctx);
int js_register_function(duk_context *ctx, char *name, int (*func)(duk_context *ctx));
CCALL int js_init();

void duk_ref_heapptr(duk_context* ctx, void* heapptr);
void duk_unref_heapptr(duk_context* ctx, void* heapptr);

// // kung/widget.cpp
// void cb_func(GtkWidget *win, GdkEventExpose *event, gpointer data);
// void button_popup(EasyGtkWidget *button);
// void button_click_show_xywindow(EasyGtkWidget *button);
// void button_click_show_camwindow(EasyGtkWidget *button);
// void button_click_new_xywindow(EasyGtkWidget *button);
// void button_click_new_camwindow(EasyGtkWidget *button);
// void EasyGtkTabify(EasyGtkWidget *frame);


// // kung/utils.cpp
// std::string string_format(const std::string fmt, ...);
// string implode(std::string separator, std::list<string> path);


// bool loadiqm(const char *filename);
// //void displayfunc();
// //void timerfunc(int val);
// void setupcamera(int w, int h);
// void reshapefunc(int w, int h);
// void animateiqm(float curframe);
// void renderiqm();
// extern float animate;


//// duktape/adder.cpp
//int init_opengl_via_hwnd(HWND hwnd, HDC *out_hdc, HGLRC *out_hglrc);
//HWND widget_get_hwnd(GtkWidget *widget);
//int duk_eval_file_safe(char *filename);
//
//// duktape/bindings_win32.cpp
//int init_win32_bindings(duk_context *ctx);
//int send_udp_packet(char *ip, int port, char *message);
//
//// duktape/bindings_imgui.cpp
//int init_imgui_bindings(duk_context *ctx);
//
//// duktape/bindings_opengl.cpp
//int init_opengl_bindings(duk_context *ctx);
//
//// duktape/bindings_recast.cpp
//int init_recast_bindings(duk_context *ctx);
//
//// duktape/bindings_bullet.cpp
//int init_bullet_bindings(duk_context *ctx);
//
//// midireader/init_midi.cpp
//struct midi_keys {
//	int pressed;
//	double stamp;
//
//};
//// from 0 to 120, makes 121 values, 25 per row, increasing 12 per step
//extern struct midi_keys midikeys[121];


#ifdef __cplusplus
	}
#endif

#endif