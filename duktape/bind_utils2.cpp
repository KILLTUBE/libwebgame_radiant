#include <stdafx.h>

#include "duktapestuff.h"
#include "../duktape-2-3-0/duktape.h"
#include "bind_utils.h"

int duk_func_utils_screensize(duk_context *ctx) {
	//char *address = (char *)duk_to_int(ctx, 0);
	//char *text = (char *)duk_to_string(ctx, 1);
	//strncpy(address, text, 256);
	int w = 800, h = 600;
	duk_push_object(ctx);
	duk_push_int(ctx, w);
	duk_put_prop_string(ctx, -2, "width");
	//duk_put_prop_index(ctx, -2, 0);
	duk_push_int(ctx, h);
	//duk_put_prop_index(ctx, -2, 1);
	duk_put_prop_string(ctx, -2, "height");
	return 1;
}

void bind_utils() {
	struct funcis funcs[] = {
		{"utils_screensize",	    duk_func_utils_screensize    },
		{NULL, NULL}
	};
	for (int i=0; funcs[i].name; i++) {
		js_register_function(ctx, funcs[i].name, funcs[i].func);
	}
}