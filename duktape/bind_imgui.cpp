#include "stdafx.h"

#include "duktapestuff.h"
#include "../duktape-2-3-0/duktape.h"	
//#include <kung/duktape/dukdebugheaders.h>
#include "../imgui/imgui.h"

// todo: , int *out_size);
float *duk_get_float_array(duk_context *ctx, int idx) {
#if 0
	struct duk_hbufferobject *buf = (struct duk_hbufferobject *) duk_to_pointer(ctx, idx);
	//duk_size_t size;
	//buf = duk_to_buffer_kung(ctx, 0, &size, DUK_BUF_MODE_DONTCARE);
	//memcpy(javascript_viewmatrix, (void *)( ((int)buf->buf) + 20 ), 16 * 4);

	float *arr = NULL;
	if (DUK_HBUFFER_HAS_EXTERNAL(buf->buf)) {
		struct duk_hbuffer_external *ext = (struct duk_hbuffer_external *) buf->buf;
		arr = (float *) ext->curr_alloc;
	} else {
		arr = (float *)( ((int)buf->buf) + 20  );
	}

	/*
	DUK_INTERNAL void duk_hbufferobject_push_validated_read(duk_context *ctx, duk_hbufferobject *h_bufobj, duk_uint8_t *p, duk_small_uint_t elem_size) {
		duk_double_union du;

		DUK_MEMCPY((void *) du.uc, (const void *) p, (size_t) elem_size);

		switch (h_bufobj->elem_type) {
		case DUK_HBUFFEROBJECT_ELEM_UINT8:
	*/

	/*
	 *  Flags
	 *
	 *  Fixed buffer:     0
	 *  Dynamic buffer:   DUK_HBUFFER_FLAG_DYNAMIC
	 *  External buffer:  DUK_HBUFFER_FLAG_DYNAMIC | DUK_HBUFFER_FLAG_EXTERNAL
	 */
		// a = new Float32Array([12323]);
		// a = op.in_a.GetVector4()
	#if 0
		ImGui::Text("Size: %d elemtype %d dyn %d ext %d",
			buf->buf->size,
			buf->elem_type,

			DUK_HBUFFER_HAS_DYNAMIC(buf->buf),
			DUK_HBUFFER_HAS_EXTERNAL(buf->buf)

			//DUK_HBUFFER_FLAG_DYNAMIC,
			//DUK_HBUFFER_FLAG_EXTERNAL,
			//DUK_HBUFFER_FLAG_DYNAMIC | DUK_HBUFFER_FLAG_EXTERNAL
		);
	#endif
	return arr;
#else
	return NULL;
#endif
}

int duk_func_imgui_drag_float(duk_context *ctx) {
	char *label = (char *) duk_to_string(ctx, 0);
	float *arr = duk_get_float_array(ctx, 1);
	//float v_speed = duk_to_number(ctx, 2);
	//float v_min = duk_to_number(ctx, 3);
	//float v_max = duk_to_number(ctx, 4);
	//char *display_format = (char *) duk_to_string(ctx, 5);
	//float power = duk_to_number(ctx, 6);
	//int ret = ImGui::DragFloat(label, v, v_speed, v_min, v_max, display_format, power);

	//float arr[1] = {123};

	int ret;
	//if ((int)arr < 10000)
	if (0)
	{
		//char bufasd[256];
		//
		//
		//struct duk_hbufferobject *buf = (struct duk_hbufferobject *) duk_to_pointer(ctx, 1);
		//snprintf(bufasd, sizeof bufasd, "invalid buffer heapptr=%d arr=%d", buf, 0);
		//ImGui::Text(bufasd);
		//ret = 0;
	} else {

		ret = ImGui::DragFloat(label, arr);
	}
	//printf("changed: %d\n", ret);
	duk_push_int(ctx, ret);
	return 1;
}


int duk_func_imgui_button(duk_context *ctx) {
	char *label = (char *)duk_to_string(ctx, 0);
	int ret = ImGui::Button(label);
	//if (ret)
	//	printf("ret: %d\n", ret);
	duk_push_int(ctx, ret);
	return 1;
}
int duk_func_imgui_text(duk_context *ctx) {
	char *text = (char *)duk_to_string(ctx, 0);
	ImGui::Text(text);
	return 0;
}

int duk_func_imgui_push_id(duk_context *ctx) {
	unsigned int id = duk_to_uint(ctx, 0);
	ImGui::PushID(id);
	return 0;
}

int duk_func_imgui_pop_id(duk_context *ctx) {
	ImGui::PopID();
	return 0;
}

int duk_func_imgui_line(duk_context *ctx) {
	//float a_x = (float)duk_to_number(ctx, 0);
	//float a_y = (float)duk_to_number(ctx, 1);
	//float b_x = (float)duk_to_number(ctx, 2);
	//float b_y = (float)duk_to_number(ctx, 3);
	//ImGuiWindow* window = ImGui::GetCurrentWindow();
	//ImVec2 a(a_x, a_y);
	//ImVec2 b(b_x, b_y);
	//window->DrawList->AddLine(a, b, 0xff000000, 2.0f);
	return 0;
}

int duk_func_imgui_point(duk_context *ctx) {
	//float a_x = (float)duk_to_number(ctx, 0);
	//float a_y = (float)duk_to_number(ctx, 1);
	//float radius = (float)duk_to_number(ctx, 2);
	//int color = duk_to_uint(ctx, 3);
	//ImGuiWindow* window = ImGui::GetCurrentWindow();
	//ImVec2 a(a_x, a_y);
	//window->DrawList->AddCircleFilled(a, radius, color);
	return 0;
}

int duk_func_imgui_set_cursor_pos_x(duk_context *ctx) {
	//float x = (float)duk_to_number(ctx, 0);
	//ImGuiWindow *window = ImGui::GetCurrentWindow();
	//window->DC.CursorPos.x = x;
	return 0;
}
int duk_func_imgui_set_cursor_pos_y(duk_context *ctx) {
	//float y = (float)duk_to_number(ctx, 0);
	//ImGuiWindow *window = ImGui::GetCurrentWindow();
	//window->DC.CursorPos.y = y;
	return 0;
}
int duk_func_imgui_get_cursor_pos_x(duk_context *ctx) {
	//ImGuiWindow *window = ImGui::GetCurrentWindow();
	//duk_push_number(ctx, window->DC.CursorPos.x);
	return 1;
}
int duk_func_imgui_get_cursor_pos_y(duk_context *ctx) {
	//ImGuiWindow *window = ImGui::GetCurrentWindow();
	//duk_push_number(ctx, window->DC.CursorPos.y);
	return 1;
}


int duk_func_imgui_is_mouse_dragging(duk_context *ctx) {
	int ret = ImGui::IsMouseDragging();
	duk_push_int(ctx, ret);
	return 1;
}

int duk_func_imgui_mouse_delta(duk_context *ctx) {
	duk_push_array(ctx);
	duk_push_number(ctx, ImGui::GetIO().MouseDelta.x);
	duk_put_prop_string(ctx, -2, "x");
	duk_push_number(ctx, ImGui::GetIO().MouseDelta.y);
	duk_put_prop_string(ctx, -2, "y");
	return 1;
}

int duk_func_imgui_mouse_released(duk_context *ctx) {
	int button = duk_to_int(ctx, 0);
	int ret = ImGui::IsMouseReleased(button);
	duk_push_int(ctx, ret);
	return 1;
}
int duk_func_imgui_set_delta(duk_context *ctx) {
	float x = (float)duk_to_number(ctx, 0);
	float y = (float)duk_to_number(ctx, 1);
	ImGui::GetIO().MouseDelta.x = x;
	ImGui::GetIO().MouseDelta.y = y;
	return 0;
}
int duk_func_imgui_reset_mouse_drag_delta(duk_context *ctx) {
	ImGui::ResetMouseDragDelta(0 /*button*/);
	return 0;
}


int duk_func_imgui_is_item_clicked(duk_context *ctx) {
	int ret = ImGui::IsItemClicked();
	duk_push_int(ctx, ret);
	return 1;
}
int duk_func_imgui_is_item_active(duk_context *ctx) {
	int ret = ImGui::IsItemActive();
	duk_push_int(ctx, ret);
	return 1;
}
int duk_func_imgui_is_item_hovered(duk_context *ctx) {
	int ret = ImGui::IsItemHovered();
	duk_push_int(ctx, ret);
	return 1;
}
int duk_func_imgui_is_item_hovered_rect(duk_context *ctx) {
	int ret = ImGui::IsItemHoveredRect();
	duk_push_int(ctx, ret);
	return 1;
}

int duk_func_imgui_tree_node(duk_context *ctx) {
	char *label = (char *)duk_to_string(ctx, 0);
	int ret = ImGui::TreeNode(label);
	duk_push_int(ctx, ret);
	return 1;
}
int duk_func_imgui_tree_pop(duk_context *ctx) {
	ImGui::TreePop();
	return 0;
}

int duk_func_imgui_checkbox(duk_context *ctx) {
	char *label = (char *)duk_to_string(ctx, 0);
	bool *address = (bool *)duk_to_int(ctx, 1);
	int ret = ImGui::Checkbox("Align label with current X position)", address);
	duk_push_int(ctx, ret);
	return 1;
}

int duk_func_imgui_push_style_var_indent_spacing(duk_context *ctx) {
	//ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize()*3); // Increase spacing to differentiate leaves from expanded contents.
	ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, 10.0f); // Increase spacing to differentiate leaves from expanded contents.
	return 0;
}
int duk_func_imgui_pop_style_var(duk_context *ctx) {
	ImGui::PopStyleVar();
	return 0;
}

int duk_func_imgui_tree_node_ex(duk_context *ctx) {
	int pointer_id = duk_to_int(ctx, 0);
	int node_flags = duk_to_int(ctx, 1);
	char *label = (char *)duk_to_string(ctx, 2);
	int node_open = ImGui::TreeNodeEx((void*)pointer_id, node_flags, label);
	duk_push_int(ctx, node_open);
	return 1;
}

int duk_func_imgui_window_rect(duk_context *ctx) {
	duk_push_array(ctx);
	ImVec2 winsize = ImGui::GetWindowSize();

	duk_push_number(ctx, winsize.x);
	duk_put_prop_string(ctx, -2, "width");
	duk_push_number(ctx, winsize.y);
	duk_put_prop_string(ctx, -2, "height");
	return 1;
}


int duk_func_imgui_begin_group(duk_context *ctx) {
	ImGui::BeginGroup();
	return 0;
}
int duk_func_imgui_end_group(duk_context *ctx) {
	ImGui::EndGroup();
	return 0;
}
int duk_func_imgui_push_item_width(duk_context *ctx) {
	float width = (float)duk_to_number(ctx, 0);
	ImGui::PushItemWidth(width);
	return 0;
}
int duk_func_imgui_pop_item_width(duk_context *ctx) {
	ImGui::PopItemWidth();
	return 0;
}
int duk_func_imgui_same_line(duk_context *ctx) {
	float local_pos_x = (float)duk_to_number(ctx, 0);
	float spacing_w = (float)duk_to_number(ctx, 1);
	ImGui::SameLine(local_pos_x, spacing_w);
	return 0;
}

bool ImGui_ImplOpenGL2_CreateFontsTexture();
int duk_func_imgui_create_fonts_texture(duk_context *ctx) {
	ImGui_ImplOpenGL2_CreateFontsTexture();
	return 0;
}


void js_register_method(duk_context *ctx, char *name, int (*func)(duk_context *ctx));

void duktape_bind_imgui(duk_context *ctx) {
	struct funcis funcs[] = {
		{"imgui_drag_float",	    duk_func_imgui_drag_float    },
		{"imgui_button"                          ,duk_func_imgui_button                             },
		{"imgui_text"                            ,duk_func_imgui_text                               },
		{"imgui_push_id"                         ,duk_func_imgui_push_id                            },
		{"imgui_pop_id"                          ,duk_func_imgui_pop_id                             },
		{"imgui_line"                            ,duk_func_imgui_line                               },
		{"imgui_point"                           ,duk_func_imgui_point                              },
		{"imgui_set_cursor_x"                    , duk_func_imgui_set_cursor_pos_x                  },
		{"imgui_set_cursor_y"                    , duk_func_imgui_set_cursor_pos_y                  },
		{"imgui_get_cursor_x"                    , duk_func_imgui_get_cursor_pos_x                  },
		{"imgui_get_cursor_y"                    , duk_func_imgui_get_cursor_pos_y                  },


		{"imgui_is_mouse_dragging"               , duk_func_imgui_is_mouse_dragging                 },
		{"imgui_mouse_delta"                     , duk_func_imgui_mouse_delta                       },
		{"imgui_mouse_released"                  , duk_func_imgui_mouse_released                    },
		{"imgui_set_delta"                       , duk_func_imgui_set_delta                         },
		{"imgui_reset_mouse_drag_delta"          , duk_func_imgui_reset_mouse_drag_delta            },


		{"imgui_is_item_clicked"                 , duk_func_imgui_is_item_clicked                   },
		{"imgui_is_item_active"                  , duk_func_imgui_is_item_active                    },
		{"imgui_is_item_hovered"                 , duk_func_imgui_is_item_hovered                   },
		{"imgui_is_item_hovered_rect"            , duk_func_imgui_is_item_hovered_rect              },

		{"imgui_tree_node"                       , duk_func_imgui_tree_node                         },
		{"imgui_tree_pop"                        , duk_func_imgui_tree_pop                          },
		{"imgui_checkbox"                        , duk_func_imgui_checkbox                          },
																									  
		{"imgui_push_style_var_indent_spacing"   , duk_func_imgui_push_style_var_indent_spacing     },
		{"imgui_pop_style_var"                   , duk_func_imgui_pop_style_var                     },
																									  
		{"imgui_tree_node_ex"                    , duk_func_imgui_tree_node_ex                      },
		{"imgui_window_rect"                     ,duk_func_imgui_window_rect                        },

		{"imgui_begin_group"                     , duk_func_imgui_begin_group                       },
		{"imgui_end_group"                       , duk_func_imgui_end_group                         },
		{"imgui_push_item_width"                 , duk_func_imgui_push_item_width                   },
		{"imgui_pop_item_width"                  , duk_func_imgui_pop_item_width                    },
		{"imgui_same_line"                       , duk_func_imgui_same_line                         },
		{"imgui_create_fonts_texture"            , duk_func_imgui_create_fonts_texture              },

		{NULL, NULL}
	};
	for (int i=0; funcs[i].name; i++) {
		js_register_function(ctx, funcs[i].name, funcs[i].func);
	}



	struct funcis methods[] = {
		{"DragFloat"                       , duk_func_imgui_drag_float                        },
		{"Button"                          , duk_func_imgui_button                            },
		{"Text"                            , duk_func_imgui_text                              },
		{"PushID"                          , duk_func_imgui_push_id                           },
		{"PopID"                           , duk_func_imgui_pop_id                            },
		{"Line"                            , duk_func_imgui_line                              },
		{"Point"                           , duk_func_imgui_point                             },
		{"SetCursorPosX"                   , duk_func_imgui_set_cursor_pos_x                  },
		{"SetCursorPosY"                   , duk_func_imgui_set_cursor_pos_y                  },
		{"GetCursorPosX"                   , duk_func_imgui_get_cursor_pos_x                  },
		{"GetCursorPosY"                   , duk_func_imgui_get_cursor_pos_y                  },
		{"IsMouseDragging"                 , duk_func_imgui_is_mouse_dragging                 },
		{"GetMouseDelta"                   , duk_func_imgui_mouse_delta                       },
		{"IsMouseReleased"                 , duk_func_imgui_mouse_released                    },
		{"SetMouseDelta"                   , duk_func_imgui_set_delta                         },
		{"ResetMouseDragDelta"             , duk_func_imgui_reset_mouse_drag_delta            },
		{"IsItemClicked"                   , duk_func_imgui_is_item_clicked                   },
		{"IsItemActive"                    , duk_func_imgui_is_item_active                    },
		{"IsItemHovered"                   , duk_func_imgui_is_item_hovered                   },
		{"IsItemHovered_rect"              , duk_func_imgui_is_item_hovered_rect              },
		{"TreeNode"                        , duk_func_imgui_tree_node                         },
		{"TreeNodeEx"                      , duk_func_imgui_tree_node_ex                      },
		{"TreePop"                         , duk_func_imgui_tree_pop                          },
		{"Checkbox"                        , duk_func_imgui_checkbox                          },
		{"PushStyleVar_IndentSpacing"      , duk_func_imgui_push_style_var_indent_spacing     }, // todo: get all the available style vars in JS?
		{"PopStyleVar"                     , duk_func_imgui_pop_style_var                     },
		{"GetWindowSize"                   , duk_func_imgui_window_rect                       },
		{"BeginGroup"                      , duk_func_imgui_begin_group                       },
		{"EndGroup"                        , duk_func_imgui_end_group                         },
		{"PushItemWidth"                   , duk_func_imgui_push_item_width                   },
		{"PopItemWidth"                    , duk_func_imgui_pop_item_width                    },
		{"SameLine"                        , duk_func_imgui_same_line                         },
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

}