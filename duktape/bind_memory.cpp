#include <stdafx.h>

#include "duktapestuff.h"
#include "../duktape-2-3-0/duktape.h"	

int duk_func_memory_write_string(duk_context *ctx) {
	char *address = (char *)duk_to_pointer(ctx, 0);
	char *text = (char *)duk_to_string(ctx, 1);
	strncpy(address, text, 256);
	return 0;
}

void bind_memory() {
	struct funcis funcs[] = {
		{"memory_write_string",	    duk_func_memory_write_string    },
		{NULL, NULL}
	};
	for (int i=0; funcs[i].name; i++) {
		js_register_function(ctx, funcs[i].name, funcs[i].func);
	}
}