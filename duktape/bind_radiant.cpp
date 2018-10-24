#include "stdafx.h"

#include "duktapestuff.h"
#include "../duktape-2-3-0/duktape.h"	
//#include <kung/duktape/dukdebugheaders.h>
#include "../q3radiant/QERTYPES.H"
#include "../q3radiant/CamWnd.h"

extern camera_t *camera;

int duk_func_radiant_camera_origin_set(duk_context *ctx) {
	float x = (float)duk_to_number(ctx, 0);
	float y = (float)duk_to_number(ctx, 1);
	float z = (float)duk_to_number(ctx, 2);
	camera->origin[0] = x;
	camera->origin[1] = y;
	camera->origin[2] = z;
	return 0;
}

int duk_func_radiant_camera_origin_get(duk_context *ctx) {
	duk_push_array(ctx);
	duk_push_number(ctx, camera->origin[0]);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_number(ctx, camera->origin[1]);
	duk_put_prop_string(ctx, -2, "y");
	duk_push_number(ctx, camera->origin[2]);
	duk_put_prop_string(ctx, -2, "z");
	return 1;
}

int duk_func_radiant_camera_forward_get(duk_context *ctx) {
	duk_push_array(ctx);
	duk_push_number(ctx, camera->forward[0]);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_number(ctx, camera->forward[1]);
	duk_put_prop_string(ctx, -2, "y");
	duk_push_number(ctx, camera->forward[2]);
	duk_put_prop_string(ctx, -2, "z");
	return 1;
}

int duk_func_Sys_UpdateWindows(duk_context *ctx) {
	Sys_UpdateWindows(W_ALL);
	return 0;
}

int duk_func_radiant_mouse_position_set(duk_context *ctx) {
	int x = (int)duk_to_number(ctx, 0);
	int y = (int)duk_to_number(ctx, 1);
	SetCursorPos(x, y);
	return 0;
}

int duk_func_radiant_mouse_position_get(duk_context *ctx) {
	POINT xy;
	GetCursorPos(&xy);
	duk_push_array(ctx);
	duk_push_number(ctx, xy.x);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_number(ctx, xy.y);
	duk_put_prop_string(ctx, -2, "y");
	return 1;
}

void duktape_bind_radiant(duk_context *ctx) {
	struct funcis funcs[] = {
		{"radiant_camera_origin_set"           , duk_func_radiant_camera_origin_set   },
		{"radiant_camera_origin_get"           , duk_func_radiant_camera_origin_get   },
		{"radiant_camera_forward_get"          , duk_func_radiant_camera_forward_get  },
		{"Sys_UpdateWindows"                   , duk_func_Sys_UpdateWindows           },
		{"radiant_mouse_position_set"          , duk_func_radiant_mouse_position_set  },
		{"radiant_mouse_position_get"          , duk_func_radiant_mouse_position_get  },
		{NULL, NULL}
	};
	for (int i=0; funcs[i].name; i++) {
		js_register_function(ctx, funcs[i].name, funcs[i].func);
	}

#if 0
	struct funcis methods[] = {
		{"DragFloat"                       , duk_func_imgui_drag_float                        },
		{"Button"                          , duk_func_imgui_button                            },
		{NULL, NULL}
	};

	duk_push_global_object(ctx); // [..., global]
	duk_push_object(ctx); // [..., global, object]

	for (int i=0; methods[i].name; i++) {
		js_register_method(ctx, methods[i].name, methods[i].func); // push_c_func, put_prop_string...
	}

	//duk_put_function_list(ctx, -1, eventloop_funcs);
	// [..., global]
	// global[GUI] = object
	duk_put_prop_string(ctx, -2, "GUI");
	// [...]
	duk_pop(ctx);
#endif
}
