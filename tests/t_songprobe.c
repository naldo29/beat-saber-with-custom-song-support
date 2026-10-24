/* Portable mocks exercise the failure/wait paths without an Apple SDK. */
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "klepton.h"
#include "kl_songprobe.h"

static const char *scenario;
static int ready, calls, lookups, debug_class, model_class, method;
static const void *assemblies[] = { &model_class };
int kl_env_on(const char *name, int dflt) {
    assert(!strcmp(name, "KL_SONG_PROBE"));
    (void)dflt;
    return strcmp(scenario, "disabled") != 0;
}
kl_image *kl_find_image(const char *name) {
    assert(!strcmp(name, "libil2cpp.so"));
    lookups++;
    return !strcmp(scenario, "no-image") ? NULL : (kl_image *)&model_class;
}
static void *domain_get(void) { return ready ? &model_class : NULL; }
static const void **domain_get_assemblies(void *d, size_t *n) {
    assert(d == &model_class);
    *n = !strcmp(scenario, "no-assemblies") ? 0 : 1;
    return assemblies;
}
static void *assembly_get_image(const void *a) { return (void *)a; }
static void *class_from_name(void *i, const char *ns, const char *name) {
    (void)i; (void)ns;
    if (!strcmp(scenario, "no-model") && !strcmp(name, "BeatmapLevelsModel")) return NULL;
    return !strcmp(name, "Debug") ? &debug_class : &model_class;
}
static const void *class_get_method_from_name(void *k, const char *name, int n) {
    assert(k == &debug_class && !strcmp(name, "Log") && n == 1);
    return !strcmp(scenario, "no-log") ? NULL : &method;
}
static const void *class_get_methods(void *k, void **iter) {
    (void)k;
    if (*iter) return NULL;
    *iter = &method;
    return &method;
}
static const char *method_get_name(const void *m) { assert(m == &method); return "Example"; }
static unsigned method_get_param_count(const void *m) { assert(m == &method); return 2; }
static void *string_new(const char *s) { return (void *)s; }
static void *runtime_invoke(const void *m, void *o, void **args, void **exc) {
    assert(m == &method && o == NULL && strstr(args[0], "song-probe"));
    calls++;
    *exc = !strcmp(scenario, "exception") ? &method : NULL;
    return NULL; /* Void success must not be interpreted as failure. */
}
void *kl_sym(kl_image *i, const char *name) {
    assert(i);
    if (!strcmp(scenario, "missing-export") && !strcmp(name, "il2cpp_string_new")) return NULL;
#define SYM(fn) if (!strcmp(name, "il2cpp_" #fn)) return (void *)fn
    SYM(domain_get); SYM(domain_get_assemblies); SYM(assembly_get_image);
    SYM(class_from_name); SYM(class_get_method_from_name); SYM(class_get_methods);
    SYM(method_get_name); SYM(method_get_param_count); SYM(string_new); SYM(runtime_invoke);
#undef SYM
    assert(!"unexpected symbol");
    return NULL;
}
int main(int argc, char **argv) {
    assert(argc == 2);
    scenario = argv[1];
    const char *target = !strcmp(scenario, "other-target") ? "superhot" : "beatsaber140";
    kl_songprobe_tick(target, 90); /* Domain not ready: must not invoke anything. */
    assert(calls == 0);
    ready = 1;
    kl_songprobe_tick(target, 120);
    kl_songprobe_tick(target, 150); /* Success/error must be terminal. */
    kl_songprobe_tick(target, 7200);
    int expect_call = !strcmp(scenario, "success") || !strcmp(scenario, "exception");
    assert(calls == expect_call);
    if (!strcmp(scenario, "disabled") || !strcmp(scenario, "other-target")) assert(lookups == 0);
    puts("PASS");
    return 0;
}
