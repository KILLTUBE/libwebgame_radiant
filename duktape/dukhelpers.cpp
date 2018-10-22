#include "stdafx.h"

#include "dukhelpers.h"



#include "../duktape-2-3-0/duktape.h""
#include "dukdebugheaders.h"
#if 0
#include <opsys/opsys.h>

duk_idx_t dukrub_push_bare_object(duk_context* ctx) {
#if DUK_VERSION >= 20000
	return duk_push_bare_object(ctx);
#else
	duk_idx_t idx;

	idx = duk_push_object(ctx);
	duk_push_undefined(ctx);
	duk_set_prototype(ctx, -2);
	return idx;
#endif
}

void duk_ref_heapptr(duk_context* ctx, void* heapptr) {
	duk_push_global_stash(ctx);
	if (!duk_get_prop_string(ctx, -1, "refs")) {
		dukrub_push_bare_object(ctx);
		duk_put_prop_string(ctx, -3, "refs");
		duk_get_prop_string(ctx, -2, "refs");
		duk_replace(ctx, -2);
	}

	/* [ ... stash refs ] */
	
	duk_push_sprintf(ctx, "%p", heapptr);
	if (duk_get_prop(ctx, -2)) {
		/* [ stash refs ref_obj ] */
		
		duk_get_prop_string(ctx, -1, "refcount");
		duk_push_number(ctx, duk_get_number(ctx, -1) + 1);
		duk_put_prop_string(ctx, -3, "refcount");
		duk_pop_n(ctx, 4);
	}
	else {
		/* [ stash refs undefined ] */

		duk_push_sprintf(ctx, "%p", heapptr);
		dukrub_push_bare_object(ctx);
		duk_push_number(ctx, 1.0);
		duk_put_prop_string(ctx, -2, "refcount");
		duk_push_heapptr(ctx, heapptr);
		duk_put_prop_string(ctx, -2, "value");

		/* [ stash refs undefined key ref_obj ] */

		duk_put_prop(ctx, -4);
		duk_pop_3(ctx);
	}
}

void duk_unref_heapptr(duk_context* ctx, void* heapptr) {
	double refcount;

	duk_push_global_stash(ctx);
	if (!duk_get_prop_string(ctx, -1, "refs")) {
		dukrub_push_bare_object(ctx);
		duk_put_prop_string(ctx, -3, "refs");
		duk_get_prop_string(ctx, -2, "refs");
		duk_replace(ctx, -2);
	}

	/* [ ... stash refs ] */

	duk_push_sprintf(ctx, "%p", heapptr);
	if (duk_get_prop(ctx, -2)) {
		/* [ stash refs ref_obj ] */

		duk_get_prop_string(ctx, -1, "refcount");
		refcount = duk_get_number(ctx, -1) - 1.0;
		if (refcount > 0.0) {
			duk_push_number(ctx, refcount);
			duk_put_prop_string(ctx, -3, "refcount");
		}
		else {
			duk_push_sprintf(ctx, "%p", heapptr);
			duk_del_prop(ctx, -4);
		}
		duk_pop_n(ctx, 4);
	}
	else {
		/* [ stash refs undefined ] */

		duk_pop_3(ctx);
	}
}


float *js_push_vec2(duk_context *ctx) {
	//  ... BUFFER
	// 2 * 4 = 8 bytes for float[3]
	float *buf = (float *) duk_push_buffer(ctx, 2 * 4, 0);

	// ... BUFFER FLOAT32BUFFER
	duk_push_buffer_object(ctx, -1, 0, 2 * 4, DUK_BUFOBJ_FLOAT32ARRAY);

	// ... FLOAT32BUFFER
	duk_remove(ctx, -2);
	return buf;
}



void duk_put_external_float32array(duk_context *ctx, float *address, int numberOfElements, char *name) {
	duk_push_external_buffer(ctx);
	duk_config_buffer(ctx, -1, (void *) address, (duk_size_t) numberOfElements * sizeof(float));
	duk_push_buffer_object(ctx, -1, 0, numberOfElements * sizeof(float), DUK_BUFOBJ_FLOAT32ARRAY);
	duk_remove(ctx, -2); // only keep the float32 view
	duk_put_prop_string(ctx, -2, name);
}

#if 1
void duk_push_opsystem(duk_context *ctx, OpSystem *native) {
	if (native->javascript_this == 0) {
	
		// lets make new object, if there is none yet
		duk_push_object(ctx);

		// todo: make those non-overwriteable
		//duk_put_external_float32array(ctx, (float *)&native->pos, 2, "pos");
		//duk_put_external_float32array(ctx, (float *)&native->size, 2, "size");

		// save the heapptr
		native->javascript_this = duk_get_heapptr(ctx, -1);

		// op.address = (uint)ret;
		duk_push_uint(ctx, (int)native);
		duk_put_prop_string(ctx, -2, "address");

		// ref the heapptr, so "op = undefined" does not kill our reference
		duk_ref_heapptr(ctx, native->javascript_this);


		// set the op pointer to userdata, so we can quickly access the "address" without getting op.address
		//struct duk_hobject *tmp = (struct duk_hobject *) duk_get_heapptr(ctx, -1);
		struct duk_hobject *tmp = (struct duk_hobject *) native->javascript_this;
		tmp->userdata = (void *) native;
	} else {
		// just push the old objet, if we already generated one
		duk_push_heapptr(ctx, native->javascript_this);
	}
}

void duk_push_op(duk_context *ctx, Op *op) {
	if (op->javascript_this == 0) {
	
		// lets make new object, if there is none yet
		duk_push_object(ctx);

		// todo: make those non-overwriteable
		duk_put_external_float32array(ctx, (float *)&op->pos, 2, "pos");
		duk_put_external_float32array(ctx, (float *)&op->size, 2, "size");

		// save the heapptr
		op->javascript_this = duk_get_heapptr(ctx, -1);

		// op.address = (uint)ret;
		duk_push_uint(ctx, (int)op);
		duk_put_prop_string(ctx, -2, "address");

		// ref the heapptr, so "op = undefined" does not kill our reference
		duk_ref_heapptr(ctx, op->javascript_this);


		// set the op pointer to userdata, so we can quickly access the "address" without getting op.address
		//struct duk_hobject *tmp = (struct duk_hobject *) duk_get_heapptr(ctx, -1);
		struct duk_hobject *tmp = (struct duk_hobject *) op->javascript_this;
		tmp->userdata = (void *) op;
	} else {
		// just push the old objet, if we already generated one
		duk_push_heapptr(ctx, op->javascript_this);
	}
}

// todo: merge LinkOutput and LinkInput into one superclass with 2 small derivations? 
void duk_push_input(duk_context *ctx, Link *native) {
	if (native->javascript_this == 0) {
	
		// lets make new object, if there is none yet
		duk_push_object(ctx);


		// todo: make those non-overwriteable
		duk_put_external_float32array(ctx, (float *)&native->val_f  , 1, "vec1");
		duk_put_external_float32array(ctx, (float *) native->vector2, 2, "vec2");
		duk_put_external_float32array(ctx, (float *) native->vector3, 3, "vec3");
		duk_put_external_float32array(ctx, (float *) native->vector4, 4, "vec4");
		duk_put_external_float32array(ctx, (float *) native->matrix, 16, "matrix");

		// save the heapptr
		native->javascript_this = duk_get_heapptr(ctx, -1);

		// op.address = (uint)ret;
		duk_push_uint(ctx, (int)native);
		duk_put_prop_string(ctx, -2, "address");

		// ref the heapptr, so "op = undefined" does not kill our reference
		duk_ref_heapptr(ctx, native->javascript_this);


		// set the op pointer to userdata, so we can quickly access the "address" without getting op.address
		//struct duk_hobject *tmp = (struct duk_hobject *) duk_get_heapptr(ctx, -1);
		struct duk_hobject *tmp = (struct duk_hobject *) native->javascript_this;
		tmp->userdata = (void *) native;
	} else {
		// just push the old object, if we already generated one
		duk_push_heapptr(ctx, native->javascript_this);
	}
}

void duk_push_output(duk_context *ctx, LinkOutput *native) {
	if (native->javascript_this == 0) {
	
		// lets make new object, if there is none yet
		duk_push_object(ctx);

		// todo: make those non-overwriteable
		duk_put_external_float32array(ctx, (float *)&native->val_f  , 1, "vec1");
		duk_put_external_float32array(ctx, (float *) native->vector2, 2, "vec2");
		duk_put_external_float32array(ctx, (float *) native->vector3, 3, "vec3");
		duk_put_external_float32array(ctx, (float *) native->vector4, 4, "vec4");
		duk_put_external_float32array(ctx, (float *) native->matrix, 16, "matrix");

		// save the heapptr
		native->javascript_this = duk_get_heapptr(ctx, -1);

		// op.address = (uint)ret;
		duk_push_uint(ctx, (int)native);
		duk_put_prop_string(ctx, -2, "address");

		// ref the heapptr, so "op = undefined" does not kill our reference
		duk_ref_heapptr(ctx, native->javascript_this);


		// set the op pointer to userdata, so we can quickly access the "address" without getting op.address
		//struct duk_hobject *tmp = (struct duk_hobject *) duk_get_heapptr(ctx, -1);
		struct duk_hobject *tmp = (struct duk_hobject *) native->javascript_this;
		tmp->userdata = (void *) native;
	} else {
		// just push the old object, if we already generated one
		duk_push_heapptr(ctx, native->javascript_this);
	}
}
#endif


void duk_push_vector(duk_context *ctx, float *native, void **heaptpr, int number_of_elements) {
	if (*heaptpr == 0) {
		//log("(should be 1) top=%d top_index=%d\n", duk_get_top(ctx), duk_get_top_index(ctx));
		// http://wiki.duktape.org/HowtoBuffers1x.html
		duk_push_external_buffer(ctx);
		duk_config_buffer(ctx, -1, (void *) native, (duk_size_t) number_of_elements * sizeof(float));
		duk_push_buffer_object(ctx, -1, 0, number_of_elements * sizeof(float), DUK_BUFOBJ_FLOAT32ARRAY);
		duk_remove(ctx, -2); // only keep the float32 view
		// save the heapptr
		*heaptpr = duk_get_heapptr(ctx, -1);

		//struct duk_hbufferobject *buf = (struct duk_hbufferobject *) *heaptpr;
		//buf->buf;

		// ref the heapptr, so "op = undefined" does not kill our reference
		duk_ref_heapptr(ctx, *heaptpr);

		//log("(should be 2) top=%d top_index=%d\n", duk_get_top(ctx), duk_get_top_index(ctx));
	} else {
		// just push the old object, if we already generated one
		duk_push_heapptr(ctx, *heaptpr);
	}
}

// todo: throw error when no userpointer AND/OR wrong type (add type field...)
void *duk_get_userpointer(duk_context *ctx, int idx) {
	struct duk_hobject *tmp = (struct duk_hobject *) duk_get_heapptr(ctx, idx);
	return tmp->userdata;
}
#endif
