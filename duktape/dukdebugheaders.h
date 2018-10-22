#ifndef DUKDEBUGHEADERS_H
#define DUKDEBUGHEADERS_H

#include "../ccall.h"

// as used in op_mesh_editor.cpp op_cube.cpp etc.

struct duk_heaphdr {
	duk_uint32_t h_flags;

#if defined(DUK_USE_REFERENCE_COUNTING)
#if defined(DUK_USE_REFCOUNT16)
	duk_uint16_t h_refcount16;
#else
	duk_size_t h_refcount;
#endif
#endif

#if defined(DUK_USE_HEAPPTR16)
	duk_uint16_t h_next16;
#else
	struct duk_heaphdr *h_next;
#endif

#if defined(DUK_USE_DOUBLE_LINKED_HEAP)
	/* refcounting requires direct heap frees, which in turn requires a dual linked heap */
#if defined(DUK_USE_HEAPPTR16)
	duk_uint16_t h_prev16;
#else
	struct duk_heaphdr *h_prev;
#endif
#endif

	/* When DUK_USE_HEAPPTR16 (and DUK_USE_REFCOUNT16) is in use, the
	 * struct won't align nicely to 4 bytes.  This 16-bit extra field
	 * is added to make the alignment clean; the field can be used by
	 * heap objects when 16-bit packing is used.  This field is now
	 * conditional to DUK_USE_HEAPPTR16 only, but it is intended to be
	 * used with DUK_USE_REFCOUNT16 and DUK_USE_DOUBLE_LINKED_HEAP;
	 * this only matter to low memory environments anyway.
	 */
#if defined(DUK_USE_HEAPPTR16)
	duk_uint16_t h_extra16;
#endif
};

struct duk_hobject {
	struct duk_heaphdr hdr;

	/*
	 *  'props' contains {key,value,flags} entries, optional array entries, and
	 *  an optional hash lookup table for non-array entries in a single 'sliced'
	 *  allocation.  There are several layout options, which differ slightly in
	 *  generated code size/speed and alignment/padding; duk_features.h selects
	 *  the layout used.
	 *
	 *  Layout 1 (DUK_USE_HOBJECT_LAYOUT_1):
	 *
	 *    e_size * sizeof(duk_hstring *)         bytes of   entry keys (e_next gc reachable)
	 *    e_size * sizeof(duk_propvalue)         bytes of   entry values (e_next gc reachable)
	 *    e_size * sizeof(duk_uint8_t)           bytes of   entry flags (e_next gc reachable)
	 *    a_size * sizeof(duk_tval)              bytes of   (opt) array values (plain only) (all gc reachable)
	 *    h_size * sizeof(duk_uint32_t)          bytes of   (opt) hash indexes to entries (e_size),
	 *                                                      0xffffffffUL = unused, 0xfffffffeUL = deleted
	 *
	 *  Layout 2 (DUK_USE_HOBJECT_LAYOUT_2):
	 *
	 *    e_size * sizeof(duk_propvalue)         bytes of   entry values (e_next gc reachable)
	 *    e_size * sizeof(duk_hstring *)         bytes of   entry keys (e_next gc reachable)
	 *    e_size * sizeof(duk_uint8_t) + pad     bytes of   entry flags (e_next gc reachable)
	 *    a_size * sizeof(duk_tval)              bytes of   (opt) array values (plain only) (all gc reachable)
	 *    h_size * sizeof(duk_uint32_t)          bytes of   (opt) hash indexes to entries (e_size),
	 *                                                      0xffffffffUL = unused, 0xfffffffeUL = deleted
	 *
	 *  Layout 3 (DUK_USE_HOBJECT_LAYOUT_3):
	 *
	 *    e_size * sizeof(duk_propvalue)         bytes of   entry values (e_next gc reachable)
	 *    a_size * sizeof(duk_tval)              bytes of   (opt) array values (plain only) (all gc reachable)
	 *    e_size * sizeof(duk_hstring *)         bytes of   entry keys (e_next gc reachable)
	 *    h_size * sizeof(duk_uint32_t)          bytes of   (opt) hash indexes to entries (e_size),
	 *                                                      0xffffffffUL = unused, 0xfffffffeUL = deleted
	 *    e_size * sizeof(duk_uint8_t)           bytes of   entry flags (e_next gc reachable)
	 *
	 *  In layout 1, the 'e_next' count is rounded to 4 or 8 on platforms
	 *  requiring 4 or 8 byte alignment.  This ensures proper alignment
	 *  for the entries, at the cost of memory footprint.  However, it's
	 *  probably preferable to use another layout on such platforms instead.
	 *
	 *  In layout 2, the key and value parts are swapped to avoid padding
	 *  the key array on platforms requiring alignment by 8.  The flags part
	 *  is padded to get alignment for array entries.  The 'e_next' count does
	 *  not need to be rounded as in layout 1.
	 *
	 *  In layout 3, entry values and array values are always aligned properly,
	 *  and assuming pointers are at most 8 bytes, so are the entry keys.  Hash
	 *  indices will be properly aligned (assuming pointers are at least 4 bytes).
	 *  Finally, flags don't need additional alignment.  This layout provides
	 *  compact allocations without padding (even on platforms with alignment
	 *  requirements) at the cost of a bit slower lookups.
	 *
	 *  Objects with few keys don't have a hash index; keys are looked up linearly,
	 *  which is cache efficient because the keys are consecutive.  Larger objects
	 *  have a hash index part which contains integer indexes to the entries part.
	 *
	 *  A single allocation reduces memory allocation overhead but requires more
	 *  work when any part needs to be resized.  A sliced allocation for entries
	 *  makes linear key matching faster on most platforms (more locality) and
	 *  skimps on flags size (which would be followed by 3 bytes of padding in
	 *  most architectures if entries were placed in a struct).
	 *
	 *  'props' also contains internal properties distinguished with a non-BMP
	 *  prefix.  Often used properties should be placed early in 'props' whenever
	 *  possible to make accessing them as fast a possible.
	 */

#if defined(DUK_USE_HEAPPTR16)
	/* Located in duk_heaphdr h_extra16.  Subclasses of duk_hobject (like
	 * duk_hcompiledfunction) are not free to use h_extra16 for this reason.
	 */
#else
	duk_uint8_t *props;
#endif

	/* prototype: the only internal property lifted outside 'e' as it is so central */
#if defined(DUK_USE_HEAPPTR16)
	duk_uint16_t prototype16;
#else
	struct duk_hobject *prototype;
#endif

#if defined(DUK_USE_OBJSIZES16)
	duk_uint16_t e_size16;
	duk_uint16_t e_next16;
	duk_uint16_t a_size16;
#if defined(DUK_USE_HOBJECT_HASH_PART)
	duk_uint16_t h_size16;
#endif
#else
	duk_uint32_t e_size;  /* entry part size */
	duk_uint32_t e_next;  /* index for next new key ([0,e_next[ are gc reachable) */
	duk_uint32_t a_size;  /* array part size (entirely gc reachable) */
#if defined(DUK_USE_HOBJECT_HASH_PART)
	duk_uint32_t h_size;  /* hash part size or 0 if unused */
#endif
#endif
	void *userdata; // kung foo man: ye whatever, i dont wanna use get/set prop
};

struct duk_hbufferobject {
	/* Shared object part. */
	struct duk_hobject obj;

	/* Underlying buffer (refcounted), may be NULL. */
	struct duk_hbuffer *buf;

	/* Slice and accessor information.
	 *
	 * Because the underlying buffer may be dynamic, these may be
	 * invalidated by the buffer being modified so that both offset
	 * and length should be validated before every access.  Behavior
	 * when the underlying buffer has changed doesn't need to be clean:
	 * virtual 'length' doesn't need to be affected, reads can return
	 * zero/NaN, and writes can be ignored.
	 *
	 * Note that a data pointer cannot be precomputed because 'buf' may
	 * be dynamic and its pointer unstable.
	 */

	duk_uint_t offset;       /* byte offset to buf */
	duk_uint_t length;       /* byte index limit for element access, exclusive */
	duk_uint8_t shift;       /* element size shift:
	                          *   0 = u8/i8
	                          *   1 = u16/i16
	                          *   2 = u32/i32/float
	                          *   3 = double
	                          */
	duk_uint8_t elem_type;   /* element type */
	duk_uint8_t is_view;
};






/* Shared prefix for all buffer types. */
struct duk_hbuffer {
	struct duk_heaphdr hdr;

	/* It's not strictly necessary to track the current size, but
	 * it is useful for writing robust native code.
	 */

	/* Current size (not counting a dynamic buffer's "spare"). */
#if defined(DUK_USE_BUFLEN16)
	/* Stored in duk_heaphdr unused flags. */
#else
	duk_size_t size;
#endif

	/*
	 *  Data following the header depends on the DUK_HBUFFER_FLAG_DYNAMIC
	 *  flag.
	 *
	 *  If the flag is clear (the buffer is a fixed size one), the buffer
	 *  data follows the header directly, consisting of 'size' bytes.
	 *
	 *  If the flag is set, the actual buffer is allocated separately, and
	 *  a few control fields follow the header.  Specifically:
	 *
	 *    - a "void *" pointing to the current allocation
	 *    - a duk_size_t indicating the full allocated size (always >= 'size')
	 *
	 *  If DUK_HBUFFER_FLAG_EXTERNAL is set, the buffer has been allocated
	 *  by user code, so that Duktape won't be able to resize it and won't
	 *  free it.  This allows buffers to point to e.g. an externally
	 *  allocated structure such as a frame buffer.
	 *
	 *  Unlike strings, no terminator byte (NUL) is guaranteed after the
	 *  data.  This would be convenient, but would pad aligned user buffers
	 *  unnecessarily upwards in size.  For instance, if user code requested
	 *  a 64-byte dynamic buffer, 65 bytes would actually be allocated which
	 *  would then potentially round upwards to perhaps 68 or 72 bytes.
	 */
};

/* Fixed buffer; data follows struct, with proper alignment guaranteed by
 * struct size.
 */
#if (DUK_USE_ALIGN_BY == 8) && defined(DUK_USE_PACK_MSVC_PRAGMA)
#pragma pack(push, 8)
#endif
struct duk_hbuffer_fixed {
	/* A union is used here as a portable struct size / alignment trick:
	 * by adding a 32-bit or a 64-bit (unused) union member, the size of
	 * the struct is effectively forced to be a multiple of 4 or 8 bytes
	 * (respectively) without increasing the size of the struct unless
	 * necessary.
	 */
	union {
		struct {
			struct duk_heaphdr hdr;
#if defined(DUK_USE_BUFLEN16)
			/* Stored in duk_heaphdr unused flags. */
#else
			duk_size_t size;
#endif
		} s;
#if (DUK_USE_ALIGN_BY == 4)
		duk_uint32_t dummy_for_align4;
#elif (DUK_USE_ALIGN_BY == 8)
		duk_double_t dummy_for_align8;
#elif (DUK_USE_ALIGN_BY == 1)
		/* no extra padding */
#else
#error invalid DUK_USE_ALIGN_BY
#endif
	} u;

	/*
	 *  Data follows the struct header.  The struct size is padded by the
	 *  compiler based on the struct members.  This guarantees that the
	 *  buffer data will be aligned-by-4 but not necessarily aligned-by-8.
	 *
	 *  On platforms where alignment does not matter, the struct padding
	 *  could be removed (if there is any).  On platforms where alignment
	 *  by 8 is required, the struct size must be forced to be a multiple
	 *  of 8 by some means.  Without it, some user code may break, and also
	 *  Duktape itself breaks (e.g. the compiler stores duk_tvals in a
	 *  dynamic buffer).
	 */
}
#if (DUK_USE_ALIGN_BY == 8) && defined(DUK_USE_PACK_GCC_ATTR)
__attribute__ ((aligned (8)))
#elif (DUK_USE_ALIGN_BY == 8) && defined(DUK_USE_PACK_CLANG_ATTR)
__attribute__ ((aligned (8)))
#endif
;
#if (DUK_USE_ALIGN_BY == 8) && defined(DUK_USE_PACK_MSVC_PRAGMA)
#pragma pack(pop)
#endif

/* Dynamic buffer with 'curr_alloc' pointing to a dynamic area allocated using
 * heap allocation primitives.  Also used for external buffers when low memory
 * options are not used.
 */
struct duk_hbuffer_dynamic {
	struct duk_heaphdr hdr;

#if defined(DUK_USE_BUFLEN16)
	/* Stored in duk_heaphdr unused flags. */
#else
	duk_size_t size;
#endif

#if defined(DUK_USE_HEAPPTR16)
	/* Stored in duk_heaphdr h_extra16. */
#else
	void *curr_alloc;  /* may be NULL if alloc_size == 0 */
#endif

	/*
	 *  Allocation size for 'curr_alloc' is alloc_size.  There is no
	 *  automatic NUL terminator for buffers (see above for rationale).
	 *
	 *  'curr_alloc' is explicitly allocated with heap allocation
	 *  primitives and will thus always have alignment suitable for
	 *  e.g. duk_tval and an IEEE double.
	 */
};

/* External buffer with 'curr_alloc' managed by user code and pointing to an
 * arbitrary address.  When heap pointer compression is not used, this struct
 * has the same layout as duk_hbuffer_dynamic.
 */
struct duk_hbuffer_external {
	struct duk_heaphdr hdr;

#if defined(DUK_USE_BUFLEN16)
	/* Stored in duk_heaphdr unused flags. */
#else
	duk_size_t size;
#endif

	/* Cannot be compressed as a heap pointer because may point to
	 * an arbitrary address.
	 */
	void *curr_alloc;  /* may be NULL if alloc_size == 0 */
};





#define DUK_HEAPHDR_FLAGS_TYPE_MASK      0x00000003UL
#define DUK_HEAPHDR_FLAGS_FLAG_MASK      (~DUK_HEAPHDR_FLAGS_TYPE_MASK)

                                             /* 2 bits for heap type */
#define DUK_HEAPHDR_FLAGS_HEAP_START     2   /* 4 heap flags */
#define DUK_HEAPHDR_FLAGS_USER_START     6   /* 26 user flags */

#define DUK_HEAPHDR_HEAP_FLAG_NUMBER(n)  (DUK_HEAPHDR_FLAGS_HEAP_START + (n))
#define DUK_HEAPHDR_USER_FLAG_NUMBER(n)  (DUK_HEAPHDR_FLAGS_USER_START + (n))
#define DUK_HEAPHDR_HEAP_FLAG(n)         (1UL << (DUK_HEAPHDR_FLAGS_HEAP_START + (n)))
#define DUK_HEAPHDR_USER_FLAG(n)         (1UL << (DUK_HEAPHDR_FLAGS_USER_START + (n)))

#define DUK_HEAPHDR_FLAG_REACHABLE       DUK_HEAPHDR_HEAP_FLAG(0)  /* mark-and-sweep: reachable */
#define DUK_HEAPHDR_FLAG_TEMPROOT        DUK_HEAPHDR_HEAP_FLAG(1)  /* mark-and-sweep: children not processed */
#define DUK_HEAPHDR_FLAG_FINALIZABLE     DUK_HEAPHDR_HEAP_FLAG(2)  /* mark-and-sweep: finalizable (on current pass) */
#define DUK_HEAPHDR_FLAG_FINALIZED       DUK_HEAPHDR_HEAP_FLAG(3)  /* mark-and-sweep: finalized (on previous pass) */

#define DUK_HTYPE_MIN                    1
#define DUK_HTYPE_STRING                 1
#define DUK_HTYPE_OBJECT                 2
#define DUK_HTYPE_BUFFER                 3
#define DUK_HTYPE_MAX                    3



/*
 *  Flags
 *
 *  Fixed buffer:     0
 *  Dynamic buffer:   DUK_HBUFFER_FLAG_DYNAMIC
 *  External buffer:  DUK_HBUFFER_FLAG_DYNAMIC | DUK_HBUFFER_FLAG_EXTERNAL
 */

#define DUK_HEAPHDR_CHECK_FLAG_BITS(h,bits)  (((h)->h_flags & (bits)) != 0)

#define DUK_HBUFFER_FLAG_DYNAMIC                  DUK_HEAPHDR_USER_FLAG(0)    /* buffer is behind a pointer, dynamic or external */
#define DUK_HBUFFER_FLAG_EXTERNAL                 DUK_HEAPHDR_USER_FLAG(1)    /* buffer pointer is to an externally allocated buffer */

#define DUK_HBUFFER_HAS_DYNAMIC(x)                DUK_HEAPHDR_CHECK_FLAG_BITS(&(x)->hdr, DUK_HBUFFER_FLAG_DYNAMIC)
#define DUK_HBUFFER_HAS_EXTERNAL(x)               DUK_HEAPHDR_CHECK_FLAG_BITS(&(x)->hdr, DUK_HBUFFER_FLAG_EXTERNAL)

#define DUK_HBUFFER_SET_DYNAMIC(x)                DUK_HEAPHDR_SET_FLAG_BITS(&(x)->hdr, DUK_HBUFFER_FLAG_DYNAMIC)
#define DUK_HBUFFER_SET_EXTERNAL(x)               DUK_HEAPHDR_SET_FLAG_BITS(&(x)->hdr, DUK_HBUFFER_FLAG_EXTERNAL)

#define DUK_HBUFFER_CLEAR_DYNAMIC(x)              DUK_HEAPHDR_CLEAR_FLAG_BITS(&(x)->hdr, DUK_HBUFFER_FLAG_DYNAMIC)
#define DUK_HBUFFER_CLEAR_EXTERNAL(x)             DUK_HEAPHDR_CLEAR_FLAG_BITS(&(x)->hdr, DUK_HBUFFER_FLAG_EXTERNAL)






//EXTERNC struct duk_hbufferobject *duk_to_buffer_kung(duk_context *ctx, duk_idx_t index, duk_size_t *out_size, duk_uint_t mode);

//float javascript_viewmatrix[16] = {0};
/*
buf = new Int32Array(16);
for (var i=0; i<16; i++)
buf[i] = i*i;
set_viewmatrix(buf)


memory_read_int(328846048 + 4*5)	

set_viewmatrix([1,2,3,4,5,6,7,8,9,10,11])

*/
// http://wiki.duktape.org/HowtoBuffers.html

//int duk_func_set_viewmatrix       (duk_context *ctx) {
//	unsigned int size;
//	struct duk_hbufferobject *buf;
//	buf = duk_to_buffer_kung(ctx, 0, &size, DUK_BUF_MODE_DONTCARE);


#endif