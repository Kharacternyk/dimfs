#include "config.h"
#include "set.h"
#include "trace.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

void check_default_config(struct config *config);

void test_config_lua() {
  struct trace *trace = create_trace();
  struct config *config = load_config(NULL, trace);
  assert(ok(trace));
  check_default_config(config);

  config = load_config(TEST_ROOT "/lua/empty.lua", trace);
  assert(ok(trace));
  check_default_config(config);

  config = load_config(TEST_ROOT "/lua/override.lua", trace);
  assert(ok(trace));

  const struct set *editors = get_editors(config);
  assert(!is_in_set("vi", editors));
  assert(is_in_set("vim", editors));
  assert(is_in_set("nvim", editors));
  assert(is_in_set("rsession", editors));
  assert(is_in_set("cat", editors));

  const char *version_pattern = get_version_pattern(config);
  assert(!strcmp(version_pattern, "override"));

  free_config(config);

  config = load_config(TEST_ROOT "/lua/path-override.lua", trace);
  assert(ok(trace));
  const struct set *overridden_paths = get_overridden_paths(config);
  assert(get_best_match_count_in_set("/", '/', overridden_paths) == 0);
  assert(get_best_match_count_in_set("/tmp", '/', overridden_paths) ==
         path_excluded);
  assert(get_best_match_count_in_set("/tmp/subdir", '/', overridden_paths) ==
         path_excluded);
  assert(get_best_match_count_in_set("/home", '/', overridden_paths) == 0);
  assert(get_best_match_count_in_set("/home/nazar/.cache", '/',
                                     overridden_paths) == path_excluded);
  assert(get_best_match_count_in_set("/home/nazar/src/klunok", '/',
                                     overridden_paths) == path_included);
  free_config(config);

  load_config(TEST_ROOT "/lua/broken-semantics.lua", trace);
  assert(!ok(trace));
  catch_all(trace);

  load_config(TEST_ROOT "/lua/broken-syntax.lua", trace);
  assert(!ok(trace));
  catch_all(trace);

  free(trace);
}
