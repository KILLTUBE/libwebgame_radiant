#include "stdafx.h"

#include "duktapestuff.h"
#include "../duktape-2-3-0/duktape.h"
#include "../imgui/imgui_dock_console.h"
#include "bind_memory.h"
#include "bind_utils.h"
#include "bind_imgui.h"

duk_context *ctx = NULL;

int js_push_global_by_name(duk_context *ctx, char *name) {
	// [..., global]
	duk_push_global_object(ctx);
	// [..., global, prop || undefined]
	int function_exists = duk_get_prop_string(ctx, -1, name);
	// [..., prop || undefined]
	duk_remove(ctx, -2);
	return function_exists;
}

int js_eval_file_safe(duk_context *ctx, char *filename) {

	// [..., file_get_contents || undefined]
	int function_exists = js_push_global_by_name(ctx, "file_get_contents");
	if (function_exists) // [..., file_get_contents]
	{
		// [..., file_get_contents, filename]
		duk_push_string(ctx, filename);
		// [..., ret]
		// file_get_contents(name)
		int rc = duk_pcall(ctx, 1);
		if (rc != 0) { // [..., undefined]
			// e.g. Callback failed: TypeError: undefined not callable
			js_printf("js> duk_eval_file_safe(%s) failed: %s\n", filename, duk_safe_to_string(ctx, -1));
			// [...]
			duk_pop(ctx);
		} else { // [..., file_contents]


			// I would rather not use safeeval javascript code, but the pcompile/pcall fails to give me a full error stack print when lib.js contains parsing errors
			// Oh well, I just forgot to check the return value of duk_pcompile()
			#if 0
				js_push_global_by_name(ctx, "safeeval");
				duk_push_global_object(ctx);
				//duk_push_int(ctx, (int) widget);
				duk_dup(ctx, -3);
				int ret = duk_pcall(ctx, 2);
				if (ret != 0) {
					Sys_Printf("safeeval failed on C level: %s\n", duk_safe_to_string(ctx, -1));
				}
				duk_pop(ctx);
			#else
				// [..., file_contents, filename]
				duk_push_string(ctx, filename);
				// [..., function || err]
				int ret = duk_pcompile(ctx, 0 /*DUK_COMPILE_SAFE | DUK_COMPILE_EVAL*/); // didn't see any difference in error reporting lol

				if (ret != 0) {
					js_printf("js> duk_eval_file_safe(%s) duk_pcompile failed: %s\n", filename, duk_safe_to_string(ctx, -1));
				} else {
				
					// [..., ret]
					int ret = duk_pcall(ctx, 0);
					if (ret != 0) {
						// I just cannot get the whole stack trace error :(
						//duk_get_prop_string(ctx, -1, "stack");
						//Sys_Printf("ERROR  %s\n", duk_safe_to_string(ctx, -1));
						//duk_pop(ctx);
						js_printf("js> duk_eval_file_safe(%s) duk_pcall failed: %s\n", filename, duk_safe_to_string(ctx, -1));
					}
				}
			#endif

			// [...]
			duk_pop(ctx);
			return ret==0; // returns 1 if success
		}

	} else { // [..., undefined]
		// [...]
		duk_pop(ctx);
	}
	return 0;
}

// e.g. js_call(ctx, "callback_call", "iiii", callback_id, widget, x, y);
int js_call(duk_context *ctx, char *function, char *params, ...) {

	if (ctx == NULL) {
		//imgui_log("js_stuff.cpp> ctx==NULL\n");
		return 0;
	}

	// prepare stack
	js_push_global_by_name(ctx, function);

	// push all vars to the stack
	va_list args;
	va_start(args, params);
	int len = strlen(params);
	int i;
	int numberOfPushedArguments = 0;
	for (i=0; i<len; i++)
	{
		switch (params[i])
		{
			case 'i': {
				int tmp = va_arg(args, int);
				duk_push_int(ctx, tmp);
				numberOfPushedArguments++;
				break;
			}
			case 'f': {
				float tmp = va_arg(args, float);
				duk_push_number(ctx, tmp);
				numberOfPushedArguments++;
				break;
			}
			case 's': {
				char *tmp = va_arg(args, char *);
				duk_push_string(ctx, tmp);
				numberOfPushedArguments++;
				break;
			}
			default:
				js_printf("script> WARNING: Identifier '%c' is not implemented\n", params[i]);
		}
	}

	// call the function
	int ret = duk_pcall(ctx, numberOfPushedArguments);
	if (ret != 0) {
		js_printf("script> call of \"%s\" failed: %s\n", function, duk_safe_to_string(ctx, -1));
		//printf("script> call of \"%s\" failed: %s\n", function, duk_safe_to_string(ctx, -1));
		//__asm { int 3 }
	} else {
		//Sys_Printf("js> console_key_press() ret: %s\n", duk_safe_to_string(ctx, -1));
		//func_ret = duk_to_int(ctx, -1);
		//Sys_Printf("call back call success i guess\n");
	}
	// either case, stack needs to be cleaned
	duk_pop(ctx);

	return 1;
}

int js_eval(char *msg) {
	duk_eval_string(ctx, msg);
	duk_pop(ctx);
	return 1;
}

duk_ret_t duk_func_log(duk_context *ctx) {
	int i;
	int n = duk_get_top(ctx);  /* #args */
	char *res;
	for (i = 0; i < n; i++) {
		res = (char *)duk_to_string(ctx, i);
		js_printf("%s", res);
	}
	return 0;
}

int duk_func_file_get_contents(duk_context *ctx) {
#ifndef EMSCRIPTEN
	const char *filename = duk_to_string(ctx, 0);
	FILE *f = NULL;
	long len;
	void *buf;
	size_t got;
	if (!filename) {
		goto error;
	}
	f = fopen(filename, "rb");
	if (!f) {
		js_printf("Cant open file: %s\n", filename);
		printf("cant open file: %s", filename);
		goto error;
	}
	if (fseek(f, 0, SEEK_END) != 0) {
		
		js_printf("cant seek to end\n");
		goto error;
	}
	len = ftell(f);
	if (fseek(f, 0, SEEK_SET) != 0) {
		js_printf("cant seek to start\n");
		goto error;
	}
	buf = duk_push_fixed_buffer(ctx, (size_t) len);
	got = fread(buf, 1, len, f);
	if (got != (size_t) len) {
		js_printf("cant read content\n");
		goto error;
	}
	fclose(f);
	f = NULL;
	// convert the fixed buffer to string
	char *ret = (char *) duk_to_string(ctx, -1);
	//printf("ret=%s\n", ret); // would print the file contents
	return 1;
	
	error:
	if (f) {
		fclose(f);
	}
	js_printf("some error reading file in file_get_contents()\n");
	//return DUK_RET_ERROR;
	duk_push_undefined(ctx);
#else
	return 1;
#endif
}

int duk_func_file_put_contents(duk_context *ctx) {
	char *filename = (char *)duk_to_string(ctx, 0);
	char *text = (char *)duk_to_string(ctx, 1);
	FILE *f = fopen(filename, "w");
	if ( ! f)
	{
		duk_push_int(ctx, 0);
		return 1;
	}
	fputs(text, f);
	fclose(f);
	
	duk_push_int(ctx, 1);
	return 1;
}

int duk_func_get_global(duk_context *ctx) {
	duk_push_global_object(ctx);
	return 1;
}

int js_register_function(duk_context *ctx, char *name, int (*func)(duk_context *ctx)) {
	duk_push_c_function(ctx, func, DUK_VARARGS);
	duk_push_string(ctx, name);
	duk_put_prop_string(ctx, -2, "name");
	duk_put_global_string(ctx, name);
	return 1;
}

CCALL duk_context *js_get_ctx() { return ctx; }

CCALL void js_reload() {
	js_eval_file_safe(ctx, "assets\\javascript\\pre_create.js");
	js_eval_file_safe(ctx, "assets\\javascript\\lib.js");
	js_eval_file_safe(ctx, "assets\\javascript\\lib_quake.js");
	//js_call(ctx, "PostReload", "");
}

int duk_func_reload(duk_context *ctx) {
	js_reload();
	return 0;
}

int js_init() {
	ctx = duk_create_heap_default();
	
	//init_bullet_bindings(ctx);
	//init_imgui_bindings(ctx);
	//init_opengl_bindings(ctx);
	//init_recast_bindings(ctx);
	//init_win32_bindings(ctx);

	struct funcis funcs[] = {
		{"imgui_log"        , duk_func_log                    },
		{"file_get_contents", duk_func_file_get_contents      },
		{"file_put_contents", duk_func_file_put_contents      },
		{"reload"           , duk_func_reload                 },
		{"get_global"       , duk_func_get_global             },
		{NULL, NULL}
	};
	for (int i=0; funcs[i].name; i++) {
		js_register_function(ctx, funcs[i].name, funcs[i].func);
	}
	
	bind_memory();
	bind_utils();
	bind_imgui();

	// for some reason pcall doesn't give me a full error stack trace
	duk_peval_string(ctx, "function safeeval(global, code) { try { eval.bind(global)(code); } catch(e) { log(e.stack); }  }");
	duk_pop(ctx);

	// doesnt work for some reason, cba to look, just use file_get_contents and eval it *mein neger*
	//duk_eval_file(ctx, "c:\\Bastardiant\\javascript\\lib.js");
	//printf("Load lib.js: %s\n", duk_safe_to_string(ctx, -1));
	//duk_pop(ctx);

	// rudimentary js handling function, which can be easily overwritten by lib.js
	duk_peval_string(ctx, "handle_input = function(code) { try { ret = eval(code); log(ret) } catch (e) {log(\"error: \" + e + \"\\n\")} return \"not needed\"; }");
	duk_pop(ctx);

	
	//js_call(ctx, "reload", "");

	return 1;
}