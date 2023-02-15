#include "bitmap.h"
#include "trace.h"
#include <assert.h>
#include <stdlib.h>

void test_bitmap() {
  struct trace *trace = create_trace();
  struct bitmap *bitmap = create_bitmap(0, trace);

  assert(ok(trace));
  assert(!get_bit_in_bitmap(42, bitmap));

  set_bit_in_bitmap(42, bitmap, trace);
  assert(ok(trace));
  assert(get_bit_in_bitmap(42, bitmap));
  assert(!get_bit_in_bitmap(2048, bitmap));

  set_bit_in_bitmap(2048, bitmap, trace);
  assert(ok(trace));
  assert(get_bit_in_bitmap(42, bitmap));
  assert(get_bit_in_bitmap(2048, bitmap));

  free_bitmap(bitmap);
  free(trace);
}
