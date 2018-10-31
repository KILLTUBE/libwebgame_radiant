#include "stdafx.h"
#include "duktapestuff.h"
#include "../duktape-2-3-0/duktape.h"
//#include <kung/duktape/dukdebugheaders.h>
#include "../q3radiant/QERTYPES.H"
#include "../q3radiant/CamWnd.h"
#include "../q3radiant/qe3.h"
#include "../craft/src/glfw/include/GLFW/glfw3.h"
#include "../craft/src/tinycthread/tinycthread.h"

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

int duk_func_radiant_camera_angles_set(duk_context *ctx) {
	float x = (float)duk_to_number(ctx, 0);
	float y = (float)duk_to_number(ctx, 1);
	float z = (float)duk_to_number(ctx, 2);
	camera->angles[0] = x;
	camera->angles[1] = y;
	camera->angles[2] = z;
	return 0;
}

int duk_func_radiant_camera_angles_get(duk_context *ctx) {
	duk_push_array(ctx);
	duk_push_number(ctx, camera->angles[0]);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_number(ctx, camera->angles[1]);
	duk_put_prop_string(ctx, -2, "y");
	duk_push_number(ctx, camera->angles[2]);
	duk_put_prop_string(ctx, -2, "z");
	return 1;
}

int duk_func_radiant_camera_forward_get(duk_context *ctx) {
	//vec3_t forward;
	//AngleVectors(camera->angles, forward, NULL, NULL);
	//forward[2] *= 1.0f; // fix "bug" or whatever
	duk_push_array(ctx);
	duk_push_number(ctx, camera->vpn[0]);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_number(ctx, camera->vpn[1]);
	duk_put_prop_string(ctx, -2, "y");
	duk_push_number(ctx, camera->vpn[2]);
	duk_put_prop_string(ctx, -2, "z");
	return 1;
}
int duk_func_radiant_camera_backward_get(duk_context *ctx) {
	//vec3_t forward;
	//AngleVectors(camera->angles, forward, NULL, NULL);
	//forward[2] *= 1.0f; // fix "bug" or whatever
	duk_push_array(ctx);
	duk_push_number(ctx, camera->vpn[0] * -1.0f);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_number(ctx, camera->vpn[1] * -1.0f);
	duk_put_prop_string(ctx, -2, "y");
	duk_push_number(ctx, camera->vpn[2] * -1.0f);
	duk_put_prop_string(ctx, -2, "z");
	return 1;
}

int duk_func_radiant_camera_right_get(duk_context *ctx) {
	duk_push_array(ctx);
	duk_push_number(ctx, camera->right[0]);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_number(ctx, camera->right[1]);
	duk_put_prop_string(ctx, -2, "y");
	duk_push_number(ctx, camera->right[2]);
	duk_put_prop_string(ctx, -2, "z");
	return 1;
}
int duk_func_radiant_camera_left_get(duk_context *ctx) {
	duk_push_array(ctx);
	duk_push_number(ctx, camera->right[0] * -1.0f);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_number(ctx, camera->right[1] * -1.0f);
	duk_put_prop_string(ctx, -2, "y");
	duk_push_number(ctx, camera->right[2] * -1.0f);
	duk_put_prop_string(ctx, -2, "z");
	return 1;
}

int duk_func_radiant_camera_up_get(duk_context *ctx) {
	// fucking AngleVectors is bugged up shit
	//vec3_t up;
	//AngleVectors(camera->angles, NULL, NULL, up);
	//up[0] *= 1.0f; // fix "bug" or whatever
	//up[1] *= 1.0f; // fix "bug" or whatever
	duk_push_array(ctx);
	duk_push_number(ctx, camera->vup[0]);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_number(ctx, camera->vup[1]);
	duk_put_prop_string(ctx, -2, "y");
	duk_push_number(ctx, camera->vup[2]);
	duk_put_prop_string(ctx, -2, "z");
	return 1;
}
int duk_func_radiant_camera_down_get(duk_context *ctx) {
	// fucking AngleVectors is bugged up shit
	//vec3_t up;
	//AngleVectors(camera->angles, NULL, NULL, up);
	//up[0] *= 1.0f; // fix "bug" or whatever
	//up[1] *= 1.0f; // fix "bug" or whatever
	duk_push_array(ctx);
	duk_push_number(ctx, camera->vup[0] * -1.0f);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_number(ctx, camera->vup[1] * -1.0f);
	duk_put_prop_string(ctx, -2, "y");
	duk_push_number(ctx, camera->vup[2] * -1.0f);
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


int duk_func_radiant_mouse_show(duk_context *ctx) {
	ShowCursor(true);
	return 0;
}

int duk_func_radiant_mouse_hide(duk_context *ctx) {
	while(ShowCursor(false) >= 0);
	return 0;
}

int duk_func_leftMousePressed(duk_context *ctx) {
	auto ret = GetAsyncKeyState(VK_LBUTTON) & 0x8000;
	duk_push_boolean(ctx, ret);
	return 1;
}

int duk_func_middleMousePressed(duk_context *ctx) {
	auto ret = GetAsyncKeyState(VK_MBUTTON) & 0x8000;
	duk_push_boolean(ctx, ret);
	return 1;
}

int duk_func_rightMousePressed(duk_context *ctx) {
	auto ret = GetAsyncKeyState(VK_RBUTTON) & 0x8000;
	duk_push_boolean(ctx, ret);
	return 1;
}

int duk_func_shiftButtonPressed(duk_context *ctx) {
	auto ret = GetAsyncKeyState(VK_SHIFT) & 0x8000;
	duk_push_boolean(ctx, ret);
	return 1;
}

int duk_func_controlButtonPressed(duk_context *ctx) {
	auto ret = GetAsyncKeyState(VK_CONTROL) & 0x8000;
	duk_push_boolean(ctx, ret);
	return 1;
}

int duk_func_GetAsyncKeyState(duk_context *ctx) {
	int button = duk_to_int(ctx, 0);
	auto ret = GetAsyncKeyState(button) & 0x8000;
	duk_push_boolean(ctx, ret);
	return 1;
}

void LoadShadersFromDir(const char *pPath);

int duk_func_LoadShadersFromDir(duk_context *ctx) {
	const char *path = duk_to_string(ctx, 0);
	LoadShadersFromDir(path);
	return 0;
}

GLFWwindow *glfw_windows[8] = {NULL};
int glfw_next_id = 0;

void on_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
}

void on_char(GLFWwindow *window, unsigned int u) {
}

void on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
}

void on_scroll(GLFWwindow *window, double xdelta, double ydelta) {
}

typedef struct
{
    GLFWwindow* window;
    const char* title;
    float r, g, b;
    thrd_t id;
} Thread;


int thread_main(void* data) {

	
	GLFWmonitor *monitor = NULL;
	GLFWwindow *window = NULL;
	window = glfwCreateWindow(640, 480, "document.title", monitor, NULL);
	//glfw_windows[0] = window;
	int width = 640;
	int height = 480;


    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, on_key);
    glfwSetCharCallback(window, on_char);
    glfwSetMouseButtonCallback(window, on_mouse_button);
    glfwSetScrollCallback(window, on_scroll);

	while (true) {

		
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		if (glfwWindowShouldClose(window)) {
			break;
		}

		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	return 0;

}

// glfw_create_window()
int duk_glfw_create_window(duk_context *ctx) {
	int window_id = glfw_next_id++;
	
	static int first = 1;
	if (first) {
		first = 0;
		glfwInit();
	}
	Thread threads[] =
    {
        { NULL, "Red", 1.f, 0.f, 0.f, 0 },
        { NULL, "Green", 0.f, 1.f, 0.f, 0 },
        { NULL, "Blue", 0.f, 0.f, 1.f, 0 }
    };
	int i=0;
	if (thrd_create(&threads[i].id, thread_main, threads + i) != thrd_success) {

	}


	duk_push_int(ctx, i);
	return 1;
}

void duktape_bind_radiant(duk_context *ctx) {
	struct funcis funcs[] = {
		{"radiant_camera_origin_set"           , duk_func_radiant_camera_origin_set   },
		{"radiant_camera_origin_get"           , duk_func_radiant_camera_origin_get   },
		{"radiant_camera_angles_set"           , duk_func_radiant_camera_angles_set   },
		{"radiant_camera_angles_get"           , duk_func_radiant_camera_angles_get   },
		{"radiant_camera_forward_get"          , duk_func_radiant_camera_forward_get  },
		{"radiant_camera_backward_get"         , duk_func_radiant_camera_backward_get },
		{"radiant_camera_right_get"            , duk_func_radiant_camera_right_get    },
		{"radiant_camera_left_get"             , duk_func_radiant_camera_left_get     },
		{"radiant_camera_up_get"               , duk_func_radiant_camera_up_get       },
		{"radiant_camera_down_get"             , duk_func_radiant_camera_down_get     },
		{"Sys_UpdateWindows"                   , duk_func_Sys_UpdateWindows           },
		{"radiant_mouse_position_set"          , duk_func_radiant_mouse_position_set  },
		{"radiant_mouse_position_get"          , duk_func_radiant_mouse_position_get  },
		{"radiant_mouse_show"                  , duk_func_radiant_mouse_show          },
		{"radiant_mouse_hide"                  , duk_func_radiant_mouse_hide          },
		{"leftMousePressed"                    , duk_func_leftMousePressed            },
		{"middleMousePressed"                  , duk_func_middleMousePressed          },
		{"rightMousePressed"                   , duk_func_rightMousePressed           },
		{"shiftButtonPressed"                  , duk_func_shiftButtonPressed          },
		{"controlButtonPressed"                , duk_func_controlButtonPressed        },
		{"GetAsyncKeyState"                    , duk_func_GetAsyncKeyState            },
		{"LoadShadersFromDir"                  , duk_func_LoadShadersFromDir          },
		// glfw
		{"glfw_create_window"                  , duk_glfw_create_window               },
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
